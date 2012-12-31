/**
* Concentra as funções de manipulação de processos.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes.
#define REGISTRADOR_STACK_POINTER 15

struct str_descritorProcesso{
	int PID; //Process ID.
	int enderecoInicio; //Início do programa na memória.
	int tamanhoAreaCodigoPalavras; //Tamanho da área de código reservada ao programa, em palavras.
	int haQuantosTicksRoda; //Há quantos ticks do relógio este processo está rodando, caso esteja.
	int tamanhoStack; //Tamanho da stack deste processo em palavras.
	CONTEXTO contextoProcesso;
};

typedef struct str_descritorProcesso DESCRITOR_PROCESSO;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo que será inicializado.
* @param int				PID_param					A PID deste descritor de processo.
* @param int				enderecoInicio_param		Endereço, na memória, do início do processo.
* @param int				tamanhoAreaCodigo_param		Tamanho da área de código do processo, em palavras.
* @param int				tamanhoStack_param			O tamanho da stack do processo, em palavras.
*/
void descritorProcesso_inicializar(DESCRITOR_PROCESSO *descritorProcesso_param, int PID_param, int enderecoInicio_param, 
	int tamanhoAreaCodigo_param, int tamanhoStack_param);

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo no qual a informação será buscada.
* @return int	PID do processo.
*/
int descritorProcesso_getPID(DESCRITOR_PROCESSO *descritorProcesso_param);

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo no qual a informação será salva.
* @param CONTEXTO			*contexto_param				O contexto que o processo terá.
*/
void descritorProcesso_setContexto(DESCRITOR_PROCESSO *descritorProcesso_param, CONTEXTO *contexto_param);

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo do qual a informação será recuperada.
* @return CONTEXTO*	O contexto do processo.
*/
CONTEXTO* descritorProcesso_getContexto(DESCRITOR_PROCESSO *descritorProcesso_param);

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo do qual a informação será recuperada.
* @return int	O endereço de início do processo na memória.
*/
int descritorProcesso_getEnderecoInicio(DESCRITOR_PROCESSO *descritorProcesso_param);

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo do qual a informação será recuperada.
* @return int	O tamanho da área de código reservada ao processo, em palavras.
*/
int descritorProcesso_getTamanhoAreaCodigoPalavras(DESCRITOR_PROCESSO *descritorProcesso_param);

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo do qual a informação será recuperada.
* @return int	O endereço de início do código deste processo.
*/
int descritorProcesso_getEnderecoInicioCodigo(DESCRITOR_PROCESSO *descritorProcesso_param);

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo do qual a informação será modificada.
* @param int				ticks_param					A quantidade de ticks há que o processo está rodando.
*/
void descritorProcesso_setFatiaTempo(DESCRITOR_PROCESSO *descritorProcesso_param, int ticks_param);

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo do qual a informação será modificada.
* @return int	A quantidade de ticks há que o processo está rodando.
*/
int descritorProcesso_getFatiaTempo(DESCRITOR_PROCESSO *descritorProcesso_param);

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo do qual a informação será modificada.
* @return int	O tamanho da stack do processo, em palavras.
*/
int descritorProcesso_getTamanhoStackPalavras(DESCRITOR_PROCESSO *descritorProcesso_param);

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo cuja stack será inicializada.
* ATENÇÃO: de uso estrito do kernel!
*/
void descritorProcesso_inicializarStack(DESCRITOR_PROCESSO *descritorProcesso_param);








