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
* @param DESCRITOR_PROCESSO	*descritorProcesso_param 	O processo no qual o contexto do processador será salvo.
*/
void privada_salvarContextoProcessadorNoProcesso(KERNEL *kernel_param, DESCRITOR_PROCESSO *descritorProcesso_param){
	descritorProcesso_setPC(descritorProcesso_param, processador_getPC(&global_processador));
	descritorProcesso_setRegistrador(descritorProcesso_param, processador_getRegistrador(&global_processador));
}

/**
* Restaura o contexto do processo fornecido no processador.
* @param KERNEL				*kernel_param				O kernel em que a operação será realizada.
* @param DESCRITOR_PROCESSO	*descritorProcesso_param 	O processo cujo contexto será colocado no processador.
*/
void privada_restauraContextoProcessoAoProcessador(KERNEL *kernel_param, DESCRITOR_PROCESSO *descritorProcesso_param){
	processador_setPC(&global_processador, descritorProcesso_getPC(descritorProcesso_param));
	processador_setRegistrador(&global_processador, descritorProcesso_getRegistrador(descritorProcesso_param));
}

/**
* Manda rodar o processo de índice dado.
* @param KERNEL				*kernel_param				O kernel ao qual pertencem o processo.
* @param DESCRITOR_PROCESSO	*descritorProcesso_param 	O processo que irá rodar.
*/
void privada_rodarProcesso(KERNEL *kernel_param, DESCRITOR_PROCESSO **descritorProcesso_param){
	kernel_param->processoRodando = descritorProcesso_param;
	descritorProcesso_setStatus(*descritorProcesso_param, STATUS_PROCESSO_EXECUTANDO);
}

/**
* Tira o processo que está rodando, deixando no estado pronto.
* @param KERNEL					*kernel_param				O kernel ao qual pertencem o processo.
*/
void privada_pararDeRodarProcessoRodando(KERNEL *kernel_param){
	descritorProcesso_setStatus(*kernel_param->processoRodando, STATUS_PROCESSO_PRONTO);
	FIFO_inserir(&kernel_param->filaProcessosProntos, kernel_param->processoRodando);
	kernel_param->processoRodando = DESCRITOR_PROCESSO_INEXISTENTE;
}

/**
* Manda o processo esperar pelo disco.
* @param KERNEL					*kernel_param				O kernel ao qual pertencem o processo.
*/
void privada_mandarProcessoRodandoEsperarDisco(KERNEL *kernel_param){
	descritorProcesso_setStatus(*kernel_param->processoRodando, STATUS_PROCESSO_BLOQUEADO);
	FIFO_inserir(&kernel_param->filaProcessosBloqueados, kernel_param->processoRodando);
	kernel_param->processoRodando = DESCRITOR_PROCESSO_INEXISTENTE;
}

/**
* Roda o escalonador, mandando rodar o processo que merecer.
* @param KERNEL	*kernel_param 	O kernel em que a operação será realizada.
*/
void privada_escalonar(KERNEL *kernel_param){
	char mensagem[100];

	if(!FIFO_vazia(&kernel_param->filaProcessosProntos)){
		sprintf(mensagem, "CPU estah rodando %d.", 
			descritorProcesso_getPID(* (DESCRITOR_PROCESSO**) FIFO_espiar(&kernel_param->filaProcessosProntos)));
		tela_escreverNaColuna(&global_tela, mensagem, 3);

		if(kernel_param->processoRodando != DESCRITOR_PROCESSO_INEXISTENTE){
			privada_pararDeRodarProcessoRodando(kernel_param);
		}
		privada_rodarProcesso(kernel_param, (DESCRITOR_PROCESSO**) FIFO_remover(&kernel_param->filaProcessosProntos));
	} else {
		tela_escreverNaColuna(&global_tela, "Nao ha processo elegivel para ser executado. Vou continuar executando o que estava.", 3);
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
	int adicionou = 0;
	DESCRITOR_PROCESSO **novoProcesso = (DESCRITOR_PROCESSO**) malloc(sizeof(DESCRITOR_PROCESSO*));
	*novoProcesso = (DESCRITOR_PROCESSO*) malloc(sizeof(DESCRITOR_PROCESSO));
	descritorProcesso_inicializar(*novoProcesso, PID_param);
	descritorProcesso_setPC(*novoProcesso, PC_param);
	descritorProcesso_setStatus(*novoProcesso, STATUS_PROCESSO_PRONTO);
	if(kernel_param->quantidadeProcessos < MAXIMO_PROCESSOS_KERNEL){
		FIFO_inserir(&kernel_param->filaProcessosProntos, novoProcesso);
		kernel_param->quantidadeProcessos++;
		adicionou = 1;
	} else {
		free(novoProcesso);
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
	kernel_param->quantidadeProcessos = 0;
	FIFO_inicializar(&kernel_param->filaProcessosProntos, MAXIMO_PROCESSOS_KERNEL);
	FIFO_inicializar(&kernel_param->filaProcessosBloqueados, MAXIMO_PROCESSOS_KERNEL);

	privada_adicionarProcesso(kernel_param, 456, 0);
	privada_adicionarProcesso(kernel_param, 457, 30);
	privada_rodarProcesso(kernel_param, (DESCRITOR_PROCESSO**) FIFO_remover(&kernel_param->filaProcessosProntos));
}

/**
* @param KERNEL			*kernel_param		O kernel que irá rodar.
* @param INTERRUPCAO	interrupcao_param	A interrupção que definirá o comportamento do kernel.
*/
void kernel_rodar(KERNEL *kernel_param, INTERRUPCAO interrupcao_param){
	char mensagem[100];
	sprintf(mensagem, "Kernel chamado para a interrupcao %d.", interrupcao_param);
	tela_escreverNaColuna(&global_tela, mensagem, 3);

	privada_salvarContextoProcessadorNoProcesso(kernel_param, *kernel_param->processoRodando);
	switch(interrupcao_param){
		case INTERRUPCAO_CONSOLE:
			tela_escreverNaColuna(&global_tela, console_ultimaLinhaDigitada(&global_console), 5);
			break;
		case INTERRUPCAO_TIMER:
			privada_escalonar(kernel_param);
			break;
		case INTERRUPCAO_DISCO:
			FIFO_inserir(&kernel_param->filaProcessosProntos, FIFO_remover(&kernel_param->filaProcessosBloqueados));
			break;
		case INTERRUPCAO_SOFTWARE_PARA_DISCO:
			privada_mandarProcessoRodandoEsperarDisco(kernel_param);
			privada_escalonar(kernel_param);
			break;
		default:
			tela_escreverNaColuna(&global_tela, "Interrupcao desconhecida.", 3);
	}
	privada_restauraContextoProcessoAoProcessador(kernel_param, *kernel_param->processoRodando);
}