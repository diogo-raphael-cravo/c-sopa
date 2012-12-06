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
*/
void descritorProcesso_inicializar(DESCRITOR_PROCESSO *descritorProcesso_param, int PID_param){
	descritorProcesso_param->PID = PID_param;
	descritorProcesso_param->PC = 0;
	int palavra=0;
	for(; palavra<TAMANHO_REGISTRADOR_PALAVRAS; palavra++){
		descritorProcesso_param->registrador.conteudo[palavra] = 0;
	}
}

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo no qual a informação será buscada.
* @return int	PID do processo.
*/
int descritorProcesso_getPID(DESCRITOR_PROCESSO *descritorProcesso_param){
	return descritorProcesso_param->PID;
}

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo no qual a informação será buscada.
* @return int	PC do processo.
*/
int descritorProcesso_getPC(DESCRITOR_PROCESSO *descritorProcesso_param){
	return descritorProcesso_param->PC;
}

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo no qual a informação será buscada.
* @return REGISTRADOR*	Cópia profunda do registrador do processo.
*/
REGISTRADOR* descritorProcesso_getRegistrador(DESCRITOR_PROCESSO *descritorProcesso_param){
	int palavra=0;
	REGISTRADOR* registradorCriado = (REGISTRADOR*) malloc(sizeof(REGISTRADOR*));
	for(; palavra<TAMANHO_REGISTRADOR_PALAVRAS; palavra++){
		registradorCriado->conteudo[palavra] = descritorProcesso_param->registrador->conteudo[palavra];
	}
	return registradorCriado;
}

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo no qual a operação será realizada.
* @param int				PC_param					O PC que o descritor de processo deverá ter.
*/
void descritorProcesso_setPC(DESCRITOR_PROCESSO *descritorProcesso_param, int PC_param){
	descritorProcesso_param->PC = PC_param;
}

/**
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O descritor de processo no qual a operação será realizada.
* @param REGISTRADOR		*registrador_param			Registrador que contém o valor que conterá o registrador 
*														do descritor de processo ao fim da operação.
*/
void descritorProcesso_setRegistrador(DESCRITOR_PROCESSO *descritorProcesso_param, REGISTRADOR *registrador_param){
	int palavra=0;
	for(; palavra<TAMANHO_REGISTRADOR_PALAVRAS; palavra++){
		descritorProcesso_param->registrador->conteudo[palavra] = registrador_param->conteudo[palavra];
	}
}







