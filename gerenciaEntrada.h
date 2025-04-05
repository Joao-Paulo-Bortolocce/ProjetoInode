void criarEntrada(int pai,TpBloco disco[],int &topo,int posInode,char nome[]){
	while(disco[pai].dir.tl==NUMERO_ENTRADAS){
		if(disco[pai].dir.prox<0){
			disco[pai].dir.prox=getBlocoLivre(disco,topo);
			disco[disco[pai].dir.prox].dir.tl=0;
		}
		pai=disco[pai].dir.prox;
		
	}
	disco[pai].dir.entradas[disco[pai].dir.tl].inode=posInode;
	strcpy(disco[pai].dir.entradas[disco[pai].dir.tl].nome,nome);
	disco[pai].dir.tl++;
}
