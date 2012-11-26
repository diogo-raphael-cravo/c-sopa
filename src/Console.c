#include "../include/DadosComuns.h"

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
	char* digitado;
	while(1){
		usleep(1000*1000*10);
//		digitado = tela_esperarLinhaUsuario(&global_tela);
		tela_escreverNaColuna(&global_tela, "Mensagem do CONSOLE.",5);
	}
}


