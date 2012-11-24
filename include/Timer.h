/**
* Concentra as funções de manipulação do timer.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------
struct str_timer{

};

typedef struct str_timer TIMER;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* Inicia thread do timer.
* @param TIMER	*timer_param	O timer que irá rodar.
*/
void timer_rodar(TIMER *timer_param);
