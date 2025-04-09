#include<stdio.h>
//#include<conio2.h>
#include<string.h>
#include<stdlib.h>


#include"structs.h"
#include"dataHorario.h"
#include"pilhaBlocosLivres.h"
#include"gerenciaBlocosLivres.h"
#include"gerenciaInode.h"
#include"gerenciaEntrada.h"
#include"gerenciaDir.h"

int topo;
int raiz;

void limpa() {
#ifdef __linux__
	system("clear");
#else
	system("cls");
#endif

}

int buscaEntrada(TpBloco disco[],int pai, int &filho,char nome[]) { //recebe Inode do pai e posiciona o filho e retorna a posição dele no vetor de entradas
	int entradaFilho,dirPai=disco[pai].inode.diretos[0];
	do {
		for (entradaFilho=0;entradaFilho<disco[dirPai].dir.tl && strcmp(disco[dirPai].dir.entradas[entradaFilho].nome,nome)!=0;entradaFilho++);
	}while (entradaFilho==disco[dirPai].dir.tl && disco[dirPai].dir.prox>-1);

	if (entradaFilho<disco[dirPai].dir.tl) {
		filho=disco[dirPai].dir.entradas[entradaFilho].inode;
		return entradaFilho;
	}
	filho= -1;
	return -1;
}

int navegar(TpBloco disco[],int &pai,int &filho,char caminho[]) {
	int tl=strlen(caminho)>0,i,j,auxPai,entradaFilho;
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
//	textcolor(15);
	int tamanho;
	printf("Informe a quantidade de blocos que o sistema tera: ");
	scanf("%d",&tamanho);
	while(tamanho<=1){
//		textcolor(4);
		printf("A quantidade informada e insuficiente\n");
//		textcolor(15);
		printf("Informe a quantidade de blocos que o sistema tera: ");
		scanf("%d",&tamanho);
	}
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

void executar(int tamanho){
	TpBloco disco[tamanho];
	inicializar(disco,tamanho);
}

int main(){
	int tamanho= tamanhoDisco();
	executar(tamanho);
}
