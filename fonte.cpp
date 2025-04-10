#include<stdio.h>
#include<windows.h>
#include<conio2.h>
#include<string.h>
#include<stdlib.h>

#include"structs.h"
#include"dataHorario.h"
#include"pilhaBlocosLivres.h"
#include"gerenciaBlocosLivres.h"
#include"gerenciaInode.h"
#include"gerenciaEntrada.h"
#include"gerenciaDir.h"

#include "relCaua.h"
//#include "gerenciaLink.h"

int topo;
int raiz;
int tamanhoDisc;
char pilhaDiretorio[20][15];
int iPilha = -1;

void limpa() {
#ifdef linux
	system("clear");
#else
	system("cls");
#endif

}

void incluiDir(char nome[15]) {
	iPilha++;
	strcpy(pilhaDiretorio[iPilha], nome);
}

void removeDir(char nomeDir[15]) {
	strcpy(nomeDir, pilhaDiretorio[iPilha]);
	pilhaDiretorio[iPilha][0] = '\0';
	iPilha--;
}

int buscaEntrada(TpBloco disco[],int pai, int &filho,char nome[]) { //recebe Inode do pai e posiciona o filho e retorna a posição dele no vetor de entradas
	int entradaFilho,dirPai;
	int i;
	char teste=1;
	for(i=0;i<disco[pai].inode.header.tamanho && teste;i++){
		dirPai=disco[pai].inode.diretos[i];
		for (entradaFilho=0;entradaFilho<disco[dirPai].dir.tl && strcmp(disco[dirPai].dir.entradas[entradaFilho].nome,nome)!=0;entradaFilho++);
		if(entradaFilho<disco[dirPai].dir.tl)
			teste=0;
	}



	if (entradaFilho<disco[dirPai].dir.tl) {
		filho=disco[dirPai].dir.entradas[entradaFilho].inode;
		return entradaFilho;
	}
	filho= -1;
	return -1;
}

int navegar(TpBloco disco[],int &pai,int &filho,char caminho[]) {
	int tl=strlen(caminho),i,j,auxPai,entradaFilho;
	char aux[tl];
	filho=pai;
	if (tl>0) {
		i=0;
		if (caminho[0]=='/') {
			pai=raiz;
			i++;
		}
		while (i<tl && filho>=0) {
			auxPai=filho;
			for (j=0;i<tl && caminho[i]!='/';i++,j++)
				aux[j]=caminho[i];
			aux[j]='\0';
			entradaFilho=buscaEntrada(disco,auxPai,filho,aux);
			i++;
		}
		if (filho>=0) {
			pai=auxPai;
			return entradaFilho;
		}
	}
	return 0;
}


int tamanhoDisco(){
	// textcolor(15);
	int tamanho;
	printf("Informe a quantidade de blocos que o sistema tera: ");
	scanf("%d",&tamanho);
	while(tamanho<=1){
		// textcolor(4);
		printf("A quantidade informada e insuficiente\n");
		// textcolor(15);
		printf("Informe a quantidade de blocos que o sistema tera: ");
		scanf("%d",&tamanho);
	}
	tamanhoDisc=tamanho;
	return tamanho;
}

void iniciarSistema(TpBloco disco[]){
	criarRaiz(raiz,disco,topo);
	criarDiretorio(raiz,disco,topo,"home");
	int home=disco[raiz].inode.diretos[0];
	int paiUsuario=disco[home].dir.entradas[2].inode;
	criarDiretorio(paiUsuario,disco,topo,NOME_USUARIO);
}

void inicializar(TpBloco disco[],int tamanho){
	topo=iniciarBlocosLivres(disco,tamanho);
	iniciarSistema(disco);
}

char isCaracResevado(char carac){
    if( carac==92 || carac=='*' || carac=='(' || carac==')' || carac=='@' ||  carac=='#' ||  carac=='?' ||  carac=='$' ||  carac==39 || carac==34 || carac==',')
        return 1;
    return 0;
}

char verificaNome(char nome[], int j){
    j++;
    int i,tl=strlen(nome);
    char aux[tl];
    for(i=0;j<tl && !isCaracResevado(nome[j]);j++,i++)
        aux[i]=nome[j];
    if(j<tl)
        return 0;
    aux[i]='\0';
    strcpy(nome,aux);
    return 1;
}


void pegaComando (char comando[], char res[]) {
	int i;
    for(i=0; i<strlen(comando) && comando[i]!= ' '; i++) {
        res[i] = comando[i];
    }
    res[i]='\0';
}


char separaNomeCaminho(char comando[], char nome[]) {// retorna true se caminho for diferente de nome
	int tl=strlen(comando);
	int i;
	for (i=tl;i>=0 && comando[i]!='/';i--);
	if (i<0) {
		strcpy(nome,comando);
		return 0;
	}
	i++;
	for (int j=0;i+j<tl;j++)
		nome[j]=comando[i+j];
	comando[i-1]='\0';
	nome[i]='\0';
	return 1;
}

int nomesDiretorios(TpBloco disco[], int diretorio, char nomesD[][100]) {
	int tam = disco[diretorio].dir.tl,i;
	for(i = 0; i<tam;i++) {
		if(disco[disco[diretorio].dir.entradas[i].inode].inode.header.permissao[0] == 'd') {
			strcpy(nomesD[i], disco[diretorio].dir.entradas[i].nome);
		}
	}
	return i;
}

int existeNomeDiretorio(char nomesD[][100], char nome[], int qtdDir) {
	int i;
	for(i = 0; i < qtdDir && stricmp(nome, nomesD[i]) != 0; i++);
	if(i < qtdDir)
		return 1; //nome encontrado
	return 0;
}

