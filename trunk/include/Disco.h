/**
* Concentra as funções de manipulação do disco.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define CAMINHO_ARQUIVO_BASE_DISCO "dados/disco.txt"

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

/**
* Carrega os dados do arquivo, juntando-os aos dados que estão no disco.
* Os dados são colocados à partir da primeira posição livre neste disco.
* @param DISCO	*disco_param			O disco em que a operação será realizada.
* @param char*	*caminhoArquivo_param	Arquivo que contém os dados, formatados da seguinte forma:
*										J P A 0
*										255 255 255 255
*										0 0 0 0
*/
void disco_carregar(DISCO *disco_param, char *caminhoArquivo_param);
