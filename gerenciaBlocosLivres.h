int iniciarBlocosLivres(TpBloco disco[],int tamanho){
	int i,topo;
	for(i=tamanho-1,topo=0;i>topo;i--){
		if(isFull(disco[topo].livres)){
			disco[topo].livres.prox=topo+1;
			topo++;
		}
		else
			push(disco[topo].livres,i);
	}
	return topo;
}


int getBlocoLivre(TpBloco disco[],int &topo){
	int bloco= pop(disco[topo].livres);
	if(isEmpty(disco[topo].livres))
		topo--;
	return bloco;
}