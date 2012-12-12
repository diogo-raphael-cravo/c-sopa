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
	REGISTRADOR IR;
	CONTEXTO contextoProcessador;
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
* @param PROCESSADOR		*processador_param	O processador cujo contexto será restaurado.
* @param CONTEXTO			*contexto_param		O contexto que o processador terá.
*/
void processador_setContexto(PROCESSADOR *processador_param, CONTEXTO *contexto_param);

/**
* @param PROCESSADOR		*processador_param	O processador cujo contexto será retornado.
* @return CONTEXTO*	O contexto do processador.
*/
CONTEXTO* processador_getContexto(PROCESSADOR *processador_param);






