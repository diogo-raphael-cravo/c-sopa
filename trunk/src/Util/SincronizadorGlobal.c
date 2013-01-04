#include "../../include/DadosComuns.h"

/**
* Variáveis globais acessíveis somente neste arquivo.
*/

struct str_sincronizadorGlobal{
	FIFO semaforosThreadsDormindo;
	int estahPausado;
	sem_t trava;
	sem_t sincronizador;
	sem_t inicializador;
};

typedef struct str_sincronizadorGlobal SINCRONIZADOR_GLOBAL;

//---------------------------------------------------------------------
//			DEFINIÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------

SINCRONIZADOR_GLOBAL privado_sincronizador;


//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------
/**
* Acorda todas as threads que dormem.
*/
void privada_sincronizado_acordarTodos(void){
	sem_wait(&privado_sincronizador.sincronizador);
	int semaforoAcordado = 0;
	for(; semaforoAcordado<FIFO_quantidadeElementos(&privado_sincronizador.semaforosThreadsDormindo); semaforoAcordado++){
		sem_post((sem_t*) FIFO_removerPosicao(&privado_sincronizador.semaforosThreadsDormindo, semaforoAcordado));
	}
	sem_post(&privado_sincronizador.sincronizador);
}

/**
* Manda thread dormir até ser acordada com acordarTodos.
* @param sem_t		*semaforoThread_param		Semáforo que coordena sono da thread.
*/
void privada_dormir(sem_t *semanaforoThread_param){
	FIFO_inserir(&privado_sincronizador.semaforosThreadsDormindo, semanaforoThread_param);
	sem_wait(semanaforoThread_param);
}

//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/**
* Faz inicializações necessárias.
*/
void sincronizadorGlobal_inicializar(void){
	FIFO_inicializar(&privado_sincronizador.semaforosThreadsDormindo, MAXIMO_THREADS_SINCRONIZADAS);
	privado_sincronizador.estahPausado = 0;
	sem_init(&privado_sincronizador.trava, 0, 1);
	sem_init(&privado_sincronizador.sincronizador, 0, 1);
}

/**
* Loop infinito executado pela thread do sincronizador.
*/
void sincronizadorGlobal_rodar(void){
	while(1){
		sem_wait(&privado_sincronizador.trava);
		if(!privado_sincronizador.estahPausado){
			sem_post(&privado_sincronizador.trava);
		}
		usleep(SINCRONIZADOR_GLOBAL_QUANTUM_MILISSEGUNDOS*1000);
		privada_sincronizado_acordarTodos();
	}
}

/**
* @param int	quantosMilissegundos_param		Quantidade de milissegundos que durará o sono da thread.
*/
void sincronizadorGlobal_dormir(int quantosMilissegundos_param){
	int quantumsPassados = 0;
	sem_t *novoSemaforo = (sem_t*) malloc(sizeof(sem_t));
	sem_init(novoSemaforo, 0, 0);
	int quantosQuantums = quantosMilissegundos_param/SINCRONIZADOR_GLOBAL_QUANTUM_MILISSEGUNDOS;
	for(; quantumsPassados<quantosQuantums; quantumsPassados++){
		privada_dormir(novoSemaforo);
	}
	free(novoSemaforo);
//	usleep(1000*quantosMilissegundos_param);
}

/**
* Avança um quantum no tempo, quando estiver pausado.
*/
void sincronizadorGlobal_sincronizado_avancar(void){
	sem_wait(&privado_sincronizador.sincronizador);
	if(privado_sincronizador.estahPausado){
		sem_post(&privado_sincronizador.trava);
	}
	sem_post(&privado_sincronizador.sincronizador);
}

/**
* Pausa avanço do tempo, caso não esteja.
*/
void sincronizadorGlobal_sincronizado_pausar(void){
	sem_wait(&privado_sincronizador.sincronizador);
	if(!privado_sincronizador.estahPausado){
		privado_sincronizador.estahPausado = !privado_sincronizador.estahPausado;
		sem_wait(&privado_sincronizador.trava);
	}
	sem_post(&privado_sincronizador.sincronizador);
}

/**
* Continua avanço no tempo, se estiver pausado.
*/
void sincronizadorGlobal_sincronizado_continuar(void){
	sem_wait(&privado_sincronizador.sincronizador);
	if(privado_sincronizador.estahPausado){
		privado_sincronizador.estahPausado = !privado_sincronizador.estahPausado;
		sem_post(&privado_sincronizador.trava);
	}
	sem_post(&privado_sincronizador.sincronizador);
}

















