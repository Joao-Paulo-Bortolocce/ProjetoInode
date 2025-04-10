void criarArquivo(int pai,TpBloco disco[],int &topo, char nome[],int tamanho){
    int posInode=gerarInode(disco,topo,1,1);
    criarEntrada(pai,disco,topo,posInode,nome);
}
