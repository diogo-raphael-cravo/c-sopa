#include "../../include/DadosComuns.h"

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
* Inicia FIFO com o tamanho dado.
* @param FIFO	*fifo_param 	A fifo que será inicializada.
* @param int	tamanho_param	O tamanho da FIFO, medido em número de elementos.
*/
void FIFO_inicializar(FIFO *fifo_param, int tamanho_param){
	int tamanho = tamanho_param+1; //quickfix
	fifo_param->tamanho = tamanho;
	fifo_param->conteudo = (void**) malloc(tamanho*sizeof(void*));
	int elemento;
	for(elemento=0; elemento<tamanho; elemento++){
		fifo_param->conteudo[elemento] = FIFO_ELEMENTO_INEXISTENTE;
	}
}

/**
* Procura o ponteiro nos elementos da fila.
* @param FIFO	*fifo_param A fifo em que a operação será realizada.
* @param void	*dado_param O dado que será procurado na fila.
* @return int	Indica se o ponteiro fornecido está na fila.
*/
int FIFO_buscaElemento(FIFO *fifo_param, void *dado_param){
	int indiceElemento;
	int encontrou = 0;
	for(indiceElemento=0; indiceElemento<fifo_param->tamanho; indiceElemento++){
		if(fifo_param->conteudo[indiceElemento] == dado_param){
			encontrou = 1;
		}
	}
	return encontrou;
}

/**
* Insere um elemento na última posição da FIFO.
* @param FIFO	*fifo_param A fifo em que a operação será realizada.
* @param void	*dado_param	O dado que será inserido na FIFO.
* ATENÇÃO: retorna/recebe um void*! É necessário utilizar cast!
*/
void FIFO_inserir(FIFO *fifo_param, void* dado_param){
	int indiceElemento;
	int indicePrimeiraPosicaoLivre = FIFO_INDICE_ELEMENTO_INEXISTENTE;

	if(!FIFO_buscaElemento(fifo_param, dado_param) && !FIFO_cheia(fifo_param)){
		for(indiceElemento=0; indiceElemento<fifo_param->tamanho; indiceElemento++){
			if(fifo_param->conteudo[indiceElemento] == FIFO_ELEMENTO_INEXISTENTE 
					&& indicePrimeiraPosicaoLivre == FIFO_INDICE_ELEMENTO_INEXISTENTE){
				indicePrimeiraPosicaoLivre = indiceElemento;
			}
		}
		fifo_param->conteudo[indicePrimeiraPosicaoLivre] = dado_param;
	} else {
		tela_escreverNaColuna(&global_tela, "Elemento jah existe!", 5);
	}
}

/**
* @param FIFO	*fifo_param A fifo em que a operação será realizada.
* @return int	Indica se a FIFO está vazia.
*/
int FIFO_vazia(FIFO *fifo_param){
	return (fifo_param->conteudo[0] == FIFO_ELEMENTO_INEXISTENTE);
}

/**
* @param FIFO	*fifo_param A fifo em que a operação será realizada.
* @return int	Indica se a FIFO está cheia.
*/
int FIFO_cheia(FIFO *fifo_param){
	return (fifo_param->conteudo[fifo_param->tamanho-1] != FIFO_ELEMENTO_INEXISTENTE);
}

/**
* Retorna o primeiro elemento da FIFO. O elemento continua na fila.
* @param FIFO	*fifo_param A fifo em que a operação será realizada.
* @return void* O primeiro elemento da FIFO (que está há mais tempo nela).
* ATENÇÃO: Retornará null quando não houver elementos!
* ATENÇÃO: retorna/recebe um void*! É necessário utilizar cast!
*/
void* FIFO_espiar(FIFO *fifo_param){
	return fifo_param->conteudo[0];
}

/**
* Retorna o primeiro elemento da FIFO. O elemento é retirado da fila.
* @param FIFO	*fifo_param A fifo em que a operação será realizada.
* @return void* O primeiro elemento da FIFO (que está há mais tempo nela).
* ATENÇÃO: Retornará null quando não houver elementos!
* ATENÇÃO: retorna/recebe um void*! É necessário utilizar cast!
*/
void* FIFO_remover(FIFO *fifo_param){
	int indice;
	void* elemento;
	if(!FIFO_vazia(fifo_param)){
		elemento = fifo_param->conteudo[0];
		for(indice=0; indice<fifo_param->tamanho-1; indice++){
			fifo_param->conteudo[indice] = fifo_param->conteudo[indice+1];
		}
		fifo_param->conteudo[FIFO_quantidadeElementos(fifo_param)] = FIFO_ELEMENTO_INEXISTENTE;
	} else {
		elemento = FIFO_ELEMENTO_INEXISTENTE;
	}
	return elemento;
}

