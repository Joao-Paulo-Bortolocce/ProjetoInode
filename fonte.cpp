#include<stdio.h>
// #include<windows.h>
// #include<conio2.h>
#include<string.h>


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

void limpa() {
#ifdef _linux_
	system("clear");
#else
	system("cls");
#endif

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
	textcolor(15);
	int tamanho;
	printf("Informe a quantidade de blocos que o sistema tera: ");
	scanf("%d",&tamanho);
	while(tamanho<=1){
		textcolor(4);
		printf("A quantidade informada e insuficiente\n");
		textcolor(15);
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
    if(carac==126 || carac==47 || carac=='.' || carac==92 || carac=='*' || carac=='(' || carac==')' || carac=='@' ||  carac=='#' ||  carac=='?' ||  carac=='$' ||  carac==39 || carac==34 || carac==',')
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


void terminal (TpBloco disco[]){
	int inode=raiz;
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
		gets(comando);
		pegaComando(comando,firstComand);
		if(strcmp(firstComand,"mkdir")==0){
			if(verificaNome(comando,strlen(firstComand))){
				criarDiretorio(inode,disco,topo,comando);
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
					int filho=0;
					if(verificaNome(comando,strlen(firstComand))){
						navegar(disco,inode,filho,comando);
						inode = filho;
					}

				}
				else
				{
					if(strcmp(comando,"")==0){

					}
					else
					{
						printf("%s nao e reconhecido como um comando do sistema", firstComand);
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