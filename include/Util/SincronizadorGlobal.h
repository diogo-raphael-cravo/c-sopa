/**
* Concentra as funções de manipulação do console.
*/
//---------------------------------------------------------------------
//			DADOS
//---------------------------------------------------------------------

//Constantes
#define SINCRONIZADOR_GLOBAL_QUANTUM_MILISSEGUNDOS 50
#define MAXIMO_THREADS_SINCRONIZADAS 20



//---------------------------------------------------------------------
//			FUNÇÕES
//---------------------------------------------------------------------
/**
* Faz inicializações necessárias.
*/
void sincronizadorGlobal_inicializar(void);

/**
* Loop infinito executado pela thread do sincronizador.
*/
void sincronizadorGlobal_rodar(void);

/**
* @param int	quantosMilissegundos_param		Quantidade de milissegundos que durará o sono da thread.
*/
void sincronizadorGlobal_dormir(int quantosMilissegundos_param);

/**
* Avança um quantum no tempo, quando estiver pausado.
*/
void sincronizadorGlobal_sincronizado_avancar(void);

/**
* Pausa avanço do tempo, caso não esteja.
*/
void sincronizadorGlobal_sincronizado_pausar(void);

/**
* Continua avanço no tempo, se estiver pausado.
*/
void sincronizadorGlobal_sincronizado_continuar(void);



