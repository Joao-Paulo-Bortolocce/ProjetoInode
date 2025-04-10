void adicionaBloco(int pai, TpBloco disco[],int &topo) {
	if (disco[pai].inode.header.tamanho<QTD_DIRETOS) {
		disco[pai].inode.diretos[disco[pai].inode.header.tamanho]=getBlocoLivre(disco,topo);
		disco[pai].inode.header.tamanho++;
	}
}

void criarEntrada(int pai,TpBloco disco[],int &topo,int posInode,char nome[]){
	int i,dir;
	if(disco[pai].inode.header.tamanho< QTD_DIRETOS){

		for(i=0;i<disco[pai].inode.header.tamanho && disco[disco[pai].inode.diretos[i]].dir.tl==NUMERO_ENTRADAS;i++);
		if(i==disco[pai].inode.header.tamanho){
			adicionaBloco(pai,disco,topo);
			disco[disco[pai].inode.diretos[i]].dir.tl=0;
		}
		dir=disco[pai].inode.diretos[i];
	}
	/*if(i==QTD_DIRETOS){
		auxPai=disco[pai].inode.indireto;
		for(i=0;i< disco[auxPai].indireto.tl && disco[disco[auxPai].indireto.indiretos[i]]].dir.tl==NUMERO_ENTRADAS,i++,j++);
		if(i<QTD_DIRETOS){
			if(i<tl)
				dir=disco[auxPai].indireto.indiretos[i];
			else{
				adicionaBloco(pai,disco);
				dir=disco[auxPai].indireto.indiretos[i];
				disco[dir].dir.tl=0;
			}
		}

	}
	else{

	}*/
	disco[dir].dir.entradas[disco[dir].dir.tl].inode=posInode;
	strcpy(disco[dir].dir.entradas[disco[dir].dir.tl].nome,nome);
	disco[dir].dir.tl++;
}


/*	if(disco[pai].dir.prox<0){
		disco[pai].dir.prox=getBlocoLivre(disco,topo);
		disco[disco[pai].dir.prox].dir.tl=0;
	}
	pai=disco[pai].dir.prox;*/