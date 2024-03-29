/**
* Este arquivo concentra variáveis que devem ser vistas por muitos arquivos.
*/

#define BOOLEANO int

/*----------------------------------------------------------------------
*						INCLUDES E DEFINES
----------------------------------------------------------------------*/
	//Bibliotecas
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <curses.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
	//Aplicação. Dependências devem ser anotadas em forma de comentários.
#include "../include/Util/FIFO.h" 						//Depende de: 
#include "../include/Util/String.h" 					//Depende de: 
#include "../include/Util/SincronizadorGlobal.h"		//Depende de: 
#include "../include/Util/SocketSOPA.h"					//Depende de:
#include "../include/Tela.h" 							//Depende de: ControladorSOPA.h
#include "../include/Memoria.h" 						//Depende de: Tela.h
#include "../include/MMU.h" 							//Depende de: Memoria.h
#include "../include/ControladorInterrupcoes.h" 		//Depende de: 
#include "../include/Registrador.h"						//Depende de: Memoria.h
#include "../include/Contexto.h"						//Depende de: Registrador.h
#include "../include/Processador.h" 					//Depende de: Memoria.h, Tela.h, Registrador.h, Contexto.h
#include "../include/DescritorProcesso.h" 				//Depende de: Contexto.h, Registrador.h
#include "../include/Disco.h" 							//Depende de: Tela.h, Memoria.h, Processador.h, String.h
#include "../include/MapaAlocacoesMemoria.h" 			//Depende de: Memoria.h
#include "../include/GerenciadorDisco.h" 				//Depende de: Disco.h
#include "../include/Arquivo.h" 						//Depende de: Disco.h, MMU.h
#include "../include/DescritorArquivo.h" 				//Depende de: Arquivo.h
#include "../include/SistemaArquivos.h" 				//Depende de: Arquivo.h, DescritorArquivo.h
#include "../include/RPC.h" 							//Depende de: 
#include "../include/PacoteAplicacaoSOPA.h"				//Depende de: RPC.h
#include "../include/PlacaRede.h"						//Depende de: PacoteAplicacaoSOPA.h
#include "../include/Kernel.h" 							//Depende de: Tela.h, ControladorInterrupcoes.h, DescritorProcesso.h, FIFO.h, Disco.h, MMU.h, MapaMemoria.h, Arquivo.h, SistemaArquivos.h, RPC.h, GerenciadorDisco.h, PlacaRede.h
#include "../include/Timer.h" 							//Depende de: Tela.h, ControladorInterrupcoes.h
#include "../include/Console.h" 						//Depende de: Tela.h, ControladorInterrupcoes.h, ControladorSOPA.h


/*----------------------------------------------------------------------
*						DEFINIÇÕES GLOBAIS
----------------------------------------------------------------------*/



/*----------------------------------------------------------------------
*						VARIÁVEIS GLOBAIS
----------------------------------------------------------------------*/
pthread_t global_threadIdProcessador;
pthread_t global_threadIdTimer;
pthread_t global_threadIdKernel;
pthread_t global_threadIdDisco;
pthread_t global_threadIdConsole;
pthread_t global_threadIdPlacaRede;
pthread_t global_threadIdSincronizadorGlobal;

sem_t global_mutexAcessoTela; //Controla acesso à tela em geral.
sem_t global_mutexParaTela; //Controla acesso à tela somente dos elementos que não são tela.
sem_t global_mutexParaEscritaTela; //Controla acesso da tela à escrita na tela.
sem_t global_mutexParaEscritaConsole; //Controla acesso do console à escrita na tela.

PROCESSADOR 				global_processador;
KERNEL 						global_kernel;
DISCO 						global_disco;
TIMER 						global_timer;
CONTROLADOR_INTERRUPCOES 	global_controladorInterrupcoes;
TELA 						global_tela;
MEMORIA 					global_memoria;
MMU							global_MMU;
CONSOLE 					global_console;
PLACA_REDE					global_placaRede;




