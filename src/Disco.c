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
* Inicia thread do disco.
* @param DISCO	*disco_param	O disco que irá 'rodar'.
*/
void disco_rodar(DISCO *disco_param){
	sem_init(&disco_param->mutexAcessoDisco, 0, 0);
	while(1){
		sem_wait(&disco_param->mutexAcessoDisco);
		controladorInterrupcoes_set(&global_controladorInterrupcoes, INTERRUPCAO_DISCO);
		tela_escreverNaColuna(&global_tela, "Mensagem do DISCO.",4);
	}
}

/**
* Libera o disco para dar uma volta.
* @param DISCO	*disco_param	O disco em que a operação será realizada.
*/
void disco_darUmaVolta(DISCO *disco_param){
	sem_post(&disco_param->mutexAcessoDisco);
}