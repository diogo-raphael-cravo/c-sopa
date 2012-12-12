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
* @param int				tamanhoAreaMemoria_param	Tamanho da área de memória do processo, em palavras.
*/
void descritorProcesso_inicializar(DESCRITOR_PROCESSO *descritorProcesso_param, int PID_param, int enderecoInicio_param, int tamanhoAreaMemoria_param){
	descritorProcesso_param->PID = PID_param;
	descritorProcesso_param->enderecoInicio = enderecoInicio_param;
	descritorProcesso_param->tamanhoAreaMemoriaPalavras = tamanhoAreaMemoria_param;
}

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo no qual a informação será buscada.
* @return int	PID do processo.
*/
int descritorProcesso_getPID(DESCRITOR_PROCESSO *descritorProcesso_param){
	return descritorProcesso_param->PID;
}

/**
* @param DESCRITOR_PROCESSO			*descritorProcesso_param	O descritor de processo no qual a operação será realizada.
* @param STATUS_DESCRITOR_PROCESSO	status_param				O status deste processo. Deve ser algum dos definidos no início deste arquivo.
*/
void descritorProcesso_setStatus(DESCRITOR_PROCESSO *descritorProcesso_param, STATUS_DESCRITOR_PROCESSO status_param){
	descritorProcesso_param->status = status_param;
}

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo no qual a informação será buscada.
* @return STATUS_DESCRITOR_PROCESSO	O status deste processo. Deve ser algum dos definidos no início deste arquivo.
*/
STATUS_DESCRITOR_PROCESSO descritorProcesso_getStatus(DESCRITOR_PROCESSO *descritorProcesso_param){
	return descritorProcesso_param->status;
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
* @return int	O tamanho da área de memória reservada ao processo, em palavras.
*/
int descritorProcesso_getTamanhoAreaMemoriaPalavras(DESCRITOR_PROCESSO *descritorProcesso_param){
	return descritorProcesso_param->tamanhoAreaMemoriaPalavras;
}






