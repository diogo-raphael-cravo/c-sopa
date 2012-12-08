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
* Inicia thread do timer.
* @param TIMER	*timer_param	O timer que irá rodar.
*/
void timer_rodar(TIMER *timer_param){
	while(1){
		usleep(1000*1000);
		controladorInterrupcoes_set(&global_controladorInterrupcoes, INTERRUPCAO_TIMER);
		tela_escreverNaColuna(&global_tela, "Mensagem do TIMER.", 2);
	}
}
