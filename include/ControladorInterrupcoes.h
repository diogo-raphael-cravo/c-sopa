/**
* Concentra as funções de manipulação do controlador de interrupções.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define SEM_INTERRUPCAO 0
#define INTERRUPCAO_TIMER 2
#define INTERRUPCAO_DISCO 5

typedef int INTERRUPCAO;

struct str_controladorInterrupcoes{
	sem_t mutexAcesso; //Controla acesso a métodos sincronizados.
	INTERRUPCAO interrupcaoGuardada;
};

typedef struct str_controladorInterrupcoes CONTROLADOR_INTERRUPCOES;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* Inicializa o controlador de interrupções.
* @param CONTROLADOR_INTERRUPCOES	*controladorInterrupcoes_param O controlador de interrupções que irá ser inicializado.
*/
void controladorInterrupcoes_sincronizado_inicializar(CONTROLADOR_INTERRUPCOES *controladorInterrupcoes_param);

/**
* @param CONTROLADOR_INTERRUPCOES	*controladorInterrupcoes_param O controlador de interrupções no qual a operação será realizada.
* @param INTERRUPCAO 	interrupcao_param	A interrupção que o controlador deverá guardar.
*/
void controladorInterrupcoes_sincronizado_set(CONTROLADOR_INTERRUPCOES *controladorInterrupcoes_param, INTERRUPCAO interrupcao_param);

/**
* @param CONTROLADOR_INTERRUPCOES	*controladorInterrupcoes_param O controlador de interrupções no qual a operação será realizada.
* @return INTERRUPCAO A interrupção que o controlador está guardando.
*/
INTERRUPCAO controladorInterrupcoes_sincronizado_get(CONTROLADOR_INTERRUPCOES *controladorInterrupcoes_param);

/**
* Reseta o valor da interrupção guardada para indicar que não há interrupção guardada.
* @param CONTROLADOR_INTERRUPCOES	*controladorInterrupcoes_param O controlador de interrupções no qual a operação será realizada.
*/
void controladorInterrupcoes_sincronizado_reset(CONTROLADOR_INTERRUPCOES *controladorInterrupcoes_param);









