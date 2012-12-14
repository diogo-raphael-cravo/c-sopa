#include "../include/DadosComuns.h"

/**
* Variáveis globais acessíveis somente neste arquivo.
*/



//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------

/**
* Manda rodar o processo de índice dado.
* @param KERNEL				*kernel_param				O kernel ao qual pertencem o processo.
* @param DESCRITOR_PROCESSO	*descritorProcesso_param 	O processo que irá rodar.
*/
void privada_rodarProcesso(KERNEL *kernel_param, DESCRITOR_PROCESSO **descritorProcesso_param){
	kernel_param->processoRodando = descritorProcesso_param;
	descritorProcesso_setStatus(*descritorProcesso_param, STATUS_PROCESSO_EXECUTANDO);
	MMU_sincronizado_setBase(&global_MMU, descritorProcesso_getEnderecoInicio(*descritorProcesso_param));
	MMU_sincronizado_setLimite(&global_MMU, descritorProcesso_getTamanhoAreaMemoriaPalavras(*descritorProcesso_param));
}

/**
* Tira o processo que está rodando, deixando no estado pronto.
* @param KERNEL					*kernel_param				O kernel ao qual pertencem o processo.
*/
void privada_pararDeRodarProcessoRodando(KERNEL *kernel_param){
	descritorProcesso_setStatus(*kernel_param->processoRodando, STATUS_PROCESSO_PRONTO);
	FIFO_inserir(&kernel_param->filaProcessosProntos, kernel_param->processoRodando);
	kernel_param->processoRodando = DESCRITOR_PROCESSO_INEXISTENTE;
	MMU_sincronizado_setBase(&global_MMU, 0); //Estas duas ações impedem uso desautorizado da memória.
	MMU_sincronizado_setLimite(&global_MMU, 0); //Estas duas ações impedem uso desautorizado da memória.
}

