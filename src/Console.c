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
* @param CONSOLE	*console_param O console que será inicializado.
*/
void console_inicializar(CONSOLE *console_param){
	console_param->ultimaLinhaDigitada = NULL;
}

/**
* Inicia thread do console.
* @param CONSOLE	*console_param O console que irá rodar.
*/
void console_rodar(CONSOLE *console_param){
	console_param = (CONSOLE*) console_param;
	char* digitado = (char*)malloc(100*sizeof(char));
	int usuarioDesejaSairDoConsole = 0;
	sem_wait(&global_mutexParaEscritaConsole);
	while(1){
		digitado = tela_esperarLinhaUsuario(&global_tela);
		usuarioDesejaSairDoConsole = (digitado[0] == '\0');
		if(!usuarioDesejaSairDoConsole){
			console_param->ultimaLinhaDigitada = (char*) malloc(strlen(digitado)*sizeof(char));
			console_param->ultimaLinhaDigitada = digitado;
			controladorInterrupcoes_set(&global_controladorInterrupcoes, INTERRUPCAO_CONSOLE);
		} else {
			sem_post(&global_mutexParaEscritaTela);
			sem_wait(&global_mutexParaEscritaConsole);
		}
	}
}

/**
* @param CONSOLE	*console_param O console em que a linha foi digitada.
* @return char*	A última linha que o usuário digitou neste console. Caso nada tenha sido digitado, retornará NULL.
*/
char* console_ultimaLinhaDigitada(CONSOLE *console_param){
	return console_param->ultimaLinhaDigitada;
}
