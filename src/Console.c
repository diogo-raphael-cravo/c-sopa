#include "../include/Console.h"
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



}

/**
* Espera que o usuário digite uma linha até [ENTER].
* @param CONSOLE	*console_param O console que irá rodar.
*/
char* console_esperarLinhaUsuario(CONSOLE *console_param){
/*	privada_moverCursorParaCaractere(tela_param, LINHA_CARACTERE_INPUT_USUARIO,COLUNA_CARACTERE_INPUT_USUARIO);
	char* digitado = (char*)malloc(TELA_COMPRIMENTO*sizeof(char));
	scanw("%s",digitado);
	privada_moverCursorParaCaractere(tela_param, LINHA_CARACTERE_INPUT_USUARIO,COLUNA_CARACTERE_INPUT_USUARIO);
	clrtoeol();
	return digitado;      //Fica esperando que o usuário aperte alguma tecla.
*/	return "a";
}