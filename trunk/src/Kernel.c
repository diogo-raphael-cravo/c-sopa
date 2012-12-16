#include "../include/DadosComuns.h"

/**
* Variáveis globais acessíveis somente neste arquivo.
*/



//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------

/**
* @param KERNEL	*kernel_param 	Kernel em que a operação será realizada.
* @return int	Um PID ainda não usado por processos deste kernel.
*/
int privada_getPIDNaoUsado(KERNEL *kernel_param){
	kernel_param->ultimoPIDUsado += 1;
	return kernel_param->ultimoPIDUsado;
}

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


/**
* Inicia a criação de um processo.
* @param KERNEL	*kernel_param		O kernel que criará o processo.
* @param char*	nomeArquivo_param	Nome do arquivo que contém o processo.
* @return int	Indica se foi possível criar.
* ATENÇÃO: se o disco não estiver rodando, vai travar a thread até que o disco rode!
*/
int privada_criarProcesso(KERNEL *kernel_param, char* nomeArquivo_param){
	int conseguiu=0;
	ARQUIVO *arquivoTransferido;
	if(!kernel_param->criandoProcesso){
		kernel_param->criandoProcesso = 1;
		arquivoTransferido = sistemaArquivos_buscaPorNome(&kernel_param->sistemaDeArquivos, nomeArquivo_param);
		if(arquivoTransferido != NULL){
			conseguiu = 1;
			kernel_param->arquivoTransferido = arquivoTransferido;
			disco_executarOperacao(&global_disco, OPERACAO_CARGA_DISCO, arquivoTransferido->blocoInicioDisco, 0, 0);
		}
	}
	return conseguiu;
}

/**
* Identifica o comando digitado pelo usuário.
* @param KERNEL 	*kernel_param 	O kernel que irá fazer a identificação.
* @param char*		comando_param	O comando que será identificado.
* @return COMANDO_USUARIO	O comando que o usuário digitou.
*/
COMANDO_USUARIO privada_getComandoUsuario(KERNEL *kernel_param, char* comando_param){
	COMANDO_USUARIO comandoDigitado = COMANDO_NENHUM;

	if(2 < strlen(comando_param) && comando_param[0] == '.' && comando_param[1] == '/'){
		comandoDigitado = COMANDO_EXECUCAO_PROGRAMA;
	}

	return comandoDigitado;
}

/**
* Identifica o n-ésimo parâmetro do comando digitado pelo usuário.
* @param KERNEL 			*kernel_param 			O kernel que irá fazer a identificação.
* @param char*				comando_param			O comando que será identificado.
* @param char*				destinoParametro_param	Variável em que o parâmetro será colocado.
*													n-ésimo parâmetro do comando do usuário. NULL, caso não haja.
* @param int				ordemParametro_param	Valor de 'n'.
*/
char* privada_getParametroComandoUsuario(KERNEL *kernel_param, char* comando_param, char* destinoParametro_param, int ordemParametro_param){
	char parametro[200];
	char* palavra=strtok(comando_param, " ");
	int posicaoPalavra = 0;
	int haParametro = 0;

	if(privada_getComandoUsuario(kernel_param, comando_param) == COMANDO_EXECUCAO_PROGRAMA && ordemParametro_param==1){
		haParametro = 1;
		for(; posicaoPalavra<strlen(palavra)-2; posicaoPalavra++){
			parametro[posicaoPalavra] = palavra[posicaoPalavra+2];
		}
		parametro[posicaoPalavra] = '\0';
	} else {
		do{
			if(palavra != NULL){
				posicaoPalavra++;
				if(posicaoPalavra==ordemParametro_param){
					haParametro = 1;
					strcpy(parametro, palavra);
				}
			}
		}while(palavra=strtok(NULL," "));
	}

	if(haParametro){
		strcpy(destinoParametro_param, parametro);
	} else {
		destinoParametro_param = NULL;
	}
}

/**
* Executa comando dado pelo usuário.
* @param KERNEL 			*kernel_param 			O kernel que irá fazer a identificação.
* @param char*				comando_param			O comando que será identificado.
*/
void privada_executarComandoUsuario(KERNEL *kernel_param, char* comando_param){
	COMANDO_USUARIO comandoExecutado = privada_getComandoUsuario(kernel_param, comando_param);
	char mensagem[200];
	char parametro[200];
	int conseguiu=0;

	switch(comandoExecutado){
		case COMANDO_EXECUCAO_PROGRAMA:
				privada_getParametroComandoUsuario(kernel_param, comando_param, parametro, 1);
				conseguiu = privada_criarProcesso(kernel_param, parametro);
				if(conseguiu){
					sprintf(mensagem, "Executando programa %s", parametro);
					tela_escreverNaColuna(&global_tela, mensagem, 3);
				} else {
					sprintf(mensagem, "O programa %s nao foi encontrado.", parametro);
					tela_escreverNaColuna(&global_tela, mensagem, 3);
				}
			break;
		default:
			tela_escreverNaColuna(&global_tela, "Comando desconhecido.", 3);
	}
}


