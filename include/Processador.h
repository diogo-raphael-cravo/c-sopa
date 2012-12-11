/**
* Concentra as funções de manipulação do processador.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define TAMANHO_INSTRUCAO_PALAVRAS TAMANHO_REGISTRADOR_BYTES

enum enum_instrucao{
	INSTRUCAO_INEXISTENTE,
	INSTRUCAO_JPA,
	INSTRUCAO_INT
};

typedef enum enum_instrucao INSTRUCAO;

struct str_processador{
	int PC;
	REGISTRADOR IR;
	REGISTRADOR registrador;
};

typedef struct str_processador PROCESSADOR;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* @param PROCESSADOR	*processador_param	O processador que será inicializado.
*/
void processador_inicializar(PROCESSADOR *processador_param);

/**
* Inicia thread do processador.
* @param PROCESSADOR	*processador_param O processador que irá rodar.
*/
void processador_rodar(PROCESSADOR *processador_param);

/**
* @param PROCESSADOR	*processador_param	O processador no qual a informação será buscada.
* @return int	PC do processador.
*/
int processador_getPC(PROCESSADOR *processador_param);

/**
* @param PROCESSADOR	*processador_param	O processador no qual a informação será buscada.
* @return REGISTRADOR*	O registrador do processador.
*/
REGISTRADOR* processador_getRegistrador(PROCESSADOR *processador_param);

/**
* @param PROCESSADOR	*processador_param	O processador no qual a operação será realizada.
* @param int				PC_param		O PC que o processador deverá ter.
*/
void processador_setPC(PROCESSADOR *processador_param, int PC_param);

/**
* @param PROCESSADOR	*processador_param		O processador no qual a operação será realizada.
* @param REGISTRADOR	*registrador_param		Registrador que contém o valor que conterá o registrador 
*												do processador ao fim da operação.
*/
void processador_setRegistrador(PROCESSADOR *processador_param, REGISTRADOR *registrador_param);


