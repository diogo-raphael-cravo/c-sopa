/**
* Concentra as funções de manipulação de processos.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes.

enum enum_statusDescritorProcesso{
	STATUS_PROCESSO_EXECUTANDO,
	STATUS_PROCESSO_PRONTO,
	STATUS_PROCESSO_BLOQUEADO
};

typedef enum enum_statusDescritorProcesso STATUS_DESCRITOR_PROCESSO;

struct str_descritorProcesso{
	int PID; //Process ID.
	int enderecoInicio; //Início do programa na memória.
	int tamanhoAreaMemoriaPalavras; //Tamanho da área de memória reservada ao programa, em palavras.
	int haQuantosTicksRoda; //Há quantos ticks do relógio este processo está rodando, caso esteja.
	STATUS_DESCRITOR_PROCESSO status; //Indica se está executando, pronto ou bloqueado.
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
* @param int				tamanhoAreaMemoria_param	Tamanho da área de memória do processo, em palavras.
*/
void descritorProcesso_inicializar(DESCRITOR_PROCESSO *descritorProcesso_param, int PID_param, int enderecoInicio_param, int tamanhoAreaMemoria_param);

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo no qual a informação será buscada.
* @return int	PID do processo.
*/
int descritorProcesso_getPID(DESCRITOR_PROCESSO *descritorProcesso_param);

/**
* @param DESCRITOR_PROCESSO			*descritorProcesso_param	O descritor de processo no qual a operação será realizada.
* @param STATUS_DESCRITOR_PROCESSO	status_param				O status deste processo. Deve ser algum dos definidos no início deste arquivo.
*/
void descritorProcesso_setStatus(DESCRITOR_PROCESSO *descritorProcesso_param, STATUS_DESCRITOR_PROCESSO status_param);

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo no qual a informação será buscada.
* @return STATUS_DESCRITOR_PROCESSO	O status deste processo. Deve ser algum dos definidos no início deste arquivo.
*/
STATUS_DESCRITOR_PROCESSO descritorProcesso_getStatus(DESCRITOR_PROCESSO *descritorProcesso_param);

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
* @return int	O tamanho da área de memória reservada ao processo, em palavras.
*/
int descritorProcesso_getTamanhoAreaMemoriaPalavras(DESCRITOR_PROCESSO *descritorProcesso_param);

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









