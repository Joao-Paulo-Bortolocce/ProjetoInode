#include<stdio.h>
// #include<windows.h>
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
#include"gerenciarArquivo.h"
#include"gerenciaLink.h"
#include "relCaua.h"

int topo;
int raiz;
int tamanhoDisc;

void limpa() {
#ifdef linux
    system("clear");
#else
	system("cls");
#endif
}

int buscaEntrada(TpBloco disco[], int pai, int &filho, char nome[]) {
    //recebe Inode do pai e posiciona o filho e retorna a posi��o dele no vetor de entradas
    int entradaFilho, dirPai;
    int i;
    char teste = 1;
    for (i = 0; i < disco[pai].inode.header.tamanho && teste; i++) {
        dirPai = disco[pai].inode.diretos[i];
        for (entradaFilho = 0; entradaFilho < disco[dirPai].dir.tl && strcmp(
                                   disco[dirPai].dir.entradas[entradaFilho].nome, nome) != 0; entradaFilho++);
        if (entradaFilho < disco[dirPai].dir.tl)
            teste = 0;
    }
    if (entradaFilho < disco[dirPai].dir.tl) {
        filho = disco[dirPai].dir.entradas[entradaFilho].inode;
        return entradaFilho;
    }
    filho = -1;
    return -1;
}

int navegar(TpBloco disco[], int &pai, int &filho, char caminho[]) {
    int tl = strlen(caminho), i, j, auxPai, entradaFilho;
    char aux[tl];
    filho = pai;
    if (tl > 0) {
        i = 0;
        if (caminho[0] == '/') {
            pai = raiz;
            i++;
        }
        while (i < tl && filho >= 0) {
            auxPai = filho;
            for (j = 0; i < tl && caminho[i] != '/'; i++, j++)
                aux[j] = caminho[i];
            aux[j] = '\0';
            entradaFilho = buscaEntrada(disco, auxPai, filho, aux);
            i++;
        }
        if (filho >= 0) {
            pai = auxPai;
            return entradaFilho;
        }
    }
    return 0;
}


int tamanhoDisco() {
    // textcolor(15);
    int tamanho;
    printf("Informe a quantidade de blocos que o sistema tera: ");
    scanf("%d", &tamanho);
    while (tamanho <= 1) {
        // textcolor(4);
        printf("A quantidade informada e insuficiente\n");
        // textcolor(15);
        printf("Informe a quantidade de blocos que o sistema tera: ");
        scanf("%d", &tamanho);
    }
    tamanhoDisc = tamanho;
    return tamanho;
}

void iniciarSistema(TpBloco disco[]) {
    criarRaiz(raiz, disco, topo);
    criarDiretorio(raiz, disco, topo, "home");
    int home = disco[raiz].inode.diretos[0];
    int paiUsuario = disco[home].dir.entradas[2].inode;
    criarDiretorio(paiUsuario, disco, topo,NOME_USUARIO);
}

void inicializar(TpBloco disco[], int tamanho) {
    topo = iniciarBlocosLivres(disco, tamanho);
    iniciarSistema(disco);
}

char isCaracResevado(char carac) {
    if (carac == 92 || carac == '*' || carac == '(' || carac == ')' || carac == '@' || carac == '#' || carac == '?' ||
        carac == '$' || carac == 39 || carac == 34 || carac == ',')
        return 1;
    return 0;
}

char verificaNome(char nome[], int j) {
    j++;
    int i, tl = strlen(nome);
    char aux[tl];
    for (i = 0; j < tl && !isCaracResevado(nome[j]); j++, i++)
        aux[i] = nome[j];
    if (j < tl)
        return 0;
    aux[i] = '\0';
    strcpy(nome, aux);
    return 1;
}


void pegaComando(char comando[], char res[]) {
    int i;
    for (i = 0; i < strlen(comando) && comando[i] != ' '; i++) {
        res[i] = comando[i];
    }
    res[i] = '\0';
}


char separaNomeCaminho(char comando[], char nome[]) {
    // retorna true se caminho for diferente de nome
    int tl = strlen(comando);
    int i;
    for (i = tl-1; i >= 0 && comando[i] != '/'; i--);
    if (i < 0) {
        strcpy(nome, comando);
        return 0;
    }
    i++;
    for (int j = 0; i + j < tl; j++)
        nome[j] = comando[i + j];
    comando[i - 1] = '\0';
    nome[i] = '\0';
    return 1;
}

int nomesDiretorios(TpBloco disco[], int diretorio, char nomesD[][100]) {
    int tam = disco[diretorio].dir.tl, i;
    for (i = 0; i < tam; i++) {
        if (disco[disco[diretorio].dir.entradas[i].inode].inode.header.permissao[0] == 'd') {
            strcpy(nomesD[i], disco[diretorio].dir.entradas[i].nome);
        }
    }
    return i;
}

