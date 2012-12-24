/**
* Concentra as funções de manipulação do controlador de interrupções.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes

enum enum_interrupcoes{
	SEM_INTERRUPCAO=0,
	INTERRUPCAO_PROCESSADOR=1,
	INTERRUPCAO_TIMER=2,
	INTERRUPCAO_SEGMENTACAO_MEMORIA=3,
	INTERRUPCAO_DISCO=5,
	INTERRUPCAO_CONSOLE=15,
	INTERRUPCAO_RPC,
	INTERRUPCAO_SOFTWARE_EXIT = 32,
	INTERRUPCAO_SOFTWARE_KILL = 33,
	INTERRUPCAO_SOFTWARE_OPEN = 34,
	INTERRUPCAO_SOFTWARE_CLOSE = 35,
	INTERRUPCAO_SOFTWARE_GET = 36,
	INTERRUPCAO_SOFTWARE_PUT = 37,
	INTERRUPCAO_SOFTWARE_READ = 38,
	INTERRUPCAO_SOFTWARE_WRITE = 39,
	INTERRUPCAO_SOFTWARE_SEMINIT = 42,
	INTERRUPCAO_SOFTWARE_P = 43,
	INTERRUPCAO_SOFTWARE_V = 44,
	INTERRUPCAO_SOFTWARE_PRINT = 46
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









