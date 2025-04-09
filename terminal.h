void terminal (){
	int inode;
	char usuario[7] = NOME_USUARIO;
	char maquina[17] = "@nomedamaquina:~";
	char caminho[NOME_ABSOLUTO]="\0";
	char comando[300];
	char firstComand[20];
	strcat(caminho,usuario);
	strcat(caminho,maquina);
	strcat(caminho,"$\0");
	do{
		printf("%s ",caminho);
		fflush(stdin);
		gets(comando);
		pegaComando(comando,firstComand);
		if(strcmp(firstComand,"mkdir")==0){
			mkdir();
		}
		else
		{
			if(strcmp(comando,"")==0){
			
			}
			else
			{
				if(strcmp(comando,"")==0){
			
				}
				else
				{
					if(strcmp(comando,"")==0){
						
					}
				}
			}
		}

		printf("\n");
		
	}while(strcmp(comando,"shutdown")!=0);
}

void pegaComando (char comando[], char res[]) {
    for(i=0; i<strlen(comando) && comando[i]!= ' '; i++) {
        res[i] = comando[i];
    }
}