//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/**
* @param KERNEL	*kernel_param	O kernel que será inicializado.
*/
void kernel_inicializar(KERNEL *kernel_param){
	kernel_param->quantidadeProcessos = 0;
	kernel_param->fazendoTransferenciaDiscoMemoria = 0;
	kernel_param->criandoProcesso = 0;
	kernel_param->ultimoPIDUsado = -1;
	FIFO_inicializar(&kernel_param->filaProcessosProntos, MAXIMO_PROCESSOS_KERNEL);
	FIFO_inicializar(&kernel_param->filaProcessosBloqueados, MAXIMO_PROCESSOS_KERNEL);

	sistemaArquivos_inicializarComArquivosDoHospedeiro(&kernel_param->sistemaDeArquivos, &global_disco);
	mapaAlocacoesMemoria_inicializar(&kernel_param->mapaMemoriaAlocada, &global_MMU, MAXIMO_PROCESSOS_KERNEL);

	memoria_sincronizado_escreverBytes(&global_memoria, 0, 'J', 'P', 'A', 0);
	int adicionou = privada_adicionarProcesso(kernel_param, privada_getPIDNaoUsado(kernel_param), 0, 1);

	if(adicionou){
		privada_rodarProcesso(kernel_param, (DESCRITOR_PROCESSO**) FIFO_remover(&kernel_param->filaProcessosProntos));
	} else {
		tela_imprimirTelaAzulDaMorte(&global_tela, "O kernel nao conseguiu criar o processo dummy.");
	}
}

/**
* @param KERNEL			*kernel_param		O kernel que irá rodar.
* @param INTERRUPCAO	interrupcao_param	A interrupção que definirá o comportamento do kernel.
*/
void kernel_rodar(KERNEL *kernel_param, INTERRUPCAO interrupcao_param){
	char mensagem[200];
	char* mensagemOperador;

	sprintf(mensagem, "Kernel chamado para a interrupcao %d.", interrupcao_param);
	tela_escreverNaColuna(&global_tela, mensagem, 3);

	descritorProcesso_setContexto(*kernel_param->processoRodando, processador_getContexto(&global_processador));
	switch(interrupcao_param){
		case INTERRUPCAO_CONSOLE:
			mensagemOperador = console_ultimaLinhaDigitada(&global_console);
			sprintf(mensagem, "Operador digitou: %s", mensagemOperador);
			tela_escreverNaColuna(&global_tela, mensagem, 3);
			privada_executarComandoUsuario(kernel_param, mensagemOperador);
			break;
		case INTERRUPCAO_TIMER:
			privada_escalonar(kernel_param);
			break;
		case INTERRUPCAO_DISCO:
			if(kernel_param->criandoProcesso){
				int enderecoMemoria = 1;
				int adicionouProcesso = 0;
				int PID = privada_getPIDNaoUsado(kernel_param);
				arquivo_transferirParaMemoria(kernel_param->arquivoTransferido, &global_MMU, enderecoMemoria);
				adicionouProcesso = privada_adicionarProcesso(kernel_param, PID, 0, 
					arquivo_getTamanhoEmPalavras(kernel_param->arquivoTransferido));
				if(!adicionouProcesso){
					sprintf(mensagem, "O kernel nao conseguiu criar o processo %d com tamanho %d.", 
						PID, arquivo_getTamanhoEmPalavras(kernel_param->arquivoTransferido));
					tela_imprimirTelaAzulDaMorte(&global_tela, mensagem);
				}
			} else if(kernel_param->fazendoTransferenciaDiscoMemoria){
				tela_imprimirTelaAzulDaMorte(&global_tela, "KERNEL: a transferencia DISCO>MEMORIA nao foi implementada ainda.");
			} else {
				if(!FIFO_vazia(&kernel_param->filaProcessosBloqueados)){
					FIFO_inserir(&kernel_param->filaProcessosProntos, FIFO_remover(&kernel_param->filaProcessosBloqueados));
				}
			}
			break;
		case INTERRUPCAO_SOFTWARE_PARA_DISCO:
			privada_mandarProcessoRodandoEsperarDisco(kernel_param);
			privada_escalonar(kernel_param);
			disco_executarOperacao(&global_disco, OPERACAO_LEITURA_DISCO, 0, 0, 0);
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






