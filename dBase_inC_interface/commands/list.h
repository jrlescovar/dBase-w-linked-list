char findField(char *field, Campos **campos);

void liststructure(dBase *dbAtual, dUnidade *unid);
void list(dUnidade *unid, char viewDelete);
void listfor(char *parm, dUnidade *unidAt, char viewDelete);

void liststructure(dBase *dbAtual, dUnidade *unid){
    int dataRecords=0, widthsum=0;
    Campos *aux = unid->Campos;
    Pdados *auxp;
  /*  
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif*/
    
    gotoxy(1,1);
    printf(". List Structure\n");
    gotoxy(1,2);
    printf("Structure for database: %s%s\n",dbAtual->disco, unid->NomeDBF);
    
    while (aux != NULL) {
        aux->Patual = aux->Pdados;
        aux = aux->prox;
    }
    
    aux = unid->Campos;
    while(aux!=NULL){
        auxp=aux->Pdados;
        while(auxp!=NULL){
            dataRecords++;
            auxp=auxp->prox;
        }
        aux=aux->prox;
    }
    gotoxy(1,3);
    printf("Number of data records: %d\n", dataRecords);
    gotoxy(1,4);
    printf("Date of last update: %s\n", unid->Data);
    gotoxy(1,5);
    printf("Field\tFieldName\tType\t\tWidth\tDec\n");
    aux=unid->Campos; dataRecords=1;
    int y = 6;
    while(aux!=NULL){
    	gotoxy(1,y);
        printf("%d\t%s\t\t", dataRecords, aux->FieldName);
        switch(aux->Type){
        	case 'N': printf("Numeric\t");  break;
        	case 'D': printf("Date\t"); break;
        	case 'L': printf("Logical\t"); break;
        	case 'C': printf("Character\t"); break;
        	case 'M': printf("Memo\t"); break;
        }
        printf("%d\t%d\n", aux->Width, aux->Dec);
    	y++;
        widthsum+=aux->Width;
        dataRecords++;
        aux=aux->prox;
    }
    gotoxy(1,y);
    printf("** TOTAL **\t\t\t%d", widthsum);
}

void list(dUnidade *unid, char viewDelete) {
	char c;
	int x,y;
    if (unid == NULL || unid->Campos == NULL) {
        limparmsg();
		gotoxy(15,24);
		printf("Db is not defined or fields, Press [SPACE]");
    	do {
	    	gotoxy(1,21);
	        c = getch();  
    	}while(c != 32);
    }
    else{	
	    Campos *auxCampos = unid->Campos;
	
	    // Imprime os nomes das colunas (cabe�alho)
	    gotoxy(1,2);
	    printf("Record# ");
	    int v = 15;
	    while (auxCampos != NULL) {
	    	gotoxy(v,2);
	        printf("%s", auxCampos->FieldName);
	        //v+=strlen(auxCampos->FieldName);
	        v = v + 12;
	        auxCampos = auxCampos->prox;
	        
	    }
	    printf("\n");
	
	    // Verifica se h� dados
	    if (unid->Status == NULL) {
	        limparmsg();
			gotoxy(22,24);
			printf("No data available, Press [SPACE]");
	    	do {
		    	gotoxy(1,21);
		        c = getch();  
	    	}while(c != 32);
	    }
	    else{
		    // Inicializa os ponteiros atuais de cada campo para o in�cio dos dados
		    Campos *campo = unid->Campos;
		    while (campo != NULL) {
		        campo->Patual = campo->Pdados;
		        campo = campo->prox;
		    }
		
		    Status *statusAtual = unid->Status;
		    gotoxy(1,1);
			printf(" LIST");
		    // Percorre cada registro (cada entrada na lista de Status)
			y=3;
		    while (statusAtual != NULL) {
		    	x=16;
		        // Decide se o registro deve ser exibido
		        if (viewDelete || statusAtual->Status == 'A') {
		            Campos *campoAtual = unid->Campos;
		            // Percorre todos os campos para imprimir os valores do registro atual
		            while (campoAtual != NULL) {
		                if (campoAtual->Patual != NULL) {
		                	gotoxy(x,y);
		                    switch (campoAtual->Type) {
		                        case 'N': printf("%.2f\t", campoAtual->Patual->Valor.N); break;
		                        case 'D': printf("%s\t", campoAtual->Patual->Valor.D); break;
		                        case 'L': printf("%d\t", campoAtual->Patual->Valor.L); break;
		                        case 'C': printf("%s\t", campoAtual->Patual->Valor.C); break;
		                        case 'M': printf("%s\t", campoAtual->Patual->Valor.M); break;
		                        default: printf("?\t"); break;
		                    }
		                    campoAtual->Patual = campoAtual->Patual->prox;
		                } else {
		                    printf("\t"); // Campo sem dado
		                }
		                x = x + 15;
		                campoAtual = campoAtual->prox;
		            }
		            y++;
		        } else {
		            // Registro exclu�do, avan�a os ponteiros dos campos sem imprimir
		            Campos *campoAtual = unid->Campos;
		            while (campoAtual != NULL) {
		                if (campoAtual->Patual != NULL) {
		                    campoAtual->Patual = campoAtual->Patual->prox;
		                }
		                campoAtual = campoAtual->prox;
		            }
		        }
		        statusAtual = statusAtual->prox;
		    }
		
		    // Resetar os ponteiros Patual para o in�cio dos dados em cada campo
		    campo = unid->Campos;
		    while (campo != NULL) {
		        campo->Patual = campo->Pdados;
		        campo = campo->prox;
		    }	
		}
    }
}

