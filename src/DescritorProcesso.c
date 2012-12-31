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
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo que será inicializado.
* @param int				PID_param					A PID deste descritor de processo.
* @param int				enderecoInicio_param		Endereço, na memória, do início do processo.
* @param int				tamanhoAreaCodigo_param		Tamanho da área de código do processo, em palavras.
* @param int				tamanhoStack_param			O tamanho da stack do processo, em palavras.
*/
void descritorProcesso_inicializar(DESCRITOR_PROCESSO *descritorProcesso_param, int PID_param, int enderecoInicio_param, 
	int tamanhoAreaCodigo_param, int tamanhoStack_param){
	descritorProcesso_param->PID = PID_param;
	descritorProcesso_param->haQuantosTicksRoda = 0;
	descritorProcesso_param->enderecoInicio = enderecoInicio_param;
	descritorProcesso_param->tamanhoAreaCodigoPalavras = tamanhoAreaCodigo_param; 
		//1, pois a primeira palavra da stack guarda o tamanho dela
	descritorProcesso_param->tamanhoStack = tamanhoStack_param+1;
}

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo no qual a informação será buscada.
* @return int	PID do processo.
*/
int descritorProcesso_getPID(DESCRITOR_PROCESSO *descritorProcesso_param){
	return descritorProcesso_param->PID;
}

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo no qual a informação será salva.
* @param CONTEXTO			*contexto_param				O contexto que o processo terá.
*/
void descritorProcesso_setContexto(DESCRITOR_PROCESSO *descritorProcesso_param, CONTEXTO *contexto_param){
	contexto_copiar(&descritorProcesso_param->contextoProcesso, contexto_param);
}

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo do qual a informação será recuperada.
* @return CONTEXTO*	O contexto do processo.
*/
CONTEXTO* descritorProcesso_getContexto(DESCRITOR_PROCESSO *descritorProcesso_param){
	return &descritorProcesso_param->contextoProcesso;
}

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo do qual a informação será recuperada.
* @return int	O endereço de início do processo na memória.
*/
int descritorProcesso_getEnderecoInicio(DESCRITOR_PROCESSO *descritorProcesso_param){
	return descritorProcesso_param->enderecoInicio;
}

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo do qual a informação será recuperada.
* @return int	O endereço de início do código deste processo.
*/
int descritorProcesso_getEnderecoInicioCodigo(DESCRITOR_PROCESSO *descritorProcesso_param){
	return descritorProcesso_param->enderecoInicio+descritorProcesso_param->tamanhoStack;
}

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo do qual a informação será recuperada.
* @return int	O tamanho da área de código reservada ao processo, em palavras.
*/
int descritorProcesso_getTamanhoAreaCodigoPalavras(DESCRITOR_PROCESSO *descritorProcesso_param){
	return descritorProcesso_param->tamanhoAreaCodigoPalavras;
}

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo do qual a informação será modificada.
* @param int				ticks_param					A quantidade de ticks há que o processo está rodando.
*/
void descritorProcesso_setFatiaTempo(DESCRITOR_PROCESSO *descritorProcesso_param, int ticks_param){
	descritorProcesso_param->haQuantosTicksRoda = ticks_param;
}

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo do qual a informação será modificada.
* @return int	A quantidade de ticks há que o processo está rodando.
*/
int descritorProcesso_getFatiaTempo(DESCRITOR_PROCESSO *descritorProcesso_param){
	return descritorProcesso_param->haQuantosTicksRoda;
}

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo do qual a informação será modificada.
* @return int	O tamanho da stack do processo, em palavras.
*/
int descritorProcesso_getTamanhoStackPalavras(DESCRITOR_PROCESSO *descritorProcesso_param){
	return descritorProcesso_param->tamanhoStack;
}

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo cuja stack será inicializada.
* ATENÇÃO: de uso estrito do kernel!
*/
void descritorProcesso_inicializarStack(DESCRITOR_PROCESSO *descritorProcesso_param){
	contexto_setRegistradorPalavra(descritorProcesso_getContexto(descritorProcesso_param), 0, REGISTRADOR_STACK_POINTER);
}



