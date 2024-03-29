#include "../include/DadosComuns.h"
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
* @param MAPA_ALOCACOES_MEMORIA	*mapa_param				O mapa que será inicializado.
* @param MMU					*MMU_param				A memória que o mapa representa.
* @param int					maximoAlocacoes_param	O número máximo de alocações que podem ser feitas neste mapa.
*/
void mapaAlocacoesMemoria_inicializar(MAPA_ALOCACOES_MEMORIA *mapa_param, MMU *MMU_param, int maximoAlocacoes_param){
	mapa_param->maximoAlocacoes = maximoAlocacoes_param;
	mapa_param->memoriaRepresentada = MMU_param;
	FIFO_inicializar(&mapa_param->filaMemoriaAlocada, maximoAlocacoes_param);
}

/**
* @param MAPA_ALOCACOES_MEMORIA	*mapa_param						O mapa em que a operação será realizada.
* @param int					tamanhoAlocacaoPalavras_param	O tamanho da alocação, em palavras.
* @return int	O endereço inicial do bloco alocado. Caso não seja possível alocar, retornará MEMORIA_ENDERECO_INEXISTENTE.
*/
int mapaAlocacoesMemoria_alocar(MAPA_ALOCACOES_MEMORIA *mapa_param, int tamanhoAlocacaoPalavras_param){
	ALOCACAO_MEMORIA **alocacaoNova = (ALOCACAO_MEMORIA**)malloc(sizeof(ALOCACAO_MEMORIA*));
	ALOCACAO_MEMORIA **alocacao = (ALOCACAO_MEMORIA**)malloc(sizeof(ALOCACAO_MEMORIA*));
	ALOCACAO_MEMORIA **alocacaoContigua = (ALOCACAO_MEMORIA**)malloc(sizeof(ALOCACAO_MEMORIA*));
	FIFO copiaAlocacao;

	*alocacaoNova = (ALOCACAO_MEMORIA*)malloc(sizeof(ALOCACAO_MEMORIA));
	FIFO_inicializar(&copiaAlocacao, FIFO_capacidadeNumeroElementos(&mapa_param->filaMemoriaAlocada));

	int enderecoLivre = 0;
	int possivelAlocar = 0;
	int aindaNaoAlocou = 1;
	int espacoLivre = 0;

	(*alocacaoNova)->tamanhoAreaMemoriaPalavras = tamanhoAlocacaoPalavras_param;
	if(FIFO_vazia(&mapa_param->filaMemoriaAlocada)){
		enderecoLivre = 0;
		if(MMU_existeEspacoFisicoParaBloco(mapa_param->memoriaRepresentada, enderecoLivre, tamanhoAlocacaoPalavras_param)){
			possivelAlocar = 1;
		}
	} else if(FIFO_quantidadeElementos(&mapa_param->filaMemoriaAlocada) == 1){
		*alocacao = * (ALOCACAO_MEMORIA**) FIFO_remover(&mapa_param->filaMemoriaAlocada);
		enderecoLivre = (*alocacao)->enderecoInicio+(*alocacao)->tamanhoAreaMemoriaPalavras;

		if(MMU_existeEspacoFisicoParaBloco(mapa_param->memoriaRepresentada, enderecoLivre, tamanhoAlocacaoPalavras_param)){
			possivelAlocar = 1;
		}
		FIFO_inserir(&copiaAlocacao, alocacao);
	} else {
		*alocacaoContigua = * (ALOCACAO_MEMORIA**) FIFO_remover(&mapa_param->filaMemoriaAlocada);
		aindaNaoAlocou = 1;
		if(tamanhoAlocacaoPalavras_param < (*alocacaoContigua)->enderecoInicio){
			possivelAlocar = 1;
			aindaNaoAlocou = 0;
			enderecoLivre = (*alocacaoContigua)->enderecoInicio - tamanhoAlocacaoPalavras_param;
		}

		while(!FIFO_vazia(&mapa_param->filaMemoriaAlocada)){
			*alocacao = *alocacaoContigua;
			*alocacaoContigua = * (ALOCACAO_MEMORIA**) FIFO_remover(&mapa_param->filaMemoriaAlocada);
			if(aindaNaoAlocou){
				enderecoLivre = (*alocacao)->enderecoInicio + (*alocacao)->tamanhoAreaMemoriaPalavras;
				espacoLivre = (*alocacaoContigua)->enderecoInicio - enderecoLivre;

				if(MMU_existeEspacoFisicoParaBloco(mapa_param->memoriaRepresentada, enderecoLivre, tamanhoAlocacaoPalavras_param)
						&& tamanhoAlocacaoPalavras_param < espacoLivre){
					possivelAlocar = 1;
					aindaNaoAlocou = 0;
				}
			}
			FIFO_inserir(&copiaAlocacao, alocacao);
		}
		if(aindaNaoAlocou){
			enderecoLivre = (*alocacaoContigua)->enderecoInicio + (*alocacaoContigua)->tamanhoAreaMemoriaPalavras;
			if(MMU_existeEspacoFisicoParaBloco(mapa_param->memoriaRepresentada, enderecoLivre, tamanhoAlocacaoPalavras_param)){
				possivelAlocar = 1;
				aindaNaoAlocou = 0;
			}
		}
	}
	possivelAlocar = possivelAlocar && (!FIFO_cheia(&mapa_param->filaMemoriaAlocada));

	FIFO_copiar(&mapa_param->filaMemoriaAlocada, &copiaAlocacao);
	FIFO_destruir(&copiaAlocacao);
	if(possivelAlocar){
		(*alocacaoNova)->enderecoInicio = enderecoLivre;
		FIFO_inserir(&mapa_param->filaMemoriaAlocada, alocacaoNova);
	} else {
		enderecoLivre = MEMORIA_ENDERECO_INEXISTENTE;
		free(*alocacaoNova);
		free(alocacaoNova);
	}
	//free(alocacao); //Não dê free neste aqui, ou vai dar free no próprio elemento do filaMemoriaAlocada.
	free(alocacaoContigua);

	return enderecoLivre;
}

/**
* @param MAPA_ALOCACOES_MEMORIA	*mapa_param				O mapa em que a operação será realizada.
* @param int					enderecoInicial_param	O endereço que inicia o bloco que será liberado.
*/
void mapaAlocacoesMemoria_liberar(MAPA_ALOCACOES_MEMORIA *mapa_param, int enderecoInicial_param){
	FIFO copiaTemporaria;
	ALOCACAO_MEMORIA** alocacaoDeletada;
	ALOCACAO_MEMORIA **alocacao = (ALOCACAO_MEMORIA**)malloc(sizeof(ALOCACAO_MEMORIA*));
	FIFO_inicializar(&copiaTemporaria, FIFO_quantidadeElementos(&mapa_param->filaMemoriaAlocada));
	while(!FIFO_vazia(&mapa_param->filaMemoriaAlocada)){
		*alocacao = * (ALOCACAO_MEMORIA**) FIFO_espiar(&mapa_param->filaMemoriaAlocada);
		if((*alocacao)->enderecoInicio == enderecoInicial_param){
			alocacaoDeletada = (ALOCACAO_MEMORIA**) FIFO_remover(&mapa_param->filaMemoriaAlocada);
			free(*alocacaoDeletada);
			free(alocacaoDeletada);
		} else {
			FIFO_inserir(&copiaTemporaria, FIFO_remover(&mapa_param->filaMemoriaAlocada));
		}
	}
	FIFO_copiar(&mapa_param->filaMemoriaAlocada, &copiaTemporaria);
	FIFO_destruir(&copiaTemporaria);

	free(alocacao);
}










