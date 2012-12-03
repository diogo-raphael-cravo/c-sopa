#include "../include/DadosComuns.h"
#include <curses.h>

/**
* Variáveis globais acessíveis somente neste arquivo.
*/



//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/**
* Inicia thread do console.
* @param CONSOLE	*console_param O console que irá rodar.
*/
void console_rodar(CONSOLE *console_param){
	char* digitado = (char*)malloc(100*sizeof(char));
	sem_wait(&global_mutexParaEscritaConsole);
	while(1){
		digitado = tela_esperarLinhaUsuario(&global_tela);
		if(digitado[0] != '\0'){
			tela_escreverNaColuna(&global_tela, digitado,5);
		} else {
			sem_post(&global_mutexParaEscritaTela);
			sem_wait(&global_mutexParaEscritaConsole);
		}
	}
}


