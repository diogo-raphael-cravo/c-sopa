/**
* Concentra as funções de manipulação de processos.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes.
#define QUANTIDADE_REGISTRADORES_CONTEXTO 16

struct str_contexto{
	int PC; //Program counter.
	REGISTRADOR registradores[QUANTIDADE_REGISTRADORES_CONTEXTO];
};

typedef struct str_contexto CONTEXTO;


//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* Inicializa o contexto.
* @param CONTEXTO	*contexto_param	O contexto que será inicializado.
*/
void contexto_inicializar(CONTEXTO *contexto_param);

/**
* @param CONTEXTO	*destino_param	O contexto para onde o segundo será copiado.
* @param CONTEXTO	*origem_param	O contexto do qual a cópia será feita.
*/
void contexto_copiar(CONTEXTO *destino_param, CONTEXTO *origem_param);

/**
* @param CONTEXTO	*contexto_param		O contexto no qual a informação será buscada.
* @return int	PC do contexto.
*/
int contexto_getPC(CONTEXTO *contexto_param);

/**
* @param CONTEXTO	*contexto_param			O contexto no qual a informação será buscada.
* @param int		indiceRegistrador_param	O índice em contexto_param->registradores do registrador que se quer.
* @return REGISTRADOR*	O registrador do contexto.
*/
REGISTRADOR* contexto_getRegistrador(CONTEXTO *contexto_param, int indiceRegistrador_param);

/**
* @param CONTEXTO			*contexto_param	O contexto no qual a operação será realizada.
* @param int				PC_param		O PC que o contexto deverá ter.
*/
void contexto_setPC(CONTEXTO *contexto_param, int PC_param);

/**
* @param CONTEXTO		*contexto_param			O contexto no qual a operação será realizada.
* @param REGISTRADOR	*registrador_param		Registrador que contém o valor que conterá o registrador 
*												do contexto ao fim da operação.
* @param int			indiceRegistrador_param	O índice em contexto_param->registradores do registrador que se quer.
*/
void contexto_setRegistrador(CONTEXTO *contexto_param, REGISTRADOR *registrador_param, int indiceRegistrador_param);

/**
* @param CONTEXTO		*contexto_param			O contexto no qual a operação será realizada.
* @param PALAVRA		*palavra_param			Palavra que contém o valor que conterá o registrador 
*												do contexto ao fim da operação.
* @param int			indiceRegistrador_param	O índice em contexto_param->registradores do registrador que se quer.
*/
void contexto_setRegistradorPalavra(CONTEXTO *contexto_param, PALAVRA palavra_param, int indiceRegistrador_param);

/**
* Imprime os registradores deste contexto na coluna fornecida da tela.
* @param CONTEXTO	*contexto_param	O contexto cujos registradores serão impressos.
* @param int		coluna_param	A coluna da tela em que a impressão será feita.
*/
void contexto_imprimirRegistradores(CONTEXTO *contexto_param, int coluna_param);

/**
* @param CONTEXTO	*contexto_param					O contexto que será lido.
* @param int		numeroRegistradorInicio_param	Número do registrador que inicia a string.
* @param int		numeroRegistradorFim_param		Número limite para o fim da string.
* @return char*	A string lida dos registradores.
* ATENÇÃO: sua memória alocada corresponderá ao total de bytes fornecidos pelos registradores no intervalo dado.
* ATENÇÃO: a string pode não ocupar todos registradores.
* ATENÇÃO: a string (nos registradores) deve ser terminada por CARACTERE_TERMINADOR_STRING_SOPA.
*/
char* contexto_lerStringDosRegistradores(CONTEXTO *contexto_param, int numeroRegistradorInicio_param, int numeroRegistradorFim_param);