/**
* @param FIFO	*fifo_param A fifo em que a operação será realizada.
* @return int	O número de elementos na fifo.
*/
int FIFO_quantidadeElementos(FIFO *fifo_param){
	int quantidade=0;
	int indice;
	for(indice=0; indice<fifo_param->tamanho-1; indice++){
		if(fifo_param->conteudo[indice] != FIFO_ELEMENTO_INEXISTENTE){
			quantidade++;
		}
	}
	return quantidade;
}

/**
* @param FIFO	*fifo_param	 A fifo cuja informação é requisitada.
* @return int	A capacidade total da FIFO, NÃO o número de elementos que ela tem.
*/
int FIFO_capacidadeNumeroElementos(FIFO *fifo_param){
	return fifo_param->tamanho;
}

/**
* Copia outra FIFO.
* @param FIFO	 *fifoDestino_param	A fifo para a qual a cópia será feita.
* @param FIFO	 *fifoOrigem_param	A fifo para da qual a cópia será feita.
* ATENÇÃO: ambas devem ter o mesmo tamanho!
*/
void FIFO_copiar(FIFO *fifoDestino_param, FIFO *fifoOrigem_param){
	FIFO copiaTemporaria;

	FIFO_inicializar(&copiaTemporaria, fifoOrigem_param->tamanho);
	FIFO_esvaziar(fifoDestino_param);

	while(!FIFO_vazia(fifoOrigem_param)){
		FIFO_inserir(&copiaTemporaria, FIFO_espiar(fifoOrigem_param));
		FIFO_inserir(fifoDestino_param, FIFO_remover(fifoOrigem_param));
	}
	while(!FIFO_vazia(&copiaTemporaria)){
		FIFO_inserir(fifoOrigem_param, FIFO_remover(&copiaTemporaria));
	}
}

/**
* Limpa a FIFO, tirando todos seus elementos.
* @param FIFO	*fifo_param	A fifo que será limpa.
*/
void FIFO_esvaziar(FIFO *fifo_param){
	while(!FIFO_vazia(fifo_param)){
		FIFO_remover(fifo_param);
	}
}

/**
* Libera a memória alocada.
* @param FIFO	*fifo_param A fifo que será destruída.
*/
void FIFO_destruir(FIFO *fifo_param){
	//free(fifo_param->conteudo);
	//free(fifo_param);
}

/**
* @param FIFO	*fifo_param				A fifo em que a busca será feita.
* @ṕaram int	posicaoElemento_param	A posição do elemento que se deseja.
* @return void* 	O elemento que está na posicaoElemento_param da FIFO. Considera 0 como a primeira posição.
*					Retornará FIFO_ELEMENTO_INEXISTENTE, caso não haja.
* ATENÇÃO: retorna/recebe um void*! É necessário utilizar cast!
* ATENÇÃO: o elemento NÂO é removido!
*/
void* FIFO_espiarPosicao(FIFO *fifo_param, int posicaoElemento_param){
	if(FIFO_capacidadeNumeroElementos(fifo_param) <= posicaoElemento_param){
		return FIFO_ELEMENTO_INEXISTENTE;
	} else if(FIFO_quantidadeElementos(fifo_param) <= posicaoElemento_param){
		return FIFO_ELEMENTO_INEXISTENTE;
	} else {
		return fifo_param->conteudo[posicaoElemento_param];
	}
}

/**
* @param FIFO	*fifo_param				A fifo em que a busca será feita.
* @param int	posicaoElemento_param	A posição do elemento que se deseja.
* @return void* 	O elemento que está na posicaoElemento_param da FIFO. Considera 0 como a primeira posição.
*					Retornará FIFO_ELEMENTO_INEXISTENTE, caso não haja.
* ATENÇÃO: retorna/recebe um void*! É necessário utilizar cast!
*/
void* FIFO_removerPosicao(FIFO *fifo_param, int posicaoElemento_param){
	if(FIFO_capacidadeNumeroElementos(fifo_param) <= posicaoElemento_param){
		return FIFO_ELEMENTO_INEXISTENTE;
	} else if(FIFO_quantidadeElementos(fifo_param) <= posicaoElemento_param){
		return FIFO_ELEMENTO_INEXISTENTE;
	} else {
		void* elemento = fifo_param->conteudo[posicaoElemento_param];
		int indice;
		for(indice=posicaoElemento_param; indice<fifo_param->tamanho-1; indice++){
			fifo_param->conteudo[indice] = fifo_param->conteudo[indice+1];
		}
		fifo_param->conteudo[FIFO_quantidadeElementos(fifo_param)] = FIFO_ELEMENTO_INEXISTENTE;
		return elemento;
	}
}
