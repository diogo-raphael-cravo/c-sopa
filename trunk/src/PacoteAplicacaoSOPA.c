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
* @param int							portaOrigemSOPA_param		A porta do SOPA do qual se origina o pacote.
* @param int							portaDestinoSOPA_param		A porta do SOPA a que se destina o pacote.
* @return PACOTE_APLICACAO_SOPA*	O pacote criado.
*/
PACOTE_APLICACAO_SOPA* pacoteAplicacaoSOPA_criarPacoteRPC(int portaOrigemSOPA_param, int portaDestinoSOPA_param){
	PACOTE_APLICACAO_SOPA_RPC* pacote = (PACOTE_APLICACAO_SOPA_RPC*) malloc(sizeof(PACOTE_APLICACAO_SOPA_RPC));

	return privada_criarPacoteAplicacaoSOPA(TIPO_PACOTE_APLICACAO_SOPA_RPC, portaOrigemSOPA_param, portaDestinoSOPA_param, pacote);
}

/**
* @param PACOTE_APLICACAO_SOPA		*pacote_param		Pacote que será destruído, invalidando o ponteiro passado!
*/
void pacoteAplicacaoSOPA_destruir(PACOTE_APLICACAO_SOPA *pacote_param){

}









