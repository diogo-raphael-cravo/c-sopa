#include "../include/DadosComuns.h"

/**
* Variáveis globais acessíveis somente neste arquivo.
*/



//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------
/**
* Salva contexto do processo fornecido.
* @param KERNEL	*kernel_param			O kernel em que a operação será realizada.
* @param int	indiceProcesso_param	Índice em descritoresProcessos do processo em cujo contexto será salvo o contexto da CPU.
*/
void privada_salvarContextoProcessadorNoProcesso(KERNEL *kernel_param, int indiceProcesso_param){
	descritorProcesso_setPC(&kernel_param->descritoresProcessos[indiceProcesso_param], processador_getPC(&global_processador));
	descritorProcesso_setRegistrador(&kernel_param->descritoresProcessos[indiceProcesso_param], 
									processador_getRegistrador(&global_processador));
}

/**
* Restaura o contexto do processo fornecido no processador.
* @param KERNEL	*kernel_param			O kernel em que a operação será realizada.
* @param int	indiceProcesso_param	Índice em descritoresProcessos do processo cujo contexto será restaurado ao processador.
*/
void privada_restauraContextoProcessoAoProcessador(KERNEL *kernel_param, int indiceProcesso_param){
	processador_setPC(&global_processador, descritorProcesso_getPC(&kernel_param->descritoresProcessos[indiceProcesso_param]));
	processador_setRegistrador(&global_processador, 
		descritorProcesso_getRegistrador(&kernel_param->descritoresProcessos[indiceProcesso_param]));
}

/**
* Roda o escalonador, mandando rodar o processo que merecer.
* @param KERNEL	*kernel_param 	O kernel em que a operação será realizada.
*/
void privada_escalonar(KERNEL *kernel_param){
	char mensagem[100];
	kernel_param->processoRodando = (kernel_param->processoRodando+1)%2;
	int PIDProcessoRodando = descritorProcesso_getPID(&kernel_param->descritoresProcessos[kernel_param->processoRodando]);
	sprintf(mensagem, "CPU esta rodando %d.", PIDProcessoRodando);
	tela_escreverNaColuna(&global_tela, mensagem, 3);
}

/**
* Adiciona um processo a este kernel.
* @param KERNEL 				*kernel_param		O kernel que receberá o processo.
* @param int					PID_param			PID do descritor do processo que será adicionado.
* @param int					PC_param			PC do descritor do processo que será adicionado.
* @return int	Indica se conseguiu adicionar o processo.
*/
int privada_adicionarProcesso(KERNEL *kernel_param, int PID_param, int PC_param){
	int indiceDescritor = 0;
	int indiceLivre = INDICE_PROCESSO_INEXISTENTE;
	int adicionou = 0;

	if(kernel_param->quantidadeProcessos < MAXIMO_PROCESSOS_KERNEL){
	    for(; indiceDescritor<MAXIMO_PROCESSOS_KERNEL; indiceDescritor++){
			if(kernel_param->indicesDescritoresProcessosOcupados[indiceDescritor] == INDICE_PROCESSO_INEXISTENTE){
				indiceLivre = indiceDescritor;
			}
    	}
		kernel_param->indicesDescritoresProcessosOcupados[kernel_param->quantidadeProcessos] = indiceLivre;
		descritorProcesso_inicializar(&kernel_param->descritoresProcessos[indiceLivre], PID_param);
		descritorProcesso_setPC(&kernel_param->descritoresProcessos[indiceLivre], PC_param);		
		kernel_param->quantidadeProcessos++;
		adicionou = 1;
	} else {
		adicionou = 0;
	}

	return adicionou;
}

//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/**
* @param KERNEL	*kernel_param	O kernel que será inicializado.
*/
void kernel_inicializar(KERNEL *kernel_param){
	kernel_param->processoRodando = 0;
	kernel_param->quantidadeProcessos = 0;
    int indiceDescritor=0;
    for(; indiceDescritor<MAXIMO_PROCESSOS_KERNEL; indiceDescritor++){
        kernel_param->indicesDescritoresProcessosOcupados[indiceDescritor] = INDICE_PROCESSO_INEXISTENTE;
    }

	privada_adicionarProcesso(kernel_param, 456, 0);
	privada_adicionarProcesso(kernel_param, 457, 30);
}

/**
* @param KERNEL			*kernel_param		O kernel que irá rodar.
* @param INTERRUPCAO	interrupcao_param	A interrupção que definirá o comportamento do kernel.
*/
void kernel_rodar(KERNEL *kernel_param, INTERRUPCAO interrupcao_param){
	char mensagem[100];
	sprintf(mensagem, "Kernel chamado para a interrupcao %d.", interrupcao_param);
	tela_escreverNaColuna(&global_tela, mensagem, 3);

	privada_salvarContextoProcessadorNoProcesso(kernel_param, kernel_param->processoRodando);
	switch(interrupcao_param){
		case INTERRUPCAO_TIMER: privada_escalonar(kernel_param);
			break;
		default: tela_escreverNaColuna(&global_tela, "Interrupcao desconhecida.", 3);
	}
	privada_restauraContextoProcessoAoProcessador(kernel_param, kernel_param->processoRodando);
}
