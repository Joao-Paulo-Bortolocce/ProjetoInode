void criarDiretorio(int pai,TpBloco disco[],int &topo, char nome[]){
	int posInode=gerarInode(disco,topo,0,1);
	int dir = disco[posInode].inode.diretos[0];
	disco[dir].dir.tl=2;
	disco[dir].dir.prox=-1;
	disco[dir].dir.entradas[0].inode=posInode;
	strcpy(disco[dir].dir.entradas[0].nome,".");
	disco[dir].dir.entradas[1].inode=pai;
	strcpy(disco[dir].dir.entradas[1].nome,"..");
	criarEntrada(pai,disco,topo,posInode,nome);
}

void criarRaiz(int &raiz,TpBloco disco[],int &topo){
	raiz=gerarInode(disco,topo,0,1);
	int dir = disco[raiz].inode.diretos[0];
	disco[dir].dir.tl=2;
	disco[dir].dir.prox=-1;
	disco[dir].dir.entradas[0].inode=raiz;
	strcpy(disco[dir].dir.entradas[0].nome,".");
	disco[dir].dir.entradas[1].inode=raiz;
	strcpy(disco[dir].dir.entradas[1].nome,"..");
}

/*
struct TpDiretorio {
    TpEntrada entradas[NUMERO_ENTRADAS];
    int prox; //indice da continuação das entradas de diretorio
    int tl;
};

struct TpEntrada {
    int inode;
    char nomeArq[NOME_ARQUIVO];
};
*/
