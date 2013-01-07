/**
* Concentra as funções de manipulação de RPCs.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define PACOTE_APLICACAO_SEPARADOR 
#define TAMANHO_PACOTE_STRING TAMANHOBUFFER
#define PORTA_QUALQUER -1

enum enum_tipoPacoteAplicacaoSOPA{
	TIPO_PACOTE_APLICACAO_SOPA_RPC
};

typedef enum enum_tipoPacoteAplicacaoSOPA TIPO_PACOTE_APLICACAO_SOPA;

struct str_pacoteAplicacaoSOPA{
	TIPO_PACOTE_APLICACAO_SOPA tipo; //Indica o tipo do pacote que será enviado, se é um RPC, timer, etc.
	int portaOrigemSOPA; //PID do processo no SOPA do qual o pacote se origina.
	int portaDestinoSOPA; //PID do processo no SOPA ao qual o pacote se destina.
	void* conteudo; //O pacote propriamente dito. Note que há pacotes diferentes para RPC, timer, etc.
	char* ipOrigem; //IP no formato "127.0.0.1". NENHUM campo deve começar em 0 (algo do tipo "127.0.0.01" causará erro).
};

typedef struct str_pacoteAplicacaoSOPA PACOTE_APLICACAO_SOPA;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* @param OPERACAO_RPC		operacao_param				A operação RPC que será realizada.
* @param FIFO*				parametros_param			Os parâmetros da operação RPC.
* @param int				portaOrigemSOPA_param		A porta do SOPA do qual se origina o pacote.
* @param int				portaDestinoSOPA_param		A porta do SOPA a que se destina o pacote.
* @return PACOTE_APLICACAO_SOPA*	O pacote criado.
*/
PACOTE_APLICACAO_SOPA* pacoteAplicacaoSOPA_criarPacoteRPC(OPERACAO_RPC operacao_param, FIFO* parametros_param,
	int portaOrigemSOPA_param, int portaDestinoSOPA_param);

/**
* @param PACOTE_APLICACAO_SOPA		*pacote_param		O pacote que será transformado em string.
* @return char*		String que representa o pacote.
* @see pacoteAplicacaoSOPA_deString 	Processo reverso.
* ATENÇÃO: o ipOrigem só é preenchido quando a mensagem é RECEBIDA!
*/
char* pacoteAplicacaoSOPA_paraString(PACOTE_APLICACAO_SOPA *pacote_param);

/**
* @param char*		string_param		String que representa o pacote.
* @return PACOTE_APLICACAO_SOPA*		O pacote que será criado à partir da string.
* @see pacoteAplicacaoSOPA_paraString 	Processo reverso.
*/
PACOTE_APLICACAO_SOPA* pacoteAplicacaoSOPA_deString(char* string_param);

/**
* @param PACOTE_APLICACAO_SOPA		*pacote_param		Pacote que será destruído, invalidando o ponteiro passado!
*/
void pacoteAplicacaoSOPA_destruir(PACOTE_APLICACAO_SOPA *pacote_param);

/**
* @param PACOTE_APLICACAO_SOPA		*pacote_param		Pacote que será consultado.
* @return TIPO_PACOTE_APLICACAO_SOPA	O tipo do pacote, o que pode ser encontrado nele.
*/
TIPO_PACOTE_APLICACAO_SOPA pacoteAplicacaoSOPA_getTipo(PACOTE_APLICACAO_SOPA *pacote_param);

/**
* @param PACOTE_APLICACAO_SOPA		*pacote_param		Pacote que será consultado.
* @return void*		Conteúdo do pacote, a mensagem mesmo.
*/
void* pacoteAplicacaoSOPA_getConteudo(PACOTE_APLICACAO_SOPA *pacote_param);

/**
* @param PACOTE_APLICACAO_SOPA		*pacote_param		Pacote que será consultado.
* @return int		Porta de origem (PID do processo do SOPA que requisitou o pacote).
*/
int pacoteAplicacaoSOPA_getPortaOrigem(PACOTE_APLICACAO_SOPA *pacote_param);

/**
* @param PACOTE_APLICACAO_SOPA		*pacote_param		Pacote que será consultado.
* @return int		Porta de desstino (PID do processo do SOPA a que se destina o pacote).
*/
int pacoteAplicacaoSOPA_getPortaDestino(PACOTE_APLICACAO_SOPA *pacote_param);

/**
* @param PACOTE_APLICACAO_SOPA		*pacote_param		Pacote que será consultado.
* @return PALAVRA		IP do hospedeiro de que se origina o pacote.
*/
PALAVRA pacoteAplicacaoSOPA_getPalavraOrigemIP(PACOTE_APLICACAO_SOPA *pacote_param);





