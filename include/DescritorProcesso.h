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
	int PC; //Program counter.
	STATUS_DESCRITOR_PROCESSO status; //Indica se está executando, pronto ou bloqueado.
	REGISTRADOR registrador;
};

typedef struct str_descritorProcesso DESCRITOR_PROCESSO;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo que será inicializado.
* @param int				PID_param					A PID deste descritor de processo.
*/
void descritorProcesso_inicializar(DESCRITOR_PROCESSO *descritorProcesso_param, int PID_param);

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo no qual a informação será buscada.
* @return int	PID do processo.
*/
int descritorProcesso_getPID(DESCRITOR_PROCESSO *descritorProcesso_param);

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo no qual a informação será buscada.
* @return int	PC do processo.
*/
int descritorProcesso_getPC(DESCRITOR_PROCESSO *descritorProcesso_param);

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo no qual a informação será buscada.
* @return REGISTRADOR*	Cópia profunda do registrador do processo.
*/
REGISTRADOR* descritorProcesso_getRegistrador(DESCRITOR_PROCESSO *descritorProcesso_param);

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo no qual a operação será realizada.
* @param int				PC_param					O PC que o descritor de processo deverá ter.
*/
void descritorProcesso_setPC(DESCRITOR_PROCESSO *descritorProcesso_param, int PC_param);

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo no qual a operação será realizada.
* @param REGISTRADOR		*registrador_param			Registrador que contém o valor que conterá o registrador 
*														do descritor de processo ao fim da operação.
*/
void descritorProcesso_setRegistrador(DESCRITOR_PROCESSO *descritorProcesso_param, REGISTRADOR *registrador_param);

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








