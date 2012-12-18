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
*
*/
