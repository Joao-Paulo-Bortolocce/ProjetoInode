void criaHardLink (int pai,TpBloco disco[],int &topo, char nome[], char nomeLink[]){
	int i =0;
	while(strcmp(disco[pai].dir.entradas[i],nome)!=0 && i< NUMERO_ENTRADAS){
		i++;
	}
	
	if(i<NUMERO_ENTRADAS){
		criarEntrada(pai,disco,topo,disco[pai].dir.entradas[i].inode,nomeLink);
		disco[pai].dir.entradas[i].inode.contLink++;
	}
	else
	{
		//nao encontrou o nome
	}
	
}

void criaSoftLink (int pai,TpBloco disco[],int &topo, char nome[], char nomeLink[]){
	char caminho [NOME_ABSOLUTO] = "", aux[NOME_ABSOLUTO];
	int pos = pai,i;
	
	while(strcmp(disco[pai].dir.entradas[i],nome)!=0 && i< NUMERO_ENTRADAS){
		i++;
	}
	
	if(i<NUMERO_ENTRADAS){
		while(disco[pos].dir.entradas[0].inode.diretos[0] != disco[pos].dir.entradas[1].inode.diretos[0]){
			strcpy(aux,"");
			strcpy(aux,disco[pos].dir.entradas[1].nome);
			strcat(aux,caminho);
			strcpy(caminho,aux);
			pos = disco[pos].dir.entradas[1].inode.diretos[0];
		}
		
		int inode = gerarInode(disco,topo,2,1);
		strcpy(disco[disco[inode].diretos[0]].softLink,nomeLink);
		criarEntrada(pai,disco,topo,inode,nomeLink);
	}
	else
	{
		//nao encontrou o nome
	}
}
