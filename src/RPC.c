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
* Envia requisição RPC ao IP e porta fornecidos.
* @param OPERACAO_RPC	operacao_param		A operação desejada.
* @param FIFO			*parametros_param	Os parâmetros enviados.
* @return RPC*	O novo RPC.
*/
RPC* rpc_criarNovo(OPERACAO_RPC operacao_param, FIFO *parametros_param){
	RPC *novo = (RPC*) malloc(sizeof(RPC));
	novo->operacao = operacao_param;
	FIFO_inicializar(&novo->parametros, FIFO_capacidadeNumeroElementos(parametros_param));
	FIFO_copiar(&novo->parametros, parametros_param);
	return novo;
}

/**
* @param RPC		*rpc_param		O RPC que será transformado em string.
* @return char*		String que representa o pacote.
* @see rpc_deString 	Processo reverso.
*/
char* rpc_paraString(RPC *rpc_param){
	char* stringRPC = (char*) malloc(TAMANHO_RPC_STRING*sizeof(char));
	char* novaString  = (char*) malloc(TAMANHO_PACOTE_STRING*sizeof(char));
	memset(stringRPC, '\0', TAMANHO_RPC_STRING);
	memset(novaString, '\0', TAMANHO_PACOTE_STRING);
	sprintf(stringRPC, "%d%c", rpc_param->operacao, SOCKET_SOPA_SEPARADOR);
	switch(rpc_param->operacao){
		case RPC_OPERACAO_ADD:
			memset(novaString, '\0', TAMANHO_PACOTE_STRING);
			sprintf(novaString, "%d%c%d%c", 
				(* (int*) FIFO_espiarPosicao(&rpc_param->parametros, 0)),
				SOCKET_SOPA_SEPARADOR,
				(* (int*) FIFO_espiarPosicao(&rpc_param->parametros, 1)),
				SOCKET_SOPA_SEPARADOR);
			break;
		case RPC_OPERACAO_RESULTADO:
			memset(novaString, '\0', TAMANHO_PACOTE_STRING);
			sprintf(novaString, "%d%c%d%c%d%c%d%c%d%c%d%c", 
				(* (int*) FIFO_espiarPosicao(&rpc_param->parametros, 0)),
				SOCKET_SOPA_SEPARADOR,
				(* (int*) FIFO_espiarPosicao(&rpc_param->parametros, 1)),
				SOCKET_SOPA_SEPARADOR,
				(* (int*) FIFO_espiarPosicao(&rpc_param->parametros, 2)),
				SOCKET_SOPA_SEPARADOR,
				(* (int*) FIFO_espiarPosicao(&rpc_param->parametros, 3)),
				SOCKET_SOPA_SEPARADOR,
				(* (int*) FIFO_espiarPosicao(&rpc_param->parametros, 4)),
				SOCKET_SOPA_SEPARADOR,
				(* (int*) FIFO_espiarPosicao(&rpc_param->parametros, 5)),
				SOCKET_SOPA_SEPARADOR);
			break;
	}
	strcat(stringRPC, novaString);

	return stringRPC;
}

/**
* @param char*		string_param		String que representa o rpc.
* @return RPC*		O rpc que será criado à partir da string.
* @see rpc_paraString 	Processo reverso.
*/
RPC* rpc_deString(char* string_param){
	int iteracoes=0;
	int *novoValor;

	OPERACAO_RPC operacao = atoi(string_pegarSubtexto(string_param, SOCKET_SOPA_SEPARADOR_STRING, 0));
	FIFO *parametros = (FIFO*) malloc(sizeof(FIFO));
	switch(operacao){
		case RPC_OPERACAO_ADD:
			iteracoes = 2;
			break;
		case RPC_OPERACAO_RESULTADO:
			iteracoes = 6;
			break;
	}

	int i;
	char *buffer;
	FIFO_inicializar(parametros, iteracoes);
	for(i=1; i<=iteracoes; i++){
		buffer = string_pegarSubtexto(string_param, SOCKET_SOPA_SEPARADOR_STRING, i);
		novoValor = (int*) malloc(sizeof(int));
		*novoValor = atoi(buffer);
		free(buffer);
		FIFO_inserir(parametros, novoValor);
	}

	return rpc_criarNovo(operacao, parametros);
}

/**
* @param RPC		*rpc_param		O RPC que será consultado.
* @return OPERACAO_RPC		Operação solicitada no RPC.
*/
OPERACAO_RPC rpc_getOperacao(RPC *rpc_param){
	return rpc_param->operacao;
}

/**
* @param RPC		*rpc_param					O RPC que será consultado.
* @param int		ordemParametro_param		Qual parâmetro: primeiro, segundo, terceiro...
* @return void*		Parâmetro do RPC.
*/
void* rpc_getParametro(RPC *rpc_param, int ordemParametro_param){
	return FIFO_espiarPosicao(&rpc_param->parametros, ordemParametro_param);
}

/**
* @param RPC		*rpc_param			O RPC que será impresso na tela.
*/
void rpc_imprimir(RPC *rpc_param){
	char mensagem[200];

	tela_escreverNaColuna(&global_tela, " RPC", 3);
	sprintf(mensagem, " {");
	tela_escreverNaColuna(&global_tela, mensagem, 3);

	sprintf(mensagem, "  Operacao=%d", rpc_param->operacao);
	tela_escreverNaColuna(&global_tela, mensagem, 3);

	int i=0;
	for(; i<FIFO_quantidadeElementos(&rpc_param->parametros); i++){
		sprintf(mensagem, "  p(%d)=%d", i, * (int*) FIFO_espiarPosicao(&rpc_param->parametros, i));
		tela_escreverNaColuna(&global_tela, mensagem, 3);
	}
	sprintf(mensagem, " }");
	tela_escreverNaColuna(&global_tela, mensagem, 3);
}







