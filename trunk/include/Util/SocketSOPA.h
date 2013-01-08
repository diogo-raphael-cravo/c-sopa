/**
* Concentra as funções de manipulação de RPCs.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define TAMANHOBUFFER 256
#define SOCKET_SOPA_SEPARADOR ','
#define SOCKET_SOPA_SEPARADOR_STRING ","
#define PORTA_TCP 10050
#define TAMANHOFILA 5

enum enum_erroRede{
	ERRO_REDE_NENHUM,
	ERRO_REDE_CRIACAO_SOCKET,
	ERRO_REDE_CONEXAO_SERVIDOR,
	ERRO_REDE_ENVIO_DADOS
};

typedef enum enum_erroRede ERRO_REDE;

struct str_dadosRequerente{
	int PID_processoRequerente;
	BOOLEANO chamadaBloqueante;
};

typedef struct str_dadosRequerente DADOS_REQUERENTE;

struct str_socketSOPA{
	struct sockaddr_in servidor;
	int serverSock;
};

typedef struct str_socketSOPA SOCKET_SOPA;


//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* @param SOCKET_SOPA		*socket_param		O socket que será inicializado.
*/
void socketSopa_inicializar(SOCKET_SOPA *socket_param);

/**
* @param SOCKET_SOPA		*socket_param		O socket que irá esperar.
* @param char*				destino_param		String na qual será colocada a mensagem recebida.
*												Ao final, concatena o IP de origem da mensagem.
*												Separa da mensagem com SOCKET_SOPA_SEPARADOR.
*/
void socketSopa_esperarMensagem(SOCKET_SOPA *socket_param, char* destino_param);

/**
* @param char*				ip_param					IP no formato "127.0.0.1". NENHUM campo deve começar em 0 (algo do tipo "127.0.0.01" causará erro).
* @param char*				mensagem_param				A mensagem que será enviada.
* @param DADOS_REQUERENTE*	*dadosRequerente_param		Dados que estarão disponíveis depois que uma
*														mensagem for enviada.
* ATENÇÃO: a mensagem já deve ter sido alocada!
*/
void socketSopa_enviarMensagem(char* ip_param, char* mensagem_param, DADOS_REQUERENTE* dadosRequerente_param);








