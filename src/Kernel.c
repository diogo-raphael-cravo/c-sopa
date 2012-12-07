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
	int indiceProcesso = (kernel_param->processoRodando+1)%2;
	int PIDProcessoRodando = descritorProcesso_getPID(&kernel_param->descritoresProcessos[kernel_param->processoRodando]);

	privada_pararDeRodarProcesso(kernel_param, kernel_param->processoRodando);
	privada_rodarProcesso(kernel_param, indiceProcesso);
	
	sprintf(mensagem, "CPU esta rodando %d.", PIDProcessoRodando);
	tela_escreverNaColuna(&global_tela, mensagem, 3);
}

/**
* Verifica se o processo está na fila.
* @param KERNEL 						*kernel_param					O kernel em que a operação será realizada.
* @param int[MAXIMO_PROCESSOS_KERNEL]	*fila_param						A fila em que o processo será procurado.
* @param int							indiceDescritorProcesso_param 	O índice do processo procurado em indicesDescritoresProcessosExistentes.
* @return int	1, se encontrar. 0 caso contrário.
*/
int privada_processoEstaNaFila(KERNEL *kernel_param, int fila_param[MAXIMO_PROCESSOS_KERNEL], int indiceDescritorProcesso_param){
	int encontrou=0;
	for(indiceDescritor=0; indiceDescritor<MAXIMO_PROCESSOS_KERNEL; indiceDescritor++){
		if(fila_param[indiceDescritor] == indiceDescritorProcesso_param){
			encontrou = 1;
		}
	}
	return encontrou;
}

/**
* Insere o processo na primeira posição vazia fila dada. O processo não pode ser inserido mais de uma vez na fila.
* @param KERNEL 						*kernel_param					O kernel em que a operação será realizada.
* @param int[MAXIMO_PROCESSOS_KERNEL]	*fila_param						A fila em que o processo será inserido.
* @param int							indiceDescritorProcesso_param 	O índice do processo em indicesDescritoresProcessosExistentes.
*/
void privada_inserirProcessoNaFila(KERNEL *kernel_param, int fila_param[MAXIMO_PROCESSOS_KERNEL], int indiceDescritorProcesso_param){
	int indiceDescritor;
	int indicePrimeiraPosicaoLivre = INDICE_PROCESSO_INEXISTENTE;

	if(!privada_processoEstaNaFila(kernel_param, fila_param, indiceDescritorProcesso_param)){
		for(indiceDescritor=0; indiceDescritor<MAXIMO_PROCESSOS_KERNEL; indiceDescritor++){
			if(fila_param[indiceDescritor] == INDICE_PROCESSO_INEXISTENTE 
					&& indicePrimeiraPosicaoLivre == INDICE_PROCESSO_INEXISTENTE){
				indicePrimeiraPosicaoLivre = indiceDescritor;
			}
		}
		fila_param[indicePrimeiraPosicaoLivre] = indiceDescritorProcesso_param;
	}
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
			if(kernel_param->indicesDescritoresProcessosExistentes[indiceDescritor] == INDICE_PROCESSO_INEXISTENTE){
				indiceLivre = indiceDescritor;
			}
    	}
		kernel_param->indicesDescritoresProcessosExistentes[kernel_param->quantidadeProcessos] = indiceLivre;
		privada_inserirProcessoNaFila(????);

		descritorProcesso_setStatus(&kernel_param->indicesDescritoresProcessosExistentes[kernel_param->quantidadeProcessos],
			STATUS_PROCESSO_PRONTO);
		descritorProcesso_inicializar(&kernel_param->descritoresProcessos[indiceLivre], PID_param);
		descritorProcesso_setPC(&kernel_param->descritoresProcessos[indiceLivre], PC_param);		
		kernel_param->quantidadeProcessos++;
		adicionou = 1;
	} else {
		adicionou = 0;
	}

	return adicionou;
}

/**
* Manda rodar o processo de índice dado.
* @param KERNEL	*kernel_param					O kernel ao qual pertencem o processo.
* @param int	indiceDescritorProcesso_param 	O índice do processo em indicesDescritoresProcessosExistentes.
*/
void privada_rodarProcesso(KERNEL	*kernel_param, int indiceDescritorProcesso_param){
	kernel_param->processoRodando = indiceDescritorProcesso_param;
	descritorProcesso_setStatus(&kernel_param->indicesDescritoresProcessosExistentes[indiceDescritorProcesso_param],
			STATUS_PROCESSO_EXECUTANDO);
}

/**
* Tira o processo que está rodando, deixando no estado pronto.
* @param KERNEL	*kernel_param					O kernel ao qual pertencem o processo.
* @param int	indiceDescritorProcesso_param 	O índice do processo em indicesDescritoresProcessosExistentes.
*/
void privada_pararDeRodarProcesso(KERNEL *kernel_param, int indiceDescritorProcesso_param){
	kernel_param->processoRodando = INDICE_PROCESSO_INEXISTENTE;
	descritorProcesso_setStatus(&kernel_param->indicesDescritoresProcessosExistentes[indiceDescritorProcesso_param],
			STATUS_PROCESSO_PRONTO);
}

/**
* Manda o processo esperar pelo disco.
* @param KERNEL	*kernel_param					O kernel ao qual pertencem o processo.
* @param int	indiceDescritorProcesso_param 	O índice do processo em indicesDescritoresProcessosExistentes.
*/
void privada_mandarProcessoEsperarDisco(KERNEL *kernel_param, int indiceDescritorProcesso_param){
	descritorProcesso_setStatus(&kernel_param->indicesDescritoresProcessosExistentes[indiceDescritorProcesso_param],
			STATUS_PROCESSO_BLOQUEADO);
}

//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/**
* @param KERNEL	*kernel_param	O kernel que será inicializado.
*/
void kernel_inicializar(KERNEL *kernel_param){
	kernel_param->quantidadeProcessos = 0;
    int indiceDescritor=0;
    for(; indiceDescritor<MAXIMO_PROCESSOS_KERNEL; indiceDescritor++){
        kernel_param->indicesDescritoresProcessosExistentes[indiceDescritor] = INDICE_PROCESSO_INEXISTENTE;
    }
	for(; indiceDescritor<MAXIMO_PROCESSOS_KERNEL; indiceDescritor++){
        kernel_param->filaProcessosBloqueados[indiceDescritor] = INDICE_PROCESSO_INEXISTENTE;
    }
	for(; indiceDescritor<MAXIMO_PROCESSOS_KERNEL; indiceDescritor++){
        kernel_param->filaProcessosProntos[indiceDescritor] = INDICE_PROCESSO_INEXISTENTE;
    }

	privada_adicionarProcesso(kernel_param, 456, 0);
	privada_adicionarProcesso(kernel_param, 457, 30);
	privada_rodarProcesso(kernel_param, 0);
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
		case INTERRUPCAO_DISCO:
			break;
		case INTERRUPCAO_SOFTWARE_PARA_DISCO:
			break;
		default: tela_escreverNaColuna(&global_tela, "Interrupcao desconhecida.", 3);
	}
	privada_restauraContextoProcessoAoProcessador(kernel_param, kernel_param->processoRodando);
}
