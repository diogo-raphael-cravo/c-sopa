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
	//Aplicação
#include "../include/Tela.h"
#include "../include/Kernel.h"
#include "../include/Processador.h"
#include "../include/Disco.h"
#include "../include/Timer.h"
#include "../include/Memoria.h"
#include "../include/Console.h"
#include "../include/ControladorInterrupcoes.h"
	//Bibliotecas
#include <stdio.h>

/*----------------------------------------------------------------------
*						VARIÁVEIS GLOBAIS
----------------------------------------------------------------------*/
PROCESSADOR 				global_processador;
KERNEL 						global_kernel;
DISCO 						global_disco;
TIMER 						global_timer;
CONTROLADOR_INTERRUPCOES 	global_controladorInterrupcoes;
TELA 						global_tela;
MEMORIA 					global_memoria;
CONSOLE 					global_console;

/*----------------------------------------------------------------------
*						FUNÇÕES GLOBAIS
----------------------------------------------------------------------*/



/*----------------------------------------------------------------------
*							MAIN
----------------------------------------------------------------------*/
int main(void)
{
	tela_inicializar(&global_tela);
	tela_adicionarColuna(&global_tela, "       PROCESSADOR");
	tela_adicionarColuna(&global_tela, "          TIMER");
	tela_adicionarColuna(&global_tela, "         KERNEL");
	tela_adicionarColuna(&global_tela, "          DISCO");
	tela_adicionarColuna(&global_tela, "        CONSOLE");
	
/*	tela_escreverNaColuna(&global_tela, "1",1);
	tela_escreverNaColuna(&global_tela, "2",2);
	tela_escreverNaColuna(&global_tela, "3",3);
	tela_escreverNaColuna(&global_tela, "4",4);
	tela_escreverNaColuna(&global_tela, "5",5);
	tela_escreverNaColuna(&global_tela, "6",1);
	tela_escreverNaColuna(&global_tela, "7",2);
	tela_escreverNaColuna(&global_tela, "8",3);
	tela_escreverNaColuna(&global_tela, "9",4);
	tela_escreverNaColuna(&global_tela, "10",5);
	tela_escreverNaColuna(&global_tela, "11",1);
	tela_escreverNaColuna(&global_tela, "12",2);
	tela_escreverNaColuna(&global_tela, "13",3);
	tela_escreverNaColuna(&global_tela, "14",4);
	tela_escreverNaColuna(&global_tela, "15",5);
	tela_escreverNaColuna(&global_tela, "16",1);
	tela_escreverNaColuna(&global_tela, "17",2);
	tela_escreverNaColuna(&global_tela, "18",3);
	tela_escreverNaColuna(&global_tela, "19",4);
	tela_escreverNaColuna(&global_tela, "20",5);
	tela_escreverNaColuna(&global_tela, "21",1);
	tela_escreverNaColuna(&global_tela, "22",2);
	tela_escreverNaColuna(&global_tela, "23",3);
	tela_escreverNaColuna(&global_tela, "24",4);
	tela_escreverNaColuna(&global_tela, "25",5);
	tela_escreverNaColuna(&global_tela, "26",1);
	tela_escreverNaColuna(&global_tela, "27",2);
	tela_escreverNaColuna(&global_tela, "28",3);
	tela_escreverNaColuna(&global_tela, "29",4);
	tela_escreverNaColuna(&global_tela, "30",5);
	tela_escreverNaColuna(&global_tela, "31",1);
	tela_escreverNaColuna(&global_tela, "32",2);
	tela_escreverNaColuna(&global_tela, "33",3);
	tela_escreverNaColuna(&global_tela, "34",4);
	tela_escreverNaColuna(&global_tela, "35",5);
	tela_escreverNaColuna(&global_tela, "36",1);
	tela_escreverNaColuna(&global_tela, "37",2);
	tela_escreverNaColuna(&global_tela, "38",3);
	tela_escreverNaColuna(&global_tela, "39",4);
	tela_escreverNaColuna(&global_tela, "40",5);
	tela_rolar(&global_tela, -5);*/
	tela_escreverNaColuna(&global_tela, "COLUNA1>Estah eh a mensagem de coluna um e ela eh muito grande. Tao grande que vai ter que ser enrolada em novas linhas.",1);
	tela_escreverNaColuna(&global_tela, "COLUNA2>Estah eh a mensagem de coluna um e ela eh muito grande. Tao grande que vai ter que ser enrolada em novas linhas.",2);
	tela_escreverNaColuna(&global_tela, "COLUNA3>Estah eh a mensagem de coluna um e ela eh muito grande. Tao grande que vai ter que ser enrolada em novas linhas.",3);
	tela_escreverNaColuna(&global_tela, "COLUNA4>Estah eh a mensagem de coluna um e ela eh muito grande. Tao grande que vai ter que ser enrolada em novas linhas.",4);
	tela_escreverNaColuna(&global_tela, "COLUNA5>Estah eh a mensagem de coluna um e ela eh muito grande. Tao grande que vai ter que ser enrolada em novas linhas.",5);
	tela_escreverNaColuna(&global_tela, "COLUNA1>Estah eh a mensagem de coluna um e ela eh muito grande. Tao grande que vai ter que ser enrolada em novas linhas.",1);
	tela_escreverNaColuna(&global_tela, "COLUNA2>Estah eh a mensagem de coluna um e ela eh muito grande. Tao grande que vai ter que ser enrolada em novas linhas.",2);
	tela_escreverNaColuna(&global_tela, "COLUNA3>Estah eh a mensagem de coluna um e ela eh muito grande. Tao grande que vai ter que ser enrolada em novas linhas.",3);
	tela_escreverNaColuna(&global_tela, "COLUNA4>Estah eh a mensagem de coluna um e ela eh muito grande. Tao grande que vai ter que ser enrolada em novas linhas.",4);
	tela_escreverNaColuna(&global_tela, "COLUNA5>Estah eh a mensagem de coluna um e ela eh muito grande. Tao grande que vai ter que ser enrolada em novas linhas.",5);
	tela_escreverNaColuna(&global_tela, "COLUNA1>Estah eh a mensagem de coluna um e ela eh muito grande. Tao grande que vai ter que ser enrolada em novas linhas.",1);
	tela_escreverNaColuna(&global_tela, "COLUNA2>Estah eh a mensagem de coluna um e ela eh muito grande. Tao grande que vai ter que ser enrolada em novas linhas.",2);
	tela_escreverNaColuna(&global_tela, "COLUNA3>Estah eh a mensagem de coluna um e ela eh muito grande. Tao grande que vai ter que ser enrolada em novas linhas.",3);
	tela_escreverNaColuna(&global_tela, "COLUNA4>Estah eh a mensagem de coluna um e ela eh muito grande. Tao grande que vai ter que ser enrolada em novas linhas.",4);
	tela_escreverNaColuna(&global_tela, "COLUNA5>Estah eh a mensagem de coluna um e ela eh muito grande. Tao grande que vai ter que ser enrolada em novas linhas.",5);

	tela_rodar(&global_tela);
	processador_rodar(&global_processador);
	kernel_rodar(&global_kernel);
	disco_rodar(&global_disco);
	timer_rodar(&global_timer);
	controladorInterrupcoes_rodar(&global_controladorInterrupcoes);
	console_rodar(&global_console);

	tela_fechar(&global_tela);
	return 0;
}