/**
* Manda o processo esperar pelo disco.
* @param KERNEL					*kernel_param				O kernel ao qual pertencem o processo.
*/
void privada_mandarProcessoRodandoEsperarDisco(KERNEL *kernel_param){
	descritorProcesso_setStatus(*kernel_param->processoRodando, STATUS_PROCESSO_BLOQUEADO);
	FIFO_inserir(&kernel_param->filaProcessosBloqueados, kernel_param->processoRodando);
	kernel_param->processoRodando = DESCRITOR_PROCESSO_INEXISTENTE;
	MMU_sincronizado_setBase(&global_MMU, 0); //Estas duas ações impedem uso desautorizado da memória.
	MMU_sincronizado_setLimite(&global_MMU, 0); //Estas duas ações impedem uso desautorizado da memória.
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
* @param KERNEL 				*kernel_param					O kernel que receberá o processo.
* @param int					PID_param						PID do descritor do processo que será adicionado.
* @param int					PC_param						PC do descritor do processo que será adicionado.
* @param int					tamanhoMemoriaPalavras_param	Tamanho da região de memória do processo, quando criado.
* @return int	Indica se conseguiu adicionar o processo.
*/
int privada_adicionarProcesso(KERNEL *kernel_param, int PID_param, int PC_param, int tamanhoMemoriaPalavras_param){
	int adicionou = 0;
	int alocouMemoria;
	int enderecoAlocado;
	DESCRITOR_PROCESSO **novoProcesso = (DESCRITOR_PROCESSO**) malloc(sizeof(DESCRITOR_PROCESSO*));
	*novoProcesso = (DESCRITOR_PROCESSO*) malloc(sizeof(DESCRITOR_PROCESSO));
	
	if(kernel_param->quantidadeProcessos < MAXIMO_PROCESSOS_KERNEL){
		enderecoAlocado = mapaAlocacoesMemoria_alocar(&kernel_param->mapaMemoriaAlocada, tamanhoMemoriaPalavras_param);
		alocouMemoria = (enderecoAlocado != MEMORIA_ENDERECO_INEXISTENTE);

		if(alocouMemoria){
			descritorProcesso_inicializar(*novoProcesso, PID_param, enderecoAlocado, tamanhoMemoriaPalavras_param);
			contexto_setPC(descritorProcesso_getContexto(*novoProcesso), PC_param);
			descritorProcesso_setStatus(*novoProcesso, STATUS_PROCESSO_PRONTO);
			FIFO_inserir(&kernel_param->filaProcessosProntos, novoProcesso);
			kernel_param->quantidadeProcessos++;
			adicionou = 1;
		} else {
			free(*novoProcesso);
			adicionou = 0;
		}
	} else {
		free(*novoProcesso);
		adicionou = 0;
	}

	return adicionou;
}

/**
* Mata o processo que está rodando.
* @param KERNEL	*kernel_param 	O kernel cujo processo será morto.
*/
void privada_matarProcessoRodando(KERNEL *kernel_param){
	mapaAlocacoesMemoria_liberar(&kernel_param->mapaMemoriaAlocada, descritorProcesso_getEnderecoInicio(*kernel_param->processoRodando));
	kernel_param->processoRodando = DESCRITOR_PROCESSO_INEXISTENTE;
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

	mapaAlocacoesMemoria_inicializar(&kernel_param->mapaMemoriaAlocada, &global_MMU, MAXIMO_PROCESSOS_KERNEL);

	//Cuidado com a avaliação curto-circuito abaixo!
	int adicionou = privada_adicionarProcesso(kernel_param, 456, 0, 100);
	adicionou = privada_adicionarProcesso(kernel_param, 457, 30, 100) || adicionou;
	adicionou = privada_adicionarProcesso(kernel_param, 458, 100, 100) || adicionou;

	if(adicionou){
		privada_rodarProcesso(kernel_param, (DESCRITOR_PROCESSO**) FIFO_remover(&kernel_param->filaProcessosProntos));
	} else {
		tela_imprimirTelaAzulDaMorte(&global_tela, "O kernel nao estah conseguindo iniciar, ele nao consegue criar processos!");
	}
}

/**
* @param KERNEL			*kernel_param		O kernel que irá rodar.
* @param INTERRUPCAO	interrupcao_param	A interrupção que definirá o comportamento do kernel.
*/
void kernel_rodar(KERNEL *kernel_param, INTERRUPCAO interrupcao_param){
	char mensagem[200];
	sprintf(mensagem, "Kernel chamado para a interrupcao %d.", interrupcao_param);
	tela_escreverNaColuna(&global_tela, mensagem, 3);

	descritorProcesso_setContexto(*kernel_param->processoRodando, processador_getContexto(&global_processador));
	switch(interrupcao_param){
		case INTERRUPCAO_CONSOLE:
			sprintf(mensagem, "Operador digitou: %s", console_ultimaLinhaDigitada(&global_console));
			tela_escreverNaColuna(&global_tela, mensagem, 3);
			break;
		case INTERRUPCAO_TIMER:
			privada_escalonar(kernel_param);
			break;
		case INTERRUPCAO_DISCO:
			if(!FIFO_vazia(&kernel_param->filaProcessosBloqueados)){
				FIFO_inserir(&kernel_param->filaProcessosProntos, FIFO_remover(&kernel_param->filaProcessosBloqueados));
			}
			break;
		case INTERRUPCAO_SOFTWARE_PARA_DISCO:
			privada_mandarProcessoRodandoEsperarDisco(kernel_param);
			privada_escalonar(kernel_param);
			disco_executarOperacao(&global_disco, OPERACAO_LEITURA_DISCO, 0, 0);
			break;
		case INTERRUPCAO_SEGMENTACAO_MEMORIA:
			sprintf(mensagem, "O processo %d foi morto por falha de segmentacao.", descritorProcesso_getPID(*kernel_param->processoRodando));
			privada_matarProcessoRodando(kernel_param);
			privada_escalonar(kernel_param);
			tela_escreverNaColuna(&global_tela, mensagem, 3);
			break;
		default:
			tela_escreverNaColuna(&global_tela, "Interrupcao desconhecida.", 3);
	}
	processador_setContexto(&global_processador, descritorProcesso_getContexto(*kernel_param->processoRodando));
}






