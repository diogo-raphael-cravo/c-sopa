/**
* Concentra as funções de manipulação de RPCs.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define	SOCKET	int
#define INVALID_SOCKET  ((SOCKET)~0)
#define MAX_PACKET 1250

enum enum_operacoesRPC{
	OPERACAO_ADD
};

typedef enum enum_operacoesRPC OPERACAO_RPC;

struct str_RPC{
	int ipDestino;
	int portaDestino;
	OPERACAO_RPC operacao;
	int parametros[10];
};

typedef struct str_RPC RPC;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* Envia requisição RPC ao IP e porta fornecidos.
* @param OPERACAO_RPC	operacao_param		A operação desejada.
* @param int			ipDestino_param		O id do 'servidor' ao qual a requisição será feita.
* @param int			portaDestino_param	A porta do 'servidor' ao qual a requisição será feita.
* @param void*			parametros_param	Os parâmetros enviados.
*/
