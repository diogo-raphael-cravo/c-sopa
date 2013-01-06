/**
* SOPA (Sistema operacional para avaliação)
* UNIVERSIDADE FEDERAL DO RIO GRANDE DO SUL (UFRGS) - INSTITUTO DE INFORMÁTICA (INF)
* SISTEMAS OPERACIONAIS II - 2012/2 - Professor: Marcelo Johann
* Autores: 	Diogo Raphael Cravo
*			Ederson Vieira
*/

/*----------------------------------------------------------------------
*						INCLUDES E DEFINES
----------------------------------------------------------------------*/
#include "../include/DadosComuns.h"

/*----------------------------------------------------------------------
*						VARIÁVEIS GLOBAIS
----------------------------------------------------------------------*/


/*----------------------------------------------------------------------
*						FUNÇÕES GLOBAIS
----------------------------------------------------------------------*/



/*----------------------------------------------------------------------
*							MAIN
----------------------------------------------------------------------*/
int main(void){
	sem_init(&global_mutexAcessoTela, 0, 1);
	sem_init(&global_mutexParaTela, 0, 1);
	sem_init(&global_mutexParaEscritaTela, 0, 0);
	sem_init(&global_mutexParaEscritaConsole, 0, 0);

	tela_inicializar(&global_tela);
	tela_adicionarColuna(&global_tela, "       PROCESSADOR");
	tela_adicionarColuna(&global_tela, "          TIMER");
	tela_adicionarColuna(&global_tela, "          KERNEL");
	tela_adicionarColuna(&global_tela, "          DISCO");
	tela_adicionarColuna(&global_tela, "         CONSOLE");

	console_inicializar(&global_console);
	memoria_sincronizado_inicializar(&global_memoria);
	MMU_sincronizado_inicializar(&global_MMU, &global_memoria);
	disco_inicializar(&global_disco);
	placaRede_inicializar(&global_placaRede);
	kernel_inicializar(&global_kernel);
	controladorInterrupcoes_inicializar(&global_controladorInterrupcoes);
	sincronizadorGlobal_inicializar();

	pthread_create(&global_threadIdSincronizadorGlobal, NULL, sincronizadorGlobal_rodar, 0);
	pthread_create(&global_threadIdDisco, NULL, disco_rodar, &global_disco);
	//usleep(1000*1000);
	pthread_create(&global_threadIdPlacaRede, NULL, placaRede_rodar, &global_placaRede);
	pthread_create(&global_threadIdProcessador, NULL, processador_rodar, &global_processador);
	pthread_create(&global_threadIdTimer, NULL, timer_rodar, &global_timer);
	pthread_create(&global_threadIdConsole, NULL, console_rodar, &global_console);
	tela_rodar(&global_tela);

	tela_fechar(&global_tela);
	return 0;
}

