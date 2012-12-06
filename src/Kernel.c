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
	descritorProcesso_setPC(kernel_param->descritoresProcessos[indiceProcesso_param], processador_getPC(&global_processador));
	descritorProcesso_setRegistrador(kernel_param->descritoresProcessos[indiceProcesso_param], 
									processador_getRegistrador(&global_processador));
}

/**
* Restaura o contexto do processo fornecido no processador.
* @param KERNEL	*kernel_param			O kernel em que a operação será realizada.
* @param int	indiceProcesso_param	Índice em descritoresProcessos do processo cujo contexto será restaurado ao processador.
*/
void privada_restauraContextoProcessoAoProcessador(KERNEL *kernel_param, int indiceProcesso_param){
	processador_setPC(&global_processador, descritorProcesso_getPC(kernel_param->descritoresProcessos[indiceProcesso_param]));
	processador_setRegistrador(&global_processador, 
		descritorProcesso_getRegistrador(kernel_param->descritoresProcessos[indiceProcesso_param]));
}

/**
* Roda o escalonador, mandando rodar o processo que merecer.
* @param KERNEL	*kernel_param 	O kernel em que a operação será realizada.
*/
void privada_escalonar(KERNEL *kernel_param){
	char mensagem[100];
	kernel_param->processoRodando = (kernel_param->processoRodando+1)%2;
	int PIDProcessoRodando = descritorProcesso_getPID(kernel_param->descritoresProcessos[kernel_param->processoRodando]);
	sprintf(mensagem, "CPU esta rodando %d.", PIDProcessoRodando);
	tela_escreverNaColuna(&global_tela, mensagem, 3);
}

/**
* Adiciona um processo a este kernel.
* @param KERNEL 				*kernel_param		O kernel que receberá o processo.
* @param DESCRITOR_PROCESSO		*descritor_param	Descritor do processo que será adicionado. Já deve ter sido alocado.
*/
void privada_adicionarProcesso(KERNEL *kernel_param, DESCRITOR_PROCESSO *descritor_param){
	kernel_param->descritoresProcessos[kernel_param->quantidadeProcessos] = descritor_param;
	kernel_param->quantidadeProcessos++;
}

//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/**
* @param KERNEL	*kernel_param	O kernel que será inicializado.
*/
void kernel_inicializar(KERNEL *kernel_param){
	kernel_param->descritoresProcessos = (DESCRITOR_PROCESSO**) malloc(sizeof(DESCRITOR_PROCESSO**));
	kernel_param->processoRodando = 0;
	kernel_param->quantidadeProcessos = 0;

	DESCRITOR_PROCESSO *processo1 = (DESCRITOR_PROCESSO*) malloc(sizeof(DESCRITOR_PROCESSO*));
	DESCRITOR_PROCESSO *processo2 = (DESCRITOR_PROCESSO*) malloc(sizeof(DESCRITOR_PROCESSO*));

	descritorProcesso_inicializar(processo1, 456);
	descritorProcesso_inicializar(processo2, 457);

	descritorProcesso_setPC(processo1, 0);
	//descritorProcesso_setPC(processo2, 30);
}

/**
* @param KERNEL			*kernel_param		O kernel que irá rodar.
* @param INTERRUPCAO	interrupcao_param	A interrupção que definirá o comportamento do kernel.
*/
void kernel_rodar(KERNEL *kernel_param, INTERRUPCAO interrupcao_param){
	char mensagem[100];
	sprintf(mensagem, "Kernel chamado para a interrupcao %d.", interrupcao_param);
	tela_escreverNaColuna(&global_tela, mensagem, 3);

	/*privada_salvarContextoProcessadorNoProcesso(kernel_param, kernel_param->processoRodando);
	switch(interrupcao_param){
		case INTERRUPCAO_TIMER: privada_escalonar(kernel_param);
			break;
		default: tela_escreverNaColuna(&global_tela, "Interrupcao desconhecida.", 3);
	}
	privada_restauraContextoProcessoAoProcessador(kernel_param, kernel_param->processoRodando);*/
}
