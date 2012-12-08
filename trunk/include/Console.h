/**
* Concentra as funções de manipulação do console.
* ====================================
*
* 			ATENÇÃO: Não confundir o console com a tela!
*
* ====================================
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

struct str_console{
	char* ultimaLinhaDigitada;
};

typedef struct str_console CONSOLE;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* @param CONSOLE	*console_param O console que será inicializado.
*/
void console_inicializar(CONSOLE *console_param);

/**
* Inicia thread do console.
* @param CONSOLE	*console_param O console que irá rodar.
*/
void console_rodar(CONSOLE *console_param);

/**
* @param CONSOLE	*console_param O console em que a linha foi digitada.
* @return char*	A última linha que o usuário digitou neste console. Caso nada tenha sido digitado, retornará NULL.
*/
char* console_ultimaLinhaDigitada(CONSOLE *console_param);
