/**
* Concentra as funções de manipulação do controlador de interrupções.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------
struct str_controladorInterrupcoes{

};

typedef struct str_controladorInterrupcoes CONTROLADOR_INTERRUPCOES;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* Inicia thread do controlador de interrupções.
* CONTROLADOR_INTERRUPCOES	*controladorInterrupcoes_param O controlador de interrupções que irá rodar.
*/
void controladorInterrupcoes_rodar(CONTROLADOR_INTERRUPCOES *controladorInterrupcoes_param);
