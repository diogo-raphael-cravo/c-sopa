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
	char* token = strtok(string_param, SOCKET_SOPA_SEPARADOR_STRING);

	OPERACAO_RPC operacao = atoi(token);
	FIFO *parametros = (FIFO*) malloc(sizeof(FIFO));
	switch(operacao){
		case RPC_OPERACAO_ADD:
			FIFO_inicializar(parametros, 2);

			token = strtok(string_param, SOCKET_SOPA_SEPARADOR_STRING);
			int *novoValor = (int*) malloc(sizeof(int));
			*novoValor = atoi(token);

			FIFO_inserir(parametros, novoValor);
			token = strtok(string_param, SOCKET_SOPA_SEPARADOR_STRING);
			novoValor = (int*) malloc(sizeof(int));
			*novoValor = atoi(token);
			FIFO_inserir(parametros, novoValor);
			break;
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











