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
* Inicializa o contexto.
* @param CONTEXTO	*contexto_param	O contexto que será inicializado.
*/
void contexto_inicializar(CONTEXTO *contexto_param){
	int registrador=0;
	contexto_param->PC = 0;
	for(; registrador<QUANTIDADE_REGISTRADORES_CONTEXTO; registrador++){
		registrador_inicializar(&contexto_param->registradores[registrador]);
	}
}

/**
* @param CONTEXTO	*destino_param	O contexto para onde o segundo será copiado.
* @param CONTEXTO	*origem_param	O contexto do qual a cópia será feita.
*/
void contexto_copiar(CONTEXTO *destino_param, CONTEXTO *origem_param){
	destino_param->PC = origem_param->PC;
	int registrador=0;
	for(; registrador<QUANTIDADE_REGISTRADORES_CONTEXTO; registrador++){
		registrador_copiar(&destino_param->registradores[registrador], &origem_param->registradores[registrador]);
	}
}

/**
* @param CONTEXTO	*contexto_param		O contexto no qual a informação será buscada.
* @return int	PC do contexto.
*/
int contexto_getPC(CONTEXTO *contexto_param){
	return contexto_param->PC;
}

/**
* @param CONTEXTO	*contexto_param			O contexto no qual a informação será buscada.
* @param int		indiceRegistrador_param	O índice em contexto_param->registradores do registrador que se quer.
* @return REGISTRADOR*	O registrador do contexto.
*/
REGISTRADOR* contexto_getRegistrador(CONTEXTO *contexto_param, int indiceRegistrador_param){
	return &contexto_param->registradores[indiceRegistrador_param];
}

/**
* @param CONTEXTO			*contexto_param	O contexto no qual a operação será realizada.
* @param int				PC_param		O PC que o contexto deverá ter.
*/
void contexto_setPC(CONTEXTO *contexto_param, int PC_param){
	contexto_param->PC = PC_param;
}

/**
* @param CONTEXTO		*contexto_param			O contexto no qual a operação será realizada.
* @param REGISTRADOR	*registrador_param		Registrador que contém o valor que conterá o registrador 
*												do contexto ao fim da operação.
* @param int			indiceRegistrador_param	O índice em contexto_param->registradores do registrador que se quer.
*/
void contexto_setRegistrador(CONTEXTO *contexto_param, REGISTRADOR *registrador_param, int indiceRegistrador_param){
	registrador_copiar(&contexto_param->registradores[indiceRegistrador_param], registrador_param);
}

/**
* @param CONTEXTO		*contexto_param			O contexto no qual a operação será realizada.
* @param PALAVRA		*palavra_param			Palavra que contém o valor que conterá o registrador 
*												do contexto ao fim da operação.
* @param int			indiceRegistrador_param	O índice em contexto_param->registradores do registrador que se quer.
*/
void contexto_setRegistradorPalavra(CONTEXTO *contexto_param, PALAVRA palavra_param, int indiceRegistrador_param){
	registrador_carregarPalavra(&contexto_param->registradores[indiceRegistrador_param], palavra_param);
}

/**
* Imprime os registradores deste contexto na coluna fornecida da tela.
* @param CONTEXTO	*contexto_param	O contexto cujos registradores serão impressos.
* @param int		coluna_param	A coluna da tela em que a impressão será feita.
*/
void contexto_imprimirRegistradores(CONTEXTO *contexto_param, int coluna_param){
	char mensagem[200];
	int registrador;

	sprintf(mensagem, "*(PC)=");
	tela_escreverNaColuna(&global_tela, mensagem, coluna_param);
	sprintf(mensagem, " =%d", contexto_param->PC);
	tela_escreverNaColuna(&global_tela, mensagem, coluna_param);
	
	for(registrador=0; registrador<QUANTIDADE_REGISTRADORES_CONTEXTO; registrador++){
		sprintf(mensagem, "*(r%d)=", registrador);
		tela_escreverNaColuna(&global_tela, mensagem, coluna_param);
		registrador_imprimir(&contexto_param->registradores[registrador], coluna_param);
	}
}


