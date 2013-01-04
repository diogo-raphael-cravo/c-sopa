/**
* Concentra as funções de manipulação de RPCs.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define TAMANHO_RPC_STRING 100

enum enum_operacoesRPC{
	OPERACAO_ADD
};

typedef enum enum_operacoesRPC OPERACAO_RPC;

struct str_RPC{
	OPERACAO_RPC operacao;
	void* parametros;
};

typedef struct str_RPC RPC;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* Envia requisição RPC ao IP e porta fornecidos.
* @param OPERACAO_RPC	operacao_param		A operação desejada.
* @param void*			parametros_param	Os parâmetros enviados.
* @return RPC*	O novo RPC.
*/
RPC* rpc_criarNovo(OPERACAO_RPC operacao_param, void* parametros_param);

/**
* @param RPC		*rpc_param		O RPC que será transformado em string.
* @return char*		String que representa o pacote.
* @see rpc_deString 	Processo reverso.
*/
char* rpc_paraString(RPC *rpc_param);

/**
* @param char*		string_param		String que representa o rpc.
* @return RPC*		O rpc que será criado à partir da string.
* @see rpc_paraString 	Processo reverso.
*/
RPC* rpc_deString(char* string_param);

/*
* @param KERNEL		*kernel_param		O kernel que executará a operação.
* @param RPC		*rpc_param			A solicitação RPC.
* ATENÇÃO: só deve ser usada pelo kernel! Usa contexto de processos.
*/













