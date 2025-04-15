#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXCOMMAND 128
#define NUM_PREFIXOS 21

#include "./database_structure/db.h"

#include "./database_structure/fila.h"
#include "./database_structure/filaStatus.h"

#include "./commands/commands.h"
#include "./commands/settodefault.h"
#include "./commands/createdbf.h"
#include "./commands/dir.h"
#include "./commands/list.h"
#include "./commands/use.h"
#include "./commands/append.h"
#include "./commands/locate.h"
#include "./commands/goto.h"
#include "./commands/pack.h"
#include "./commands/recall.h"
#include "./commands/setdelete.h"
#include "./commands/zap.h"
#include "./commands/clear_delete.h"
#include "./commands/visualDbase.h"
#include "./commands/edit.h"
#include "./commands/display.h"
#ifdef _WIN32
    #include <windows.h>
    #include <conio2.h>
#else
    #include <unistd.h> // sleep on MacOS
#endif


int main(void){
	
	char dire[3] ="?", auxdire[4] = "?",c; // exibir o DIRETORIO 	 NA INTERFACE
	char nome[25] = "          ";
    dBase *dbase = NULL, *dbAtual=NULL;
    dUnidade *unidadeAtual=NULL;
    int viewDelete=0, confirmeUse;
	Command command;
	char ent[MAXCOMMAND];
    ent[strcspn(ent, "\n")] = 0;
    to_upper_str(ent);
    
    dataseeder(&dbase);
	tela1();
	clear();
	while(strcmp("QUIT", ent)!=0){
		commandlinePosUse(dire, nome); 
		fflush(stdin);
        fgets(ent, sizeof(ent), stdin);
        ent[strcspn(ent, "\n")] = 0;
        to_upper_str(ent);
        
		command=processarComando(ent);
		clear();
		gotoxy(50,18);
        switch(command.type){
            case 0: setDefault(&dbase, command.value, &dbAtual, &unidadeAtual);
		            strcpy(dire, command.value);
		        	strncpy(dire, command.value, sizeof(dire) - 1);
		    		dire[sizeof(dire) - 1] = '\0';
		        	commandline(dire);
					break; 
            case 1: commandCreate(dire, command.value);
					createdbf(dbAtual, command.value); break;
            case 2: commandDir(dire);
					dir(dbAtual); 
					clear();break;
            case 3: /*QUIT*/ break; 
            case 4: strcpy(nome,command.value);
            		strcpy(auxdire, dire);
    				commandlinePosUse(dire, nome); 
					confirmeUse = use(dbAtual, &unidadeAtual,command.value); 
					if(confirmeUse == 0){
						strcpy(nome,"         ");
	    				commandlinePosUse(dire, nome); 
					}
					limparescrita();
					break;
            case 5: liststructure(dbAtual, unidadeAtual);break;
            case 6: commandlineAppend(dire,nome);append(&unidadeAtual);break;
            case 7: listfor(command.value, unidadeAtual, viewDelete);break;
            case 8: list(unidadeAtual, viewDelete); limparescrita();break;
            case 9: clear(); break; 
            
            case 10: commandlineLocate(dire,nome);locate(command.value, unidadeAtual, viewDelete); break;
            case 11: gotoo(unidadeAtual, command.value); break;
            case 12: commandlineDisplay(dire,nome);display(unidadeAtual);break; 
            case 13: commandlineEdit(dire,nome);edit(unidadeAtual);break;
                
            case 14: commandlineDeleteAll(dire,nome);delete_all(unidadeAtual); break;
            case 15: commandlineDelete(dire,nome);deletee(unidadeAtual); break;
            case 16:commandlineRecallAll(dire,nome); 
					recall_all(unidadeAtual); break;
            case 17: setdelete(&viewDelete, command.value); 
            		if(viewDelete){
            			limparmsg();
						gotoxy(26,24);
						printf("  viewDelete On, Press [SPACE]");
						commandlineSetDeleteOn(dire,nome);
				    	do {
					    	gotoxy(1,21);
					        c = getch();  
				    	}while(c != 32);
            		}else{
            			limparmsg();
						gotoxy(27,24);
						printf("  viewDelete Off, Press [SPACE]");
						commandlineSetDeleteOff(dire,nome);
				    	do {
					    	gotoxy(1,21);
					        c = getch();  
				    	}while(c != 32);
            		}
			break; 
            case 18: commandlinePack(dire,nome);pack(unidadeAtual); break;
            case 19: commandlineZap(dire,nome);zap(unidadeAtual); break;
            case 20: commandlineRecall(dire,nome); 
					 recall(unidadeAtual); break;
            default: commandinvalido(dire);break; 
        }
        
        

	}
	
	return 0;
}
