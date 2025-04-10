void criaHardLink (int pai,TpBloco disco[],int &topo, char nome[], char nomeLink[]){
	int i =0;
	while(strcmp(disco[pai].dir.entradas[i].nome,nome)!=0 && i< disco[pai].dir.tl){
		printf("%s\n",disco[pai].dir.entradas[i].nome);
		i++;
	}
	
	if(i<disco[pai].dir.tl){
        if(disco[disco[pai].dir.entradas[i].inode].inode.header.permissao[0]=='-'){
        	criarEntrada(pai,disco,topo,disco[pai].dir.entradas[i].inode,nomeLink);
			disco[disco[pai].dir.entradas[i].inode].inode.header.contLink++;
        }else
          {
			printf("Nao e possivel criar um link para esse tipo de arquivo");
          }

	}
	else
	{
		printf("Nao existe nenhum arquivo com esse nome %s",nome);
	}
	
}

// void criaSoftLink (int pai,TpBloco disco[],int &topo, char nome[], char nomeLink[]){
// 	char caminho [NOME_ABSOLUTO] = "", aux[NOME_ABSOLUTO];
// 	int pos = pai,i;
//
// 	while(strcmp(disco[pai].dir.entradas[i],nome)!=0 && i< NUMERO_ENTRADAS){
// 		i++;
// 	}
//
// 	if(i<NUMERO_ENTRADAS){
// 		while(disco[pos].dir.entradas[0].inode.diretos[0] != disco[pos].dir.entradas[1].inode.diretos[0]){
// 			strcpy(aux,"");
// 			strcpy(aux,disco[pos].dir.entradas[1].nome);
// 			strcat(aux,caminho);
// 			strcpy(caminho,aux);
// 			pos = disco[pos].dir.entradas[1].inode.diretos[0];
// 		}
//
// 		int inode = gerarInode(disco,topo,2,1);
// 		strcpy(disco[disco[inode].diretos[0]].softLink,nomeLink);
// 		criarEntrada(pai,disco,topo,inode,nomeLink);
// 	}
// 	else
// 	{
// 		//nao encontrou o nome
// 	}
// }
