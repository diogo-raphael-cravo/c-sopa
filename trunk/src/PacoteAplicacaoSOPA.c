#include "../include/DadosComuns.h"

/**
* Variáveis globais acessíveis somente neste arquivo.
*/



//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------
/**
* @param TIPO_PACOTE_APLICACAO_SOPA		tipo_param					O tipo do pacote criado.
* @param int							portaOrigemSOPA_param		A porta do SOPA do qual se origina o pacote.
* @param int							portaDestinoSOPA_param		A porta do SOPA a que se destina o pacote.
* @param void*							conteudo_param				O pacote propriamente dito.
* @return 
*/
PACOTE_APLICACAO_SOPA* privada_criarPacoteAplicacaoSOPA(TIPO_PACOTE_APLICACAO_SOPA tipo_param,
		int portaOrigemSOPA_param, int portaDestinoSOPA_param, void* conteudo_param){
	PACOTE_APLICACAO_SOPA *novoPacote = (PACOTE_APLICACAO_SOPA*) malloc(sizeof(PACOTE_APLICACAO_SOPA));

	novoPacote->tipo = tipo_param;
	novoPacote->portaOrigemSOPA = portaOrigemSOPA_param;
	novoPacote->portaDestinoSOPA = portaDestinoSOPA_param;
	novoPacote->conteudo = conteudo_param;

	return novoPacote;
}

//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/**
* @param OPERACAO_RPC		operacao_param				A operação RPC que será realizada.
* @param FIFO*				parametros_param			Os parâmetros da operação RPC.
* @param int				portaOrigemSOPA_param		A porta do SOPA do qual se origina o pacote.
* @param int				portaDestinoSOPA_param		A porta do SOPA a que se destina o pacote.
* @return PACOTE_APLICACAO_SOPA*	O pacote criado.
*/
PACOTE_APLICACAO_SOPA* pacoteAplicacaoSOPA_criarPacoteRPC(OPERACAO_RPC operacao_param, FIFO* parametros_param,
		int portaOrigemSOPA_param, int portaDestinoSOPA_param){
	return privada_criarPacoteAplicacaoSOPA(
				TIPO_PACOTE_APLICACAO_SOPA_RPC, 
				portaOrigemSOPA_param, portaDestinoSOPA_param, 
				rpc_criarNovo(operacao_param, parametros_param));
}

/**
* @param PACOTE_APLICACAO_SOPA		*pacote_param		O pacote que será transformado em string.
* @return char*		String que representa o pacote.
* @see pacoteAplicacaoSOPA_deString 	Processo reverso.
* ATENÇÃO: o ipOrigem só é preenchido quando a mensagem é RECEBIDA!
*/
char* pacoteAplicacaoSOPA_paraString(PACOTE_APLICACAO_SOPA *pacote_param){
	char* stringPacote = (char*) malloc(TAMANHO_PACOTE_STRING*sizeof(char));
	memset(stringPacote, '\0', TAMANHO_PACOTE_STRING);
	sprintf(stringPacote, "%d\n%d\n%d\n", 
		pacote_param->tipo,
		pacote_param->portaOrigemSOPA,
		pacote_param->portaDestinoSOPA);
	switch(pacote_param->tipo){
		case TIPO_PACOTE_APLICACAO_SOPA_RPC:
			strcat(stringPacote, rpc_paraString((RPC*) pacote_param->conteudo));
			break;
	}
	return stringPacote;
}

/**
* @param char*		string_param		String que representa o pacote.
* @return PACOTE_APLICACAO_SOPA*		O pacote que será criado à partir da string.
* @see pacoteAplicacaoSOPA_paraString 	Processo reverso.
*/
PACOTE_APLICACAO_SOPA* pacoteAplicacaoSOPA_deString(char* string_param){
	char* token = strtok(string_param, "\n");

	TIPO_PACOTE_APLICACAO_SOPA tipo = atoi(token);
	token = strtok(NULL, "\n");
	int portaOrigemSOPA = atoi(token);
	token = strtok(NULL, "\n");
	int portaDestinoSOPA = atoi(token);
	token = strtok(NULL, "\n");
	void* conteudo; 
	switch(tipo){
		case TIPO_PACOTE_APLICACAO_SOPA_RPC:
			conteudo = rpc_deString(token);
			break;
	}
	char* ipOrigem = strtok(NULL, "\n");

	PACOTE_APLICACAO_SOPA *pacote = pacoteAplicacaoSOPA_criarPacoteRPC(tipo, conteudo, portaOrigemSOPA, portaDestinoSOPA);
	pacote->ipOrigem = ipOrigem;
	return pacote;
}

/**
* @param PACOTE_APLICACAO_SOPA		*pacote_param		Pacote que será destruído, invalidando o ponteiro passado!
*/
void pacoteAplicacaoSOPA_destruir(PACOTE_APLICACAO_SOPA *pacote_param){

}









