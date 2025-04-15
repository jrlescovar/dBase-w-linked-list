void append(dUnidade **unidade) {
	char c;
	int x=1, y=0;
    if (*unidade == NULL) {
        limparmsg();
		gotoxy(6,24);
	    printf("Define a DB to use | You can define with 'USE name.dbf'\n");
    	do{
	    	gotoxy(1,21);
	        c = getch();  
    	}while(c != 32);
    } else {
        Campos *auxCampos = (*unidade)->Campos;
        int exit = 0;
        Status *novoStatus = NULL;
        Pdados *dadosTemp[MAXFIELDNAME];
        int count = 0,i;

        novoStatus = (Status*)malloc(sizeof(Status));
        if (novoStatus == NULL) {
            limparmsg();
			gotoxy(34,24);
			printf("Memory error!, Press [SPACE]");
	    	do {
		    	gotoxy(1,21);
		        c = getch();  
	    	}while(c != 32);
            exit = 1;
        } 
		else {
            novoStatus->Status = 'A';
            novoStatus->prox = NULL;
            
            while (auxCampos != NULL && !exit) {
                Pdados *novoDado = (Pdados*)malloc(sizeof(Pdados));
                if (novoDado == NULL) {
                   	limparmsg();
					gotoxy(34,24);
					printf("Memory error!, Press [SPACE]");
			    	do {
				    	gotoxy(1,21);
				        c = getch();  
			    	}while(c != 32);
                    exit = 1;
                } 
				else {
                    novoDado->prox = NULL;
                    y++;
                    gotoxy(x,y);
                    printf("%s: ", auxCampos->FieldName);                    
                    switch (auxCampos->Type) {
                        case 'N':
                            if(scanf("%f", &novoDado->Valor.N) != 1){
                                limparmsg();
								gotoxy(32,24);
								printf("invalid value, Press [SPACE]");
						    	do {
							    	gotoxy(1,21);
							        c = getch();  
						    	}while(c != 32);
                                free(novoDado);
                                exit = 1;
                            }
                            break;
                        case 'D':
                        	gotoxy(x,y);
                            printf("(DD/MM/YYYY) ");
                            fflush(stdin);
                            fgets(novoDado->Valor.D, MAXDATA, stdin);
                            novoDado->Valor.D[strcspn(novoDado->Valor.D, "\n")] = '\0';
                            break;
                        case 'L':
                        	gotoxy(x,y);
                            if (scanf("%d", &novoDado->Valor.L) != 1) {
                                limparmsg();
								gotoxy(32,24);
								printf("invalid value, Press [SPACE]");
						    	do {
							    	gotoxy(1,21);
							        c = getch();  
						    	}while(c != 32);
                                free(novoDado);
                                exit = 1;
                            }
                            break;
                        case 'C':
                            fflush(stdin);
                            fgets(novoDado->Valor.C, MAXBYTES, stdin);
                            novoDado->Valor.C[strcspn(novoDado->Valor.C, "\n")] = '\0';
                            break;
                        case 'M':
                            fflush(stdin);
                            fgets(novoDado->Valor.M, MAXBYTES, stdin);
                            novoDado->Valor.M[strcspn(novoDado->Valor.M, "\n")] = '\0';
                            break;
                        default:
                            limparmsg();
							gotoxy(34,24);
							printf("Unknown type!, Press [SPACE]");
					    	do {
						    	gotoxy(1,21);
						        c = getch();  
					    	}while(c != 32);
                            free(novoDado);
                            exit = 1;
                    }

                    if (!exit) {
                        dadosTemp[count++] = novoDado;
                        auxCampos = auxCampos->prox;
                    } 
					else {
                        free(novoDado);
                    }
                }
            }

            if (!exit) {
                if ((*unidade)->Status == NULL) {
                    (*unidade)->Status = novoStatus;
                } 
				else {
                    Status *ultimoStatus = (*unidade)->Status;
                    while (ultimoStatus->prox != NULL) {
                        ultimoStatus = ultimoStatus->prox;
                    }
                    ultimoStatus->prox = novoStatus;
                }

                Campos *campoAtual = (*unidade)->Campos;
                for (i = 0; i < count; i++) {
                    if (campoAtual->Pdados == NULL) {
                        campoAtual->Pdados = dadosTemp[i];
                        campoAtual->Patual = dadosTemp[i];
                    } else {
                        Pdados *ultimoDado = campoAtual->Pdados;
                        while (ultimoDado->prox != NULL) {
                            ultimoDado = ultimoDado->prox;
                        }
                        ultimoDado->prox = dadosTemp[i];
                    }
                    campoAtual = campoAtual->prox;
                }
            } 
			else {
                free(novoStatus);
                for (i = 0; i < count; i++) {
                    free(dadosTemp[i]);
                }
            }
        }
    }
}
