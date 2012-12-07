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
	fifo_param->tamanho = tamanho_param;
	fifo_param->conteudo = (void**) malloc(tamanho_param*sizeof(void*));
	int elemento;
	for(elemento=0; elemento<tamanho_param; elemento++){
		fifo_param->conteudo[elemento] = FIFO_ELEMENTO_INEXISTENTE;
	}
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

	if(!FIFO_contem(fifo_param, dado_param)){
		for(indiceElemento=0; indiceElemento<fifo_param->tamanho; indiceElemento++){
			if(fifo_param->conteudo[indiceElemento] == FIFO_ELEMENTO_INEXISTENTE 
					&& indicePrimeiraPosicaoLivre == FIFO_INDICE_ELEMENTO_INEXISTENTE){
				indicePrimeiraPosicaoLivre = indiceElemento;
			}
		}
		fifo_param[indicePrimeiraPosicaoLivre] = dado_param;
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
* Procura o ponteiro nos elementos da fila.
* @param FIFO	*fifo_param A fifo em que a operação será realizada.
* @param void	*dado_param O dado que será procurado na fila.
* @return int	Indica se o ponteiro fornecido está na fila.
*/
int FIFO_contem(FIFO *fifo_param, void *dado_param){
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
	} else {
		elemento = FIFO_ELEMENTO_INEXISTENTE;
	}
	return elemento;
}


