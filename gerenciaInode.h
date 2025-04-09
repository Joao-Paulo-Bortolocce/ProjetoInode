void permissao(char permissao[],char tipo){
	switch(tipo){
		case 0: //Quer dizer que � diretorio
			strcpy(permissao,PERMISSAO_DIR);
			break;
		case 1: //Quer dizer que � arquivo
			strcpy(permissao,PERMISSAO_ARQ);
			break;
		case 2: //Quer dizer que � link
			strcpy(permissao,PERMISSAO_LINK);
			break;
	}
}

TpHeader gerarHeader(char tipo, int tamanho){
	TpHeader header;
	permissao(header.permissao,tipo);
	header.data=obterDataAtual();
	header.horario=obterHorarioAtual();
	strcpy(header.usuario,NOME_USUARIO);
	strcpy(header.grupo,NOME_USUARIO);
	header.tamanho=tamanho;
	header.contLink=1;
	return header;
}

int apontarDiretos(int vet[],int &tamanho,TpBloco disco[],int &topo,int inicio){
	int i;
	for(i=inicio;tamanho>0 && i<QTD_DIRETOS;i++,tamanho--){
		vet[i]=getBlocoLivre(disco,topo);
	}
	return i;
}

int apontarIndiretoDuplo(int duplo[],int &tamanho,TpBloco disco[],int &topo){
	int indice,i,tl;
	for(i=0;i<QTD_DIRETOS && tamanho>0;i++){
		indice=getBlocoLivre(disco,topo);
		duplo[i]=indice;
		tl=apontarDiretos(disco[indice].indireto.indiretos,tamanho,disco,topo,0);	
		disco[indice].indireto.tl=tl;
	}
	return i;
}

int apontarIndiretoTriplo(int triplo[],int &tamanho,TpBloco disco[],int &topo){
	int indice,i,tl;
	for(i=0;i<QTD_DIRETOS && tamanho>0;i++){
		indice=getBlocoLivre(disco,topo);
		triplo[i]=indice;
		tl=apontarIndiretoDuplo(disco[indice].indireto.indiretos,tamanho,disco,topo);
		disco[indice].indireto.tl=tl;
	}
	return i;
}

void apontarBlocos(TpInode &inode,int &tamanho,TpBloco disco[],int &topo, int inicio){
	apontarDiretos(inode.diretos,tamanho,disco,topo,inicio);
	if(tamanho>0){//adicionando indireto simple
		int indireto=getBlocoLivre(disco,topo),tl;
		inode.indireto=indireto;
		tl=apontarDiretos(disco[indireto].indireto.indiretos,tamanho,disco,topo,0);
		disco[indireto].indireto.tl=tl;
		if(tamanho>0){//Apontamento indireto dupl
			indireto=getBlocoLivre(disco,topo);
			inode.duplo=indireto;
			tl=apontarIndiretoDuplo(disco[indireto].indireto.indiretos,tamanho,disco,topo);
			disco[indireto].indireto.tl=tl;
			if(tamanho>0){
				indireto=getBlocoLivre(disco,topo);
				inode.triplo=indireto;
				tl=apontarIndiretoTriplo(disco[indireto].indireto.indiretos,tamanho,disco,topo);
				disco[indireto].indireto.tl=tl;
			}
		}
	}
}

int gerarInode(TpBloco disco[],int &topo,char tipo, int tamanho){
	int atual=getBlocoLivre(disco,topo),posAtual=atual;
	disco[atual].inode.header=gerarHeader(tipo,tamanho);
	apontarBlocos(disco[atual].inode,tamanho,disco,topo,0);
	while(tamanho>0){ //fazer o ultimo do triplo apontar para o novo Inod
		int aux,pos,novoInode;
		pos=disco[posAtual].inode.triplo;
		pos=disco[pos].indireto.indiretos[QTD_DIRETOS-1];
		pos=disco[pos].indireto.indiretos[QTD_DIRETOS-1];
		aux=disco[pos].indireto.indiretos[QTD_DIRETOS-1];
		novoInode=getBlocoLivre(disco,topo);
		disco[pos].indireto.indiretos[QTD_DIRETOS-1]=novoInode;
		disco[novoInode].inode.diretos[0]=aux;
		apontarBlocos(disco[novoInode].inode,tamanho,disco,topo,1);
		posAtual=novoInode;
	}
return atual;
}