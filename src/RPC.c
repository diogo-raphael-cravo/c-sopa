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
	sprintf(stringRPC, "%d\n", rpc_param->operacao);

	switch(rpc_param->operacao){
		case OPERACAO_ADD:
			memset(novaString, '\0', TAMANHO_PACOTE_STRING);
			sprintf(novaString, "%d\n%d\n", 
				(* (int*) FIFO_espiarPosicao(&rpc_param->parametros, 0)),
				(* (int*) FIFO_espiarPosicao(&rpc_param->parametros, 1)));
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
	char* token = strtok(string_param, "\n");

	OPERACAO_RPC operacao = atoi(token);
	FIFO *parametros = (FIFO*) malloc(sizeof(FIFO));
	switch(operacao){
		case OPERACAO_ADD:
			FIFO_inicializar(parametros, 2);

			token = strtok(string_param, "\n");
			int *novoValor = (int*) malloc(sizeof(int));
			*novoValor = atoi(token);

			FIFO_inserir(parametros, novoValor);
			token = strtok(string_param, "\n");
			novoValor = (int*) malloc(sizeof(int));
			*novoValor = atoi(token);
			FIFO_inserir(parametros, novoValor);
			break;
	}
	return rpc_criarNovo(operacao, parametros);
}













