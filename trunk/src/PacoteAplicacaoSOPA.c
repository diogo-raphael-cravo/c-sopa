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
	sprintf(stringPacote, "%d%c%d%c%d%c", 
		pacote_param->tipo,
		SOCKET_SOPA_SEPARADOR,
		pacote_param->portaOrigemSOPA,
		SOCKET_SOPA_SEPARADOR,
		pacote_param->portaDestinoSOPA,
		SOCKET_SOPA_SEPARADOR);
	char* stringConteudo;
	switch(pacote_param->tipo){
		case TIPO_PACOTE_APLICACAO_SOPA_RPC:
			stringConteudo = rpc_paraString((RPC*) pacote_param->conteudo);
			strcat(stringPacote, stringConteudo);
			free(stringConteudo);
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
	TIPO_PACOTE_APLICACAO_SOPA tipo = atoi(string_pegarSubtexto(string_param, SOCKET_SOPA_SEPARADOR_STRING, 0));
	int portaOrigemSOPA = atoi(string_pegarSubtexto(string_param, SOCKET_SOPA_SEPARADOR_STRING, 1));
	int portaDestinoSOPA = atoi(string_pegarSubtexto(string_param, SOCKET_SOPA_SEPARADOR_STRING, 2));

	char* stringConteudo = string_pegarSubtextoEntreSubtextos(string_param, SOCKET_SOPA_SEPARADOR_STRING, 3, 1);
	void* conteudo; 
	switch(tipo){
		case TIPO_PACOTE_APLICACAO_SOPA_RPC:
			conteudo = rpc_deString(stringConteudo);
			break;
	}

	PACOTE_APLICACAO_SOPA *pacote = privada_criarPacoteAplicacaoSOPA(tipo, portaOrigemSOPA, portaDestinoSOPA, conteudo);
	pacote->ipOrigem = (char*) malloc((3*5+1)*sizeof(char));
	strcpy(pacote->ipOrigem, string_pegarSubtexto(string_param, SOCKET_SOPA_SEPARADOR_STRING, -1));
	return pacote;
}

/**
* @param PACOTE_APLICACAO_SOPA		*pacote_param		Pacote que será destruído, invalidando o ponteiro passado!
*/
void pacoteAplicacaoSOPA_destruir(PACOTE_APLICACAO_SOPA *pacote_param){

}

/**
* @param PACOTE_APLICACAO_SOPA		*pacote_param		Pacote que será consultado.
* @return TIPO_PACOTE_APLICACAO_SOPA	O tipo do pacote, o que pode ser encontrado nele.
*/
TIPO_PACOTE_APLICACAO_SOPA pacoteAplicacaoSOPA_getTipo(PACOTE_APLICACAO_SOPA *pacote_param){
	return pacote_param->tipo;
}

/**
* @param PACOTE_APLICACAO_SOPA		*pacote_param		Pacote que será consultado.
* @return void*		Conteúdo do pacote, a mensagem mesmo.
*/
void* pacoteAplicacaoSOPA_getConteudo(PACOTE_APLICACAO_SOPA *pacote_param){
	return pacote_param->conteudo;
}


/**
* @param PACOTE_APLICACAO_SOPA		*pacote_param		Pacote que será consultado.
* @return int		Porta de origem (PID do processo do SOPA que requisitou o pacote).
*/
int pacoteAplicacaoSOPA_getPortaOrigem(PACOTE_APLICACAO_SOPA *pacote_param){
	return pacote_param->portaOrigemSOPA;
}

/**
* @param PACOTE_APLICACAO_SOPA		*pacote_param		Pacote que será consultado.
* @return int		Porta de desstino (PID do processo do SOPA a que se destina o pacote).
*/
int pacoteAplicacaoSOPA_getPortaDestino(PACOTE_APLICACAO_SOPA *pacote_param){
	return pacote_param->portaDestinoSOPA;
}

/**
* @param PACOTE_APLICACAO_SOPA		*pacote_param		Pacote que será consultado.
* @return PALAVRA		IP do hospedeiro de que se origina o pacote.
*/
PALAVRA pacoteAplicacaoSOPA_getPalavraOrigemIP(PACOTE_APLICACAO_SOPA *pacote_param){
	PALAVRA ip = 0;

tela_escreverNaColuna(&global_tela, "GetOrigem", 5);
sincronizadorGlobal_sincronizado_pausar();
	char* tokenIp = (char*) malloc(200*sizeof(char));
tela_escreverNaColuna(&global_tela, tokenIp, 5);
	tokenIp = strtok(pacote_param->ipOrigem, ".");
tela_escreverNaColuna(&global_tela, tokenIp, 5);
	ip = ip | (atoi(tokenIp)*256*256*256 & 0xFF000000);
	tokenIp = strtok(NULL, ".");
tela_escreverNaColuna(&global_tela, tokenIp, 5);
	ip = ip | (atoi(tokenIp)*256*256 & 0x00FF0000);
	tokenIp = strtok(NULL, ".");
tela_escreverNaColuna(&global_tela, tokenIp, 5);
	ip = ip | (atoi(tokenIp)*256 & 0x0000FF00);
	tokenIp = strtok(NULL, ".");
tela_escreverNaColuna(&global_tela, tokenIp, 5);
	ip = ip | (atoi(tokenIp) & 0x000000FF);
	free(tokenIp);

	return ip;
}




