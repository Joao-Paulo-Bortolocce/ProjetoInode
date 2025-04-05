#include<stdio.h>
#include<windows.h>
#include<conio2.h>
#include<string.h>


#include"structs.h"
#include"dataHorario.h"
#include"pilhaBlocosLivres.h"
#include"gerenciaBlocosLivres.h"
#include"gerenciaInode.h"
#include"gerenciaEntrada.h"
#include"gerenciaDir.h"

int topo;
int raiz;

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
