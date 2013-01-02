/**
* Implementa uma FIFO para tipos quaisquer.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define FIFO_ELEMENTO_INEXISTENTE NULL
#define FIFO_INDICE_ELEMENTO_INEXISTENTE -1

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

/**
* @param FIFO	*fifo_param	 A fifo em que a operação será realizada.
* @return int	O número de elementos na fifo.
*/
int FIFO_quantidadeElementos(FIFO *fifo_param);

/**
* @param FIFO	*fifo_param	 A fifo cuja informação é requisitada.
* @return int	A capacidade total da FIFO, NÃO o número de elementos que ela tem.
*/
int FIFO_capacidadeNumeroElementos(FIFO *fifo_param);

/**
* Copia outra FIFO.
* @param FIFO	 *fifoDestino_param	A fifo para a qual a cópia será feita.
* @param FIFO	 *fifoOrigem_param	A fifo para da qual a cópia será feita.
* ATENÇÃO: ambas devem ter o mesmo tamanho!
*/
void FIFO_copiar(FIFO *fifoDestino_param, FIFO *fifoOrigem_param);

/**
* Limpa a FIFO, tirando todos seus elementos.
* @param FIFO	*fifo_param	A fifo que será limpa.
*/
void FIFO_esvaziar(FIFO *fifo_param);

/**
* Libera a memória alocada.
* @param FIFO	*fifo_param A fifo que será destruída.
*/
void FIFO_destruir(FIFO *fifo_param);

/**
* @param FIFO	*fifo_param				A fifo em que a busca será feita.
* @param int	posicaoElemento_param	A posição do elemento que se deseja.
* @return void* 	O elemento que está na posicaoElemento_param da FIFO. Considera 0 como a primeira posição.
*					Retornará FIFO_ELEMENTO_INEXISTENTE, caso não haja.
* ATENÇÃO: retorna/recebe um void*! É necessário utilizar cast!
* ATENÇÃO: o elemento NÂO é removido!
*/
void* FIFO_espiarPosicao(FIFO *fifo_param, int posicaoElemento_param);

/**
* @param FIFO	*fifo_param				A fifo em que a busca será feita.
* @param int	posicaoElemento_param	A posição do elemento que se deseja.
* @return void* 	O elemento que está na posicaoElemento_param da FIFO. Considera 0 como a primeira posição.
*					Retornará FIFO_ELEMENTO_INEXISTENTE, caso não haja.
* ATENÇÃO: retorna/recebe um void*! É necessário utilizar cast!
*/
void* FIFO_removerPosicao(FIFO *fifo_param, int posicaoElemento_param);




