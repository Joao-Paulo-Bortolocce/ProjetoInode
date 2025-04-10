#define NUMERO_ENTRADAS 10
#define NOME_ENTRADA 50
#define NOME_USUARIO "robson"
#define TAM_NOME_USUARIO 20
#define NOME_ABSOLUTO 256
#define QTD_PERMISSOES 11
#define QTD_DIRETOS 5
#define MAX_PILHA 10
#define PERMISSAO_ARQ "-rw-r--r--"
#define PERMISSAO_DIR "drwxr-xr-x"
#define PERMISSAO_LINK "lrwxr-xr-x"

struct TpEntrada {
    int inode;
    char nome[NOME_ENTRADA];
};

struct TpDiretorio {
    TpEntrada entradas[NUMERO_ENTRADAS];
    int tl;
};

struct TpData {
    int dia, mes, ano;
};

struct TpHorario {
    int hora, minuto, segundo;
};

struct TpHeader {
    char permissao[QTD_PERMISSOES];
    TpData data;
    TpHorario horario;
    char usuario[TAM_NOME_USUARIO], grupo[TAM_NOME_USUARIO];
    int tamanho;
    int contLink;
};

struct TpIndireto{
    int indiretos[QTD_DIRETOS];
    int tl;
};

struct TpInode {
    TpHeader header;
    int diretos[QTD_DIRETOS];
    int indireto, duplo, triplo;
};

struct TpPilha {
    int topo;
    int blocosLivres[MAX_PILHA]; // Supondo um limite m�ximo de 10 blocos livres
    int prox; //Indice do vetor que � a proxima pilha de blocos livres
};

struct TpBloco {
    char bad;
    TpInode inode;
    TpDiretorio dir;
    TpPilha livres;
    TpIndireto indireto;
    char softLink[NOME_ABSOLUTO];
};
