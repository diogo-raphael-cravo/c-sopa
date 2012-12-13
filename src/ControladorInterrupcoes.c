#include "../include/DadosComuns.h"

/**
* Variáveis globais acessíveis somente neste arquivo.
*/



//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/**
* Inicializa o controlador de interrupções.
* @param CONTROLADOR_INTERRUPCOES	*controladorInterrupcoes_param O controlador de interrupções que irá ser inicializado.
*/
void controladorInterrupcoes_inicializar(CONTROLADOR_INTERRUPCOES *controladorInterrupcoes_param){
	sem_init(&controladorInterrupcoes_param->mutexAcesso, 0, 1);
	sem_wait(&controladorInterrupcoes_param->mutexAcesso);
	controladorInterrupcoes_param->interrupcaoGuardada = SEM_INTERRUPCAO;
	controladorInterrupcoes_param->haInterrupcaoMemoria = 0;
	sem_post(&controladorInterrupcoes_param->mutexAcesso);
}

/**
* @param CONTROLADOR_INTERRUPCOES	*controladorInterrupcoes_param O controlador de interrupções no qual a operação será realizada.
* @param INTERRUPCAO 	interrupcao_param	A interrupção que o controlador deverá guardar.
*/
void controladorInterrupcoes_set(CONTROLADOR_INTERRUPCOES *controladorInterrupcoes_param, INTERRUPCAO interrupcao_param){
	sem_wait(&controladorInterrupcoes_param->mutexAcesso);
	if(interrupcao_param == INTERRUPCAO_SEGMENTACAO_MEMORIA){
		controladorInterrupcoes_param->haInterrupcaoMemoria = 1;
	} else {
		controladorInterrupcoes_param->interrupcaoGuardada = interrupcao_param;
	}
}

/**
* @param CONTROLADOR_INTERRUPCOES	*controladorInterrupcoes_param O controlador de interrupções no qual a operação será realizada.
* @return INTERRUPCAO A interrupção que o controlador está guardando.
*/
INTERRUPCAO controladorInterrupcoes_get(CONTROLADOR_INTERRUPCOES *controladorInterrupcoes_param){
	if(controladorInterrupcoes_param->haInterrupcaoMemoria){
		return INTERRUPCAO_SEGMENTACAO_MEMORIA;
	} else {
		return controladorInterrupcoes_param->interrupcaoGuardada;		
	}
}

/**
* Reseta o valor da interrupção guardada para indicar que não há interrupção guardada.
* @param CONTROLADOR_INTERRUPCOES	*controladorInterrupcoes_param O controlador de interrupções no qual a operação será realizada.
*/
void controladorInterrupcoes_reset(CONTROLADOR_INTERRUPCOES *controladorInterrupcoes_param){
	if(controladorInterrupcoes_param->haInterrupcaoMemoria){
		controladorInterrupcoes_param->haInterrupcaoMemoria = 0;
	} else {
		controladorInterrupcoes_param->interrupcaoGuardada = SEM_INTERRUPCAO;
	}
	sem_post(&controladorInterrupcoes_param->mutexAcesso);
}




