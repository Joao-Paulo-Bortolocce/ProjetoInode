void init(TpPilha &pilha){
	pilha.topo=pilha.prox=-1;
}

void push(TpPilha &pilha,int bloco){
	pilha.blocosLivres[++pilha.topo]=bloco;
}

int pop(TpPilha &pilha){
	return pilha.blocosLivres[pilha.topo--];
}

char isEmpty(TpPilha pilha){
	return pilha.topo==-1;
}

char isFull(TpPilha pilha){
	return pilha.topo==MAX_PILHA;
}