int pegaTamanho(char comando[]) {
    char tamVet[20];
    int tam, i = strlen(comando) - 1,aux;
    int j;
    for (; i >= 0 && comando[i] != ' ' && comando[i]>='0' && comando[i]<='9';  i--);
    if (i < 0 || comando[i] != ' ')
        return -1;
    aux=i;
    for (j = 0; i<comando[i]!='\0'; j++,i++) {
        tamVet[j] = comando[i];
    }
    tamVet[j] = '\0';
    comando[aux] = '\0';
    tam = atoi(tamVet);
    return tam;
}

int existeNomeDiretorio(char nomesD[][100], char nome[], int qtdDir) {
    int i;
    for (i = 0; i < qtdDir && strcmp(nome, nomesD[i]) != 0; i++);
    if (i < qtdDir)
        return 1; //nome encontrado
    return 0;
}

int buscaNomeDiretorio(TpBloco disco[], int &topo, TpDiretorio &diretorio, char nomeDir[]) {
    int pos;
    for (pos = 0; pos < diretorio.tl && strcmp(nomeDir, diretorio.entradas[pos].nome) != 0; pos++);
    if (pos < diretorio.tl)
        return pos;
    return -1;
}

void rmDir(TpBloco disco[], int &topo, int inodePai, TpDiretorio &diretorio, char nomeDir[]) {
    // int pos = buscaNomeDiretorio(disco, topo, diretorio, nomeDir), inodeID, i, blocoDir;
    int inodeID, pos = buscaEntrada(disco, inodePai, inodeID, nomeDir), i, blocoDir;

    if (pos != -1) {
        blocoDir = disco[inodeID].inode.diretos[0];

        // Verifica se o diret�rio a ser removido est� vazio
        if (disco[blocoDir].dir.tl == 2) {
            // Apenas "." e ".."
            // Libera o bloco e o inode
            push(disco[topo].livres, blocoDir);
            push(disco[topo].livres, inodeID);
            // Remove a entrada do diret�rio atual
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

void incluiNomePasta(char caminho[], char comando[300]) {
    if (strcmp(comando, "..") == 0) {
        char flag = 1;
        for (int i = strlen(caminho); i >= 0; i--) {
            if (caminho[i] == '/') {
                flag = 0;
            }
        }
        if (flag == 0) {
            int i;
            for (i = strlen(caminho) - 1; i > 0 && caminho[i] != '/'; i--);
            caminho[0]='/';
            if (i==0)
                i++;
            caminho[i] = '$';
            caminho[i + 1] = '\0';
        }
    } else {
        int len = strlen(caminho);
        int i = len-1;
        if (len > 2) {
            caminho[len-1] = '/';
            i++;
        }

        int j = 0;
        while (comando[j] != '\0' && i < NOME_ABSOLUTO - 2) {
            caminho[i++] = comando[j++];
        }

        caminho[i++] = '$';
        caminho[i] = '\0';
    }
}

void ls(char comando[], TpBloco disco[], int dir) {
    if (strcmp(comando, "\0") == 0) {
        for (int i = 0; i < disco[dir].dir.tl; i++) {
            if (strcmp(disco[dir].dir.entradas[i].nome, ".") != 0 && strcmp(disco[dir].dir.entradas[i].nome, "..") != 0)
                printf("%s  ", disco[dir].dir.entradas[i].nome);
        }
    }
    if (strcmp(comando, "-l") == 0) {
        for (int i = 0; i < disco[dir].dir.tl; i++) {
            printf("%s %d %s %s ", disco[disco[dir].dir.entradas[i].inode].inode.header.permissao,
                   disco[disco[dir].dir.entradas[i].inode].inode.header.contLink,
                   disco[disco[dir].dir.entradas[i].inode].inode.header.usuario,
                   disco[disco[dir].dir.entradas[i].inode].inode.header.grupo);
            switch (disco[disco[dir].dir.entradas[i].inode].inode.header.data.mes) {
                case 1:
                    printf("Jan %d %d %s", disco[disco[dir].dir.entradas[i].inode].inode.header.data.dia,
                           disco[disco[dir].dir.entradas[i].inode].inode.header.data.ano,
                           disco[dir].dir.entradas[i].nome);
                    break;
                case 2:
                    printf("Feb %d %d %s", disco[disco[dir].dir.entradas[i].inode].inode.header.data.dia,
                           disco[disco[dir].dir.entradas[i].inode].inode.header.data.ano,
                           disco[dir].dir.entradas[i].nome);
                    break;
                case 3:
                    printf("Mar %d %d %s", disco[disco[dir].dir.entradas[i].inode].inode.header.data.dia,
                           disco[disco[dir].dir.entradas[i].inode].inode.header.data.ano,
                           disco[dir].dir.entradas[i].nome);
                    break;
                case 4:
                    printf("Apr %d %d %s", disco[disco[dir].dir.entradas[i].inode].inode.header.data.dia,
                           disco[disco[dir].dir.entradas[i].inode].inode.header.data.ano,
                           disco[dir].dir.entradas[i].nome);
                    break;
                case 5:
                    printf("May %d %d %s", disco[disco[dir].dir.entradas[i].inode].inode.header.data.dia,
                           disco[disco[dir].dir.entradas[i].inode].inode.header.data.ano,
                           disco[dir].dir.entradas[i].nome);
                    break;
                case 6:
                    printf("Jun %d %d %s", disco[disco[dir].dir.entradas[i].inode].inode.header.data.dia,
                           disco[disco[dir].dir.entradas[i].inode].inode.header.data.ano,
                           disco[dir].dir.entradas[i].nome);
                    break;
                case 7:
                    printf("Jul %d %d %s", disco[disco[dir].dir.entradas[i].inode].inode.header.data.dia,
                           disco[disco[dir].dir.entradas[i].inode].inode.header.data.ano,
                           disco[dir].dir.entradas[i].nome);
                    break;
                case 8:
                    printf("Ago %d %d %s", disco[disco[dir].dir.entradas[i].inode].inode.header.data.dia,
                           disco[disco[dir].dir.entradas[i].inode].inode.header.data.ano,
                           disco[dir].dir.entradas[i].nome);
                    break;
                case 9:
                    printf("Set %d %d %s", disco[disco[dir].dir.entradas[i].inode].inode.header.data.dia,
                           disco[disco[dir].dir.entradas[i].inode].inode.header.data.ano,
                           disco[dir].dir.entradas[i].nome);
                    break;
                case 10:
                    printf("Oct %d %d %s", disco[disco[dir].dir.entradas[i].inode].inode.header.data.dia,
                           disco[disco[dir].dir.entradas[i].inode].inode.header.data.ano,
                           disco[dir].dir.entradas[i].nome);
                    break;
                case 11:
                    printf("Nov %d %d %s", disco[disco[dir].dir.entradas[i].inode].inode.header.data.dia,
                           disco[disco[dir].dir.entradas[i].inode].inode.header.data.ano,
                           disco[dir].dir.entradas[i].nome);
                    break;
                case 12:
                    printf("Dez %d %d %s", disco[disco[dir].dir.entradas[i].inode].inode.header.data.dia,
                           disco[disco[dir].dir.entradas[i].inode].inode.header.data.ano,
                           disco[dir].dir.entradas[i].nome);
                    break;
                default: ;
            }
            printf("\n");
        }
    }
}

void link(char comando[], int pai, TpBloco disco[], int &topo) {
    char tipo[3];
    char primeiroNome[NOME_ABSOLUTO];
    char segundoNome[NOME_ABSOLUTO];
    int i;
    for (i = 0; i < 2; i++) {
        tipo[i] = comando[i];
    }
    memmove(comando, comando + 3, strlen(comando) - 1);
    tipo[i] = '\0';
    if (strcmp(tipo, "-h") == 0) {
        //hardLink
        pegaComando(comando, primeiroNome);
        int TL = 0;
        for (int i = strlen(primeiroNome); i < strlen(comando); ++i) {
            segundoNome[TL++] = comando[i];
        }
        segundoNome[TL] = '\0';
        criaHardLink(pai, disco, topo, primeiroNome, segundoNome);
    }
    if (strcmp(tipo, "-s") == 0) {
        //soft
    }
}

void bad(TpBloco disco[], int numeroBloco) {
    disco[numeroBloco].bad=1;
}

char vi(TpBloco disco[], int filho) {
    int tam = disco[filho].inode.header.tamanho, i = 0, flag = 1;
    while(i < tam && flag) {
        if(disco[disco[filho].inode.diretos[i]].bad == 1)
            flag = 0;
        i++;
    }
    if(i < tam)
        return 0;
    return 1;
}


void terminal(TpBloco disco[]) {
    int inode = raiz, filho;
    char usuario[7] = NOME_USUARIO;
    char maquina[17] = "@nomedamaquina:";
    char caminho[NOME_ABSOLUTO] = "\0";
    char local[NOME_ABSOLUTO]= "/$\0";
    char comando[300];
    char firstComand[20];
    strcat(caminho, usuario);
    strcat(caminho, maquina);
    strcat(caminho, local);
    do {
        caminho[0]='\0';
        strcat(caminho, usuario);
        strcat(caminho, maquina);
        strcat(caminho, local);
        printf("%s ", caminho);
        fflush(stdin);
        // gets(comando);
        scanf(" %[^\n]", &comando);
        // scanf("%s",&comando);
        pegaComando(comando, firstComand);
        if (strcmp(firstComand, "mkdir") == 0) {
            char nomesD[100][100];
            int qtdDir = nomesDiretorios(disco, disco[inode].inode.diretos[0], nomesD);
            if (verificaNome(comando, strlen(firstComand)) && !existeNomeDiretorio(nomesD, comando, qtdDir)) {
                char nome[strlen(comando)];
                if (separaNomeCaminho(comando, nome)) {
                    navegar(disco, inode, filho, comando);
                    inode = filho;
                }
                criarDiretorio(inode, disco, topo, nome);
            } else {
                printf("%s Nao pode ser o nome de uma pasta", comando);
            }
        } else {
             if (strcmp(firstComand, "ls") == 0) {
                int tamD = disco[inode].inode.header.tamanho;
                verificaNome(comando, strlen(firstComand));
                for (int i = 0; i < tamD; i++)
                    ls(comando, disco, disco[inode].inode.diretos[i]);
            } else {
                if (strcmp(firstComand, "cd") == 0) {
                    if (verificaNome(comando, strlen(firstComand))) {
                        int entradaDir = navegar(disco, inode, filho, comando);
                        if (filho>0 && disco[filho].inode.header.permissao[0] == 'd') {
                            inode = filho;
                            incluiNomePasta(local, comando);
                        } else {
                            printf("voce esta tentando acessar algo que nao eh um diretorio!");
                        }
                    } else {
                        printf("nao foi possivel ir ate %s", comando);
                    }
                } else {
                    if (strcmp(firstComand, "fb") == 0) {
                        //verifica quais blocos est�o livres e faz sua listagem
                        listarBlocosLivres(disco, topo);
                    }
                    else{
                        if (strcmp(firstComand, "rmdir") == 0) {
                            char nomesD[100][100];
                            int qtdDir = nomesDiretorios(disco, disco[inode].inode.diretos[0], nomesD);
                            if (verificaNome(comando, strlen(firstComand)) && existeNomeDiretorio(
                                    nomesD, comando, qtdDir))
                                rmDir(disco, topo, inode, disco[disco[inode].inode.diretos[0]].dir, comando);
                        } else {
                            if (strcmp(firstComand, "touch") == 0) {

                                int tl = pegaTamanho(comando),aux=inode;
                                if (verificaNome(comando, strlen(firstComand)) && tl >= 0) {
                                    char nome[strlen(comando)];
                                    if (separaNomeCaminho(comando, nome)) {
                                        navegar(disco, inode, filho, comando);
                                        inode = filho;
                                    }
                                    if (buscaEntrada(disco,inode,filho,nome)<0)
                                        criarArquivo(inode, disco, topo, nome, tl / 10);
                                    else
                                        printf("o nome do arquivo %s ja existe nesse diretorio",nome);

                                }
                                inode=aux;
                            }
                            else {
                                if (strcmp(firstComand, "link") == 0) {
                                    if (verificaNome(comando, strlen(firstComand))) {
                                        link(comando, inode, disco, topo);
                                    }
                                } else {
                                    if (strcmp(firstComand, "bad") == 0) {
                                        int numeroBloco = pegaTamanho(comando);
                                        if (numeroBloco < tamanhoDisc) {
                                            bad(disco, numeroBloco);
                                            printf("numero do bloco %d se tornou bad", numeroBloco);
                                        } else {
                                            printf("numero do bloco invalido");
                                        }
                                    } else {
                                        if (strcmp(firstComand, "vi") == 0) {
                                            char aux[100];
                                            int posArq;
                                            if (verificaNome(comando, strlen(firstComand))) {
                                                posArq = buscaEntrada(disco, inode, filho, comando);
                                                if (filho >= 0) {
                                                    char arqCorrompido = vi(disco, filho);
                                                    if (arqCorrompido) {
                                                        printf("arquivo aberto");
                                                        }
                                                    } else
                                                        printf("arquivo %s corrompido", comando);
                                                }
                                            }
                                         else {
                                        	if(strcmp(firstComand, "rel6") == 0){
                                        		int tamD = disco[inode].inode.header.tamanho;
								                for (int i = 0; i < tamD; i++)
								                	relatorio6 (disco,disco[inode].inode.diretos[i]);
								                
                                        	}
                                        	else
                                            printf("%s nao e reconhecido como um comando do sistema", firstComand);
                                        }
                                    }
                                
                                }
                            }
                        }
                    }
                }
            }
        }

        printf("\n");
    } while (strcmp(comando, "shutdown") != 0);
}


void executar(int tamanho) {
    TpBloco disco[tamanho];
    inicializar(disco, tamanho);
    terminal(disco);
}

int main() {
    int tamanho = tamanhoDisco();
    executar(tamanho);
}
