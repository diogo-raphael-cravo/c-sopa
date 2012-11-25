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

};

typedef struct str_console CONSOLE;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* Inicia thread do console.
* @param CONSOLE	*console_param O console que irá rodar.
*/
void console_rodar(CONSOLE *console_param);

/**
* Espera que o usuário digite uma linha até [ENTER].
* @param CONSOLE	*console_param O console que irá rodar.
*/
char* console_esperarLinhaUsuario(CONSOLE *console_param);