int buscaNomeDiretorio(TpBloco disco[], int &topo, TpDiretorio &diretorio, char nomeDir[]) {
	int pos;
	for(pos = 0; pos < diretorio.tl && strcmp(nomeDir, diretorio.entradas[pos].nome) != 0; pos++);
	if(pos < diretorio.tl)
		return pos;
	return -1;
}

void rmDir(TpBloco disco[], int &topo, int inodePai,TpDiretorio &diretorio , char nomeDir[]) {

    int pos = buscaNomeDiretorio(disco, topo, diretorio, nomeDir), inodeID, i, blocoDir;

	if (pos != -1) {
		inodeID = diretorio.entradas[pos].inode;
	    blocoDir = disco[inodeID].inode.diretos[0];
	
	    // Verifica se o diretório a ser removido está vazio
	    if (disco[blocoDir].dir.tl == 2) { // Apenas "." e ".."
	        // Libera o bloco e o inode
	        push(disco[topo].livres, blocoDir);
	        push(disco[topo].livres, inodeID);
	
	        // Remove a entrada do diretório atual
	        for (i = pos; i < diretorio.tl - 1; i++) {
	            diretorio.entradas[i] = diretorio.entradas[i + 1];
	        }
	        diretorio.tl--;
	
	        printf("Diretorio \"%s\" removido com sucesso.\n", nomeDir);
	    } else {
	        printf("Erro: diretorio \"%s\" nao esta vazio.\n", nomeDir);
	    }
	}
}

void incluiNomePasta(char caminho[NOME_ABSOLUTO], char comando[300], int inode, int entradaDir) {
	char nomePasta[15];
	if(strcmp(comando, "..") == 0){
		strcpy(nomePasta, "/");
		strcat(nomePasta, comando);
		strcat(caminho, nomePasta);
	}
	else {
		strcpy(nomePasta, "/");
		strcat(nomePasta, comando);
		if(caminho[strlen(caminho)-1] == '$') {
			strcat(caminho, nomePasta);	
		}
		else {
			int i;
			for(i=strlen(caminho); caminho[i]!='/'; i--);
			caminho[i] = '\0';
			strcat(caminho, nomePasta);
		}
	}
}

void terminal (TpBloco disco[]){
	int inode=raiz,filho;
	char usuario[7] = NOME_USUARIO;
	char maquina[17] = "@nomedamaquina:~";
	char caminho[NOME_ABSOLUTO]="\0";
	char comando[300];
	char firstComand[20];
	strcat(caminho,usuario);
	strcat(caminho,maquina);
	strcat(caminho,"$\0");
	do{
		printf("%s ",caminho);
		fflush(stdin);
		// gets(comando);
		scanf(" %[^\n]",&comando);
		// scanf("%s",&comando);
		pegaComando(comando,firstComand);
		if(strcmp(firstComand,"mkdir")==0){
			char nomesD[100][100];
			int qtdDir = nomesDiretorios(disco, disco[inode].inode.diretos[0], nomesD);
			if(verificaNome(comando,strlen(firstComand)) && !existeNomeDiretorio(nomesD, comando, qtdDir)){

				char nome[strlen(comando)];
				if(separaNomeCaminho(comando,nome)) {
					navegar(disco,inode,filho,comando);
					inode = filho;

				}
				criarDiretorio(inode,disco,topo,nome);
			}
			else
			{
				printf("%s Nao pode ser o nome de uma pasta", comando);
			}
		}
		else
		{
			if(strcmp(firstComand,"ls")==0){
				relatorio6(disco,disco[inode].inode.diretos[0]);
			}
			else
			{
				if(strcmp(firstComand,"cd")==0){
					if(verificaNome(comando,strlen(firstComand))){
						int entradaDir = navegar(disco,inode,filho,comando);
						inode = filho;
						if(strcmp(comando, "..") == 0) {
							char nomeDir[15];
							if(iPilha>=0)
								removeDir(nomeDir);
								
							if(iPilha != -1) {
								removeDir(nomeDir);
								incluiNomePasta(caminho, nomeDir, inode, 0);	
							}
							else
								incluiNomePasta(caminho, "", inode, 0);
						}
						else {
							incluiNomePasta(caminho, comando, inode, entradaDir);
							incluiDir(comando);
						}
					}
					else {
						printf("nao foi possivel ir ate %s", comando);
					}

				}
				else
				{
					if(strcmp(firstComand,"fb")==0){//verifica quais blocos estão livres e faz sua listagem
						listarBlocosLivres(disco, topo);
					}
					else
					{
						if(strcmp(firstComand,"rmDir")==0) {
							char nomesD[100][100];
							int qtdDir = nomesDiretorios(disco, disco[inode].inode.diretos[0], nomesD);
							if(verificaNome(comando,strlen(firstComand)) && existeNomeDiretorio(nomesD, comando, qtdDir))
								rmDir(disco, topo, inode ,disco[disco[inode].inode.diretos[0]].dir, comando);
						} 
						else {
							if(strcmp(comando,"")==0) {
								
							} 
							else {
								printf("%s nao e reconhecido como um comando do sistema", firstComand);
							}
						}
						
					}
				}
			}
		}

		printf("\n");

	}while(strcmp(comando,"shutdown")!=0);
}



void executar(int tamanho){
	TpBloco disco[tamanho];
	inicializar(disco,tamanho);
	terminal(disco);
}

int main(){
	int tamanho = tamanhoDisco();
	executar(tamanho);
}
