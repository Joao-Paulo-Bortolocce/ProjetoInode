//void relatorio4 (TpBloco disco[],int raiz){
//	int aux[tamanhoDisc],i;
//	for(i=0;i<tamanhoDisc;i++){
//		aux[i]=0;
//	} 
//	blocosPerdidos(disco,aux,raiz);
//	//exibe blocos perdidos
//	
//	int total=0,pos=0;
//	gotoxy(0,0);
//	printf("[");
//	for(int i=0;i<tamanhoDisc;i++){
//		if(aux[i]==0){
//			total++;
//			pos++;
//			if(pos%20==0)
//				printf("\n");
//			printf(" %d ", i);
//			if(i<tamanhoDisc-1){
//				printf("|");
//			}
//		}
//	}
//	printf("]");
//	gotoxy((pos%20)+1,0);
//	printf("Total : %d" total);
//	getch(); 
//}

//void blocosPerdidos (TpBloco disco[], int aux [], int pai){
//	int tam = disco[pai].dir.tl,i;
//	int tamBloc,k=0;
//	for(i=0;i<tam;i++){
//		if(disco[disco[pai].dir.entradas[i].inode].inode.header.permissao[0]=='d'){ //se for diretório eu chamo recursivamente pra fazer com esse diretório novo
//			blocosPerdidos(disco,aux,disco[disco[pai].dir.entradas[i].inode].inode.diretos[0]);
//		}
//		//após fazer com o diretório eu vou pegar todos as posições diretas, ind, dup e triplo do inode de todas as entradas desse diretório
//		tamBloc = disco[disco[pai].dir.entradas[i].inode].inode.tamanho; // pego tamanho de blocos que o inode daquela entrada ocupa

//		while(k<QTD_BLOCOS && j<tamBloc){//assinalo os diretos no aux como usado
//			aux[disco[disco[pai].dir.entradas[i].inode].inode.diretos[k++]]=1;
//			j++;
//		}
//	
//		if(j<tamBloc){//verifico se nao chegou, para evitar acessar valores nulos como o TL de indiretos sendo que o inode usa apenas os diretos
//			k=0;
//			tamInd = disco[disco[disco[pai].dir.entradas[i].inode].inode.indireto].tl;
//			while(k< tamInd+QTD_BLOCOS && j<tamBloc){//rodo o vetor de indiretos
//				aux[disco[disco[disco[pai].dir.entradas[i].inode].inode.indireto].indiretos[k++]] = 1;
//				j++;
//			}
//		}
//		
//		int tamIndDup=0;
//		
//		if(j<tamBloc){//novamente verifico se nao chegou pra nao acessar nulos
//			for(int y =0; y<QTD_BLOCOS && j<tamBloc; y++){
//				k=0;
//				tamIndDup+=disco[disco[disco[disco[pai].dir.entradas[i].inode].inode.duplo].indiretos[y].tl;//vou somando o tamanho dos indiretos dos duplos para saber quando parar
//				while(k<tamIndDup+tamInd+QTD_BLOCOS && j<tamBloc){//até o final do indireto na posicao do indireto duplo
//					aux[disco[disco[disco[disco[pai].dir.entradas[i].inode].inode.duplo].indiretos[y].indiretos[k++]] = 1;
//					j++;
//				}
//			}
//		}
//		
//		int tamIndDupT=tamIndDup;
//		tamIndDup=0;
//		if(j<tamBloc){//verifico se nao pego nulo
//			for(int z=0;z<QTD_BLOCOS && j<tamBloc;z++){//para cada duplo de um triplo
//				for(int y =0; y<QTD_BLOCOS && j<tamBloc; y++){//para cada indireto de um duplo
//					tamIndDup+=disco[disco[disco[disco[pai].dir.entradas[i].inode].inode.triplo].indiretos[z].indiretos[y].tl;
//					k=0;
//					while(k<tamIndDupT+tamIndDup+tamInd+QTD_BLOCOS && j<tamBloc){//para cara bloco do indireto
//						aux[disco[disco[disco[disco[pai].dir.entradas[i].inode].inode.triplo].indiretos[z].indiretos[y].indiretos[k++]] = 1;
//						j++;
//					}
//				}
//			}
//		}
		
//	}
//}



void relatorio6 (TpBloco disco[],int diretorio){
	int tam = disco[diretorio].dir.tl,i;
	for(i=0;i<tam;i++){
		 switch(disco[disco[diretorio].dir.entradas[i].inode].inode.header.permissao[0]) {
		            case 'd': // Diretório
		                textcolor(LIGHTBLUE);
		                printf("%-20s", disco[diretorio].dir.entradas[i].nome); // Nome com largura fixa para alinhamento
		                textcolor(WHITE);
		                cprintf(" <DIRETÓRIO>\n");
		                break;
		
		            case '-': // Arquivo
		                textcolor(WHITE);
		                printf("%-20s", disco[diretorio].dir.entradas[i].nome);
		                textcolor(LIGHTGRAY);
		                cprintf(" <ARQUIVO>\n");
		                break;
		
		            case 'l': // Link
		                textcolor(LIGHTGREEN);
		                printf("%-20s", disco[diretorio].dir.entradas[i].nome);
		                textcolor(WHITE);
		                cprintf(" <LINK>\n");
		                break;
		 }
	}	    
	textcolor(WHITE); // Resetando a cor
}
