/**
* Concentra as funções de manipulação de RPCs.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes

enum enum_tipoPacoteAplicacaoSOPA{
	TIPO_PACOTE_APLICACAO_SOPA_RPC
};

typedef enum enum_tipoPacoteAplicacaoSOPA TIPO_PACOTE_APLICACAO_SOPA;

struct str_pacoteRPC{
	
};

typedef struct str_pacoteRPC PACOTE_APLICACAO_SOPA_RPC;

struct str_pacoteAplicacaoSOPA{
	TIPO_PACOTE_APLICACAO_SOPA tipo; //Indica o tipo do pacote que será enviado, se é um RPC, timer, etc.
	int portaOrigemSOPA; //PID do processo no SOPA do qual o pacote se origina.
	int portaDestinoSOPA; //PID do processo no SOPA ao qual o pacote se destina.
	void* conteudo; //O pacote propriamente dito. Note que há pacotes diferentes para RPC, timer, etc.
};

typedef struct str_pacoteAplicacaoSOPA PACOTE_APLICACAO_SOPA;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* @param int		portaOrigemSOPA_param		A porta do SOPA do qual se origina o pacote.
* @param int		portaDestinoSOPA_param		A porta do SOPA a que se destina o pacote.
* @return PACOTE_APLICACAO_SOPA*	O pacote criado.
*/
PACOTE_APLICACAO_SOPA* pacoteAplicacaoSOPA_criarPacoteRPC(int portaOrigemSOPA_param, int portaDestinoSOPA_param);

/**
* @param PACOTE_APLICACAO_SOPA		*pacote_param		O pacote que será transformado em string.
* @return char*		String que representa o pacote.
* @see pacoteAplicacaoSOPA_deString 	Processo reverso.
*/
char* pacoteAplicacaoSOPA_paraString(PACOTE_APLICACAO_SOPA *pacote_param);

/**
* @param char*		string_param		String que representa o pacote.
* @return PACOTE_APLICACAO_SOPA		O pacote que será criado à partir da string.
* @see pacoteAplicacaoSOPA_paraString 	Processo reverso.
*/
PACOTE_APLICACAO_SOPA* pacoteAplicacaoSOPA_deString(char* string_param);

/**
* @param PACOTE_APLICACAO_SOPA		*pacote_param		Pacote que será destruído, invalidando o ponteiro passado!
*/
void pacoteAplicacaoSOPA_destruir(PACOTE_APLICACAO_SOPA *pacote_param);










