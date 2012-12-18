/**
* Concentra as funções de manipulação do controlador de interrupções.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes

enum enum_interrupcoes{
	SEM_INTERRUPCAO=0,
	INTERRUPCAO_TIMER=2,
	INTERRUPCAO_SEGMENTACAO_MEMORIA=3,
	INTERRUPCAO_DISCO=5,
	INTERRUPCAO_CONSOLE=15,
	INTERRUPCAO_SOFTWARE_PARA_DISCO=36,
	INTERRUPCAO_RPC
};

typedef enum enum_interrupcoes INTERRUPCAO;

struct str_controladorInterrupcoes{
	sem_t mutexAcesso; //Controla acesso a métodos sincronizados.
	INTERRUPCAO interrupcaoGuardada;
	int haInterrupcaoMemoria; //Indica se houve uma INTERRUPCAO_MEMORIA, que tem preferência sob outras interrupções.
};

typedef struct str_controladorInterrupcoes CONTROLADOR_INTERRUPCOES;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* Inicializa o controlador de interrupções.
* @param CONTROLADOR_INTERRUPCOES	*controladorInterrupcoes_param O controlador de interrupções que irá ser inicializado.
*/
void controladorInterrupcoes_inicializar(CONTROLADOR_INTERRUPCOES *controladorInterrupcoes_param);

/**
* @param CONTROLADOR_INTERRUPCOES	*controladorInterrupcoes_param O controlador de interrupções no qual a operação será realizada.
* @param INTERRUPCAO 	interrupcao_param	A interrupção que o controlador deverá guardar.
*/
void controladorInterrupcoes_set(CONTROLADOR_INTERRUPCOES *controladorInterrupcoes_param, INTERRUPCAO interrupcao_param);

/**
* @param CONTROLADOR_INTERRUPCOES	*controladorInterrupcoes_param O controlador de interrupções no qual a operação será realizada.
* @return INTERRUPCAO A interrupção que o controlador está guardando.
*/
INTERRUPCAO controladorInterrupcoes_get(CONTROLADOR_INTERRUPCOES *controladorInterrupcoes_param);

/**
* Reseta o valor da interrupção guardada para indicar que não há interrupção guardada.
* @param CONTROLADOR_INTERRUPCOES	*controladorInterrupcoes_param O controlador de interrupções no qual a operação será realizada.
*/
void controladorInterrupcoes_reset(CONTROLADOR_INTERRUPCOES *controladorInterrupcoes_param);









