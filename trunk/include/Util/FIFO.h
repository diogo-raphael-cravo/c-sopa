/**
* Implementa uma FIFO para tipos quaisquer.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define FIFO_ELEMENTO_INEXISTENTE null
#define FIFO_INDICE_ELEMENTO_INEXISTENTE null

struct str_FIFO{
	void** conteudo; //Elementos guardados na fifo.
	int tamanho; //O tamanho da FIFO, medido em número de elementos.
};

typedef struct str_FIFO FIFO;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* Inicia FIFO com o tamanho dado.
* @param FIFO	*fifo_param A fifo que será inicializada.
* @param int	tamanho_param	O tamanho da FIFO, medido em número de elementos.
*/
void FIFO_inicializar(FIFO *fifo_param, int tamanho_param);

/**
* Insere um elemento na última posição da FIFO.
* @param FIFO	*fifo_param A fifo em que a operação será realizada.
* @param void	*dado_param	O dado que será inserido na FIFO.
* ATENÇÃO: retorna/recebe um void*! É necessário utilizar cast!
*/
void FIFO_inserir(FIFO *fifo_param, void* dado_param);

/**
* @param FIFO	*fifo_param A fifo em que a operação será realizada.
* @return int	Indica se a FIFO está vazia.
*/
int FIFO_vazia(FIFO *fifo_param);

/**
* @param FIFO	*fifo_param A fifo em que a operação será realizada.
* @return int	Indica se a FIFO está cheia.
*/
int FIFO_cheia(FIFO *fifo_param);

/**
* Procura o ponteiro nos elementos da fila.
* @param FIFO	*fifo_param A fifo em que a operação será realizada.
* @param void	*dado_param O dado que será procurado na fila.
* @return int	Indica se o ponteiro fornecido está na fila.
*/
int FIFO_buscaElemento(FIFO *fifo_param, void *dado_param);

/**
* Retorna o primeiro elemento da FIFO. O elemento continua na fila.
* @param FIFO	*fifo_param A fifo em que a operação será realizada.
* @return void* O primeiro elemento da FIFO (que está há mais tempo nela).
* ATENÇÃO: Retornará null quando não houver elementos!
* ATENÇÃO: retorna/recebe um void*! É necessário utilizar cast!
*/
void* FIFO_espiar(FIFO *fifo_param);

/**
* Retorna o primeiro elemento da FIFO. O elemento é retirado da fila.
* Atenção! Retornará null quando não houver elementos!
* @param FIFO	*fifo_param A fifo em que a operação será realizada.
* @return void* O primeiro elemento da FIFO (que está há mais tempo nela).
* ATENÇÃO: retorna/recebe um void*! É necessário utilizar cast!
*/
void* FIFO_remover(FIFO *fifo_param);

