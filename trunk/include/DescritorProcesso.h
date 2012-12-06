/**
* Concentra as funções de manipulação de processos.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------
struct str_descritorProcesso{
	int PID; //Process ID.
	int PC; //Program counter.
	REGISTRADOR *registrador;
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