void listfor(char *parm, dUnidade *unidAt, char viewDelete) {
    Campos *campos;
    Status *statusAtual;
    char filterField[MAXNAME], filter[MAXNAME],c;
    int i = 0, j = 0;
    int x, y;
    // Parse do par�metro
    while (parm[i] != '=' && parm[i] != '\0') i++;
    
    if (parm[i] == '=') {
        // Extrair nome do campo
        while (j < i && parm[j] == ' ') j++;
        strncpy(filterField, parm + j, i - j);
        filterField[i - j] = '\0';
        
        // Extrair valor do filtro
        i++;
        while (parm[i] == ' ') i++;
        strcpy(filter, parm + i);

        Campos *campoFiltro = unidAt->Campos;
        if (findField(filterField, &campoFiltro)) {
            // Imprimir cabe�alhos
            Campos *auxCampos = unidAt->Campos;
            x=15;
            gotoxy(1,2);
	    	printf("Record# ");
            while (auxCampos) {
            	gotoxy(1,x);
                printf("%s\t", auxCampos->FieldName);
                auxCampos = auxCampos->prox;
                x = x+12;
            }
            printf("\n");

            // Inicializar ponteiros
            statusAtual = unidAt->Status;
            auxCampos = unidAt->Campos;
            while (auxCampos) {
                auxCampos->Patual = auxCampos->Pdados;
                auxCampos = auxCampos->prox;
            }

            // Percorrer registros
            while (statusAtual) {
                int mostrar = (viewDelete || statusAtual->Status == 'A');
                int filtroOK = 0;

                // Verificar filtro
				if (campoFiltro->Patual) {
				    switch (campoFiltro->Type) {
				        case 'N':
				            filtroOK = (campoFiltro->Patual->Valor.N == atof(filter));
				            break;
				            
				        case 'D': {
				            char valCampo[MAXNAME], valFiltro[MAXNAME];
				            strcpy(valCampo, campoFiltro->Patual->Valor.D);
				            strcpy(valFiltro, filter);
				            to_upper_str(valCampo);
				            to_upper_str(valFiltro);
				            filtroOK = (strcmp(valCampo, valFiltro) == 0);
				            break;
				        }
				            
				        case 'C': {
				            char valCampo[MAXNAME], valFiltro[MAXNAME];
				            strcpy(valCampo, campoFiltro->Patual->Valor.C);
				            strcpy(valFiltro, filter);
				            to_upper_str(valCampo);
				            to_upper_str(valFiltro);
				            filtroOK = (strcmp(valCampo, valFiltro) == 0);
				            break;
				        }
				            
				        case 'M': {
				            char valCampo[MAXNAME], valFiltro[MAXNAME];
				            strcpy(valCampo, campoFiltro->Patual->Valor.M);
				            strcpy(valFiltro, filter);
				            to_upper_str(valCampo);
				            to_upper_str(valFiltro);
				            filtroOK = (strcmp(valCampo, valFiltro) == 0);
				            break;
				        }
				            
				        case 'L':
				            filtroOK = (campoFiltro->Patual->Valor.L == atoi(filter));
				            break;
				            
				        default:
				            filtroOK = 0;
				            break;
				    }
				}

                // Imprimir registro se atender aos crit�rios
                if (mostrar && filtroOK) {
                    Campos *campoPrint = unidAt->Campos;
                    y=3;x=16;
                    while (campoPrint) {
                        if (campoPrint->Patual) {
                        	gotoxy(x,y);
                            switch (campoPrint->Type) {
                                case 'N': printf("%.2f\t", campoPrint->Patual->Valor.N); break;
                                case 'D': printf("%s\t", campoPrint->Patual->Valor.D); break;
                                case 'L': printf("%d\t", campoPrint->Patual->Valor.L); break;
                                case 'C': printf("%s\t", campoPrint->Patual->Valor.C); break;
                                case 'M': printf("%s\t", campoPrint->Patual->Valor.M); break;
                                default: printf("?\t");
                            }
                        } else {
                            x = x + 15;
                        }
                        campoPrint = campoPrint->prox;
                    }
                    y++;
                }

                // Avan�ar para pr�ximo registro
                Campos *campoAvanco = unidAt->Campos;
                while (campoAvanco) {
                    if (campoAvanco->Patual) {
                        campoAvanco->Patual = campoAvanco->Patual->prox;
                    }
                    campoAvanco = campoAvanco->prox;
                }
                statusAtual = statusAtual->prox;
            }

            // Resetar ponteiros
            auxCampos = unidAt->Campos;
            while (auxCampos) {
                auxCampos->Patual = auxCampos->Pdados;
                auxCampos = auxCampos->prox;
            }
        } else {
            limparmsg();
			gotoxy(12,24);
		    printf("field  %s not found!, Press [SPACE]",filterField);
	    	do{
		    	gotoxy(1,21);
		        c = getch();  
	    	}while(c != 32);
        }
    } else {
        limparmsg();
		gotoxy(10,24);
	    printf("Invalid format! Use: FIELD=VALUE, Press [SPACE]");
    	do{
	    	gotoxy(1,21);
	        c = getch();  
    	}while(c != 32);
    }
}


char findField(char *field, Campos **campos) {
    Campos *auxCampos = *campos;
    char fieldName[MAXNAME];

    while (auxCampos != NULL) {
        strcpy(fieldName, auxCampos->FieldName);
        to_upper_str(fieldName); // Normaliza para maiusculas
        
        if (strcmp(fieldName, field) == 0) {
            *campos = auxCampos;
            return 1; // Campo encontrado
        }
        
        auxCampos = auxCampos->prox;
    }
    
    return 0; // Campo nao encontrado
}

