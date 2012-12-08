/**
* Concentra as funções de manipulação do disco.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------
struct str_disco{
	sem_t mutexAcessoDisco;
};

typedef struct str_disco DISCO;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* Inicia thread do disco.
* @param DISCO	*disco_param	O disco que irá 'rodar'.
*/
void disco_rodar(DISCO *disco_param);

/**
* Libera o disco para dar uma volta.
* @param DISCO	*disco_param	O disco em que a operação será realizada.
*/
void disco_darUmaVolta(DISCO *disco_param);
