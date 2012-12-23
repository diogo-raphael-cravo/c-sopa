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
	INSTRUCAO_ABSOLUTE_JUMP,
	INSTRUCAO_LOAD_REGISTER_FROM_MEMORY,
	INSTRUCAO_LOAD_REGISTER_FROM_CONSTANT,
	INSTRUCAO_WRITE_REGISTER_IN_MEMORY,
	INSTRUCAO_SUBTRACT_REGISTERS,
	INSTRUCAO_ADD_REGISTERS,
	INSTRUCAO_DECREMENT_REGISTER,
	INSTRUCAO_INCREMENT_REGISTER,
	INSTRUCAO_COMPARE_REGISTERS,
	INSTRUCAO_JUMP_ON_ZERO,
	INSTRUCAO_JUMP_ON_EQUAL,
	INSTRUCAO_JUMP_ON_LESS,
	INSTRUCAO_SOFTWARE_INTERRUPT,
	INSTRUCAO_NOP
};

typedef enum enum_instrucao INSTRUCAO;

struct str_processador{
	REGISTRADOR IR;
	CONTEXTO contextoProcessador;
	int Z; //Registrador de estado de comparação: zero.
	int E; //Registrador de estado de comparação: igual.
	int L; //Registrador de estado de comparação: menor.
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

/**
* @param PROCESSADOR		*processador_param	O processador cujos dados serão impressos.
* @param int				coluna_param	A coluna da tela em que a impressão será feita.
*/
void processador_imprimir(PROCESSADOR *processador_param, int coluna_param);






