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
* @param void*			parametros_param	Os parâmetros enviados.
* @return RPC*	O novo RPC.
*/
RPC* rpc_criarNovo(OPERACAO_RPC operacao_param, void* parametros_param){
	RPC *novo = (RPC*) malloc(sizeof(RPC));
	novo->operacao = operacao_param;
	if(operacao_param == OPERACAO_ADD){
		novo->parametros = parametros_param;
	}
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
	itoa(rpc_param->operacao, novaString, 10);
	strcat(stringRPC, novaString);
	strcat(stringRPC, "\n");

	switch(rpc_param->operacao){
		case OPERACAO_ADD:
			memset(novaString, '\0', TAMANHO_PACOTE_STRING);
			itoa(((int) rpc_param->parametros[0]), novaString, 10);
			strcat(stringRPC, novaString);
			strcat(stringRPC, "\n");

			memset(novaString, '\0', TAMANHO_PACOTE_STRING);
			itoa((int) rpc_param->parametros[1], novaString, 10);
			strcat(stringRPC, novaString);
			strcat(stringRPC, "\n");
			break;
	}

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
	void* parametros;
	switch(operacao){
		case OPERACAO_ADD:
			parametros = (void*) malloc(2*sizeof(int));
			token = strtok(string_param, "\n");
			parametros[0] = atoi(token);
			token = strtok(string_param, "\n");
			parametros[1] = atoi(token);
			break;
	}
	return rpc_criarNovo(operacao, parametros);
}













