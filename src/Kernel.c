#include "../include/DadosComuns.h"

/**
* Variáveis globais acessíveis somente neste arquivo.
*/



//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------

/**
* @param KERNEL		*kernel_param	O kernel em que o processo será procurado.
* @param int		PID_param		PID do processo procurado.
* @return DESCRITOR_PROCESSO* 	O processo que tem o PID solicitado. Caso não exista neste kernel, retornará NULL.
*/
DESCRITOR_PROCESSO* privada_buscaProcessoComPID(KERNEL *kernel_param, int PID_param){
	int posicaoTestada;
	DESCRITOR_PROCESSO* processoTestado;
	DESCRITOR_PROCESSO* processoEncontrado;

	processoEncontrado = NULL;

	if(descritorProcesso_getPID(*kernel_param->processoRodando) == PID_param){
		processoEncontrado = *kernel_param->processoRodando;
	}
	for(posicaoTestada=0; posicaoTestada<
			FIFO_quantidadeElementos(&kernel_param->filaProcessosBloqueados); posicaoTestada++){
		processoTestado = * (DESCRITOR_PROCESSO**) FIFO_espiarPosicao(&kernel_param->filaProcessosBloqueados, posicaoTestada);
		if(descritorProcesso_getPID(processoTestado) == PID_param){
			processoEncontrado = processoTestado;
		}
	}
	for(posicaoTestada=0; posicaoTestada<
			FIFO_quantidadeElementos(&kernel_param->filaProcessosProntos); posicaoTestada++){
		processoTestado = * (DESCRITOR_PROCESSO**) FIFO_espiarPosicao(&kernel_param->filaProcessosProntos, posicaoTestada);
		if(descritorProcesso_getPID(processoTestado) == PID_param){
			processoEncontrado = processoTestado;
		}
	}

	return processoEncontrado;
}

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
* @param int					enderecoMemoria_param			Caso o processo esteja na memória. 
*																Caso contrário, deverá conter MEMORIA_ENDERECO_INEXISTENTE.
* @return int	Indica se conseguiu adicionar o processo.
*/
int privada_adicionarProcesso(KERNEL *kernel_param, int PID_param, int PC_param, int tamanhoMemoriaPalavras_param, int enderecoMemoria_param){
	int adicionou = 0;
	int alocouMemoria;
	int enderecoAlocado;
	DESCRITOR_PROCESSO **novoProcesso = (DESCRITOR_PROCESSO**) malloc(sizeof(DESCRITOR_PROCESSO*));
	*novoProcesso = (DESCRITOR_PROCESSO*) malloc(sizeof(DESCRITOR_PROCESSO));
	
	if(kernel_param->quantidadeProcessos < MAXIMO_PROCESSOS_KERNEL){
		if(enderecoMemoria_param == MEMORIA_ENDERECO_INEXISTENTE){
			enderecoAlocado = mapaAlocacoesMemoria_alocar(&kernel_param->mapaMemoriaAlocada, tamanhoMemoriaPalavras_param);
		} else {
			enderecoAlocado = enderecoMemoria_param;
		}
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
* @param KERNEL	*kernel_param			O kernel que criará o processo.
* @param char*	nomeArquivo_param		Nome do arquivo que contém o processo.
* @return ERRO_KERNEL	Indica o erro que aconteceu, caso algum erro tenha acontecido.
* ATENÇÃO: se o disco não estiver rodando, vai travar a thread até que o disco rode!
*/
int privada_criarProcesso(KERNEL *kernel_param, char* nomeArquivo_param){
	int PID, enderecoMemoria;
	int erro = KERNEL_ERRO_NENHUM;
	ARQUIVO *arquivoTransferido;

	if(kernel_param->quantidadeProcessos+1 == MAXIMO_PROCESSOS_KERNEL){
		erro = KERNEL_ERRO_MAXIMO_PROCESSOS_ATINGIDO;
	}

	if(erro == KERNEL_ERRO_NENHUM){
		PID = privada_getPIDNaoUsado(kernel_param);
		arquivoTransferido = sistemaArquivos_buscaPorNome(&kernel_param->sistemaDeArquivos, nomeArquivo_param);
		if(arquivoTransferido == NULL){
			erro = KERNEL_ERRO_ARQUIVO_INEXISTENTE;
		}
	}

	if(erro == KERNEL_ERRO_NENHUM){
		enderecoMemoria = mapaAlocacoesMemoria_alocar(&kernel_param->mapaMemoriaAlocada,
		arquivo_getTamanhoEmPalavras(arquivoTransferido));
		int faltouMemoria = (enderecoMemoria == MEMORIA_ENDERECO_INEXISTENTE);
		if(faltouMemoria){
			erro = KERNEL_ERRO_MEMORIA_INSUFICIENTE;
		}
	}

	if(erro == KERNEL_ERRO_NENHUM){
		DESCRITOR_PROCESSO **novoProcesso = (DESCRITOR_PROCESSO**) malloc(sizeof(DESCRITOR_PROCESSO*));
		*novoProcesso = (DESCRITOR_PROCESSO*) malloc(sizeof(DESCRITOR_PROCESSO));
		descritorProcesso_inicializar(*novoProcesso, PID, enderecoMemoria, arquivo_getTamanhoEmPalavras(arquivoTransferido));
		contexto_setPC(descritorProcesso_getContexto(*novoProcesso), 0);
		descritorProcesso_setStatus(*novoProcesso, STATUS_PROCESSO_PRONTO);

		kernel_param->arquivoTransferido = arquivoTransferido;

		OPERACAO_DISCO* operacaoDisco = gerenciadorDisco_criarOperacaoDiscoCargaDMA(enderecoMemoria, 
				arquivoTransferido->enderecoInicioDisco, 
				arquivo_getTamanhoEmPalavras(arquivoTransferido));
		OPERACAO_KERNEL* operacaoKernel = gerenciadorDisco_criarOperacaoKernelCriacaoProcesso(novoProcesso);

		gerenciadorDisco_agendar(&kernel_param->gerenciadorAcessoDisco, operacaoDisco, operacaoKernel);
	}

	return erro;
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
	} else if(8 <= strlen(comando_param)
				&& comando_param[0] == 'i' && comando_param[1] == 'm' && comando_param[2] == 'p'
				&& comando_param[3] == 'r' && comando_param[4] == 'i' && comando_param[5] == 'm'
				&& comando_param[6] == 'i' && comando_param[7] == 'r'){
		comandoDigitado = COMANDO_IMPRIMIR;
	} else if(5 <= strlen(comando_param)
				&& comando_param[0] == 'a' && comando_param[1] == 'j' && comando_param[2] == 'u'
				&& comando_param[3] == 'd' && comando_param[4] == 'a'){
		comandoDigitado = COMANDO_AJUDA;
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
void privada_getParametroComandoUsuario(KERNEL *kernel_param, char* comando_param, char* destinoParametro_param, int ordemParametro_param){
	char parametro[200];
	char* copiaComandoUsuario = (char*) malloc((strlen(comando_param)+1)*sizeof(char));
	char* palavra;
	int posicaoPalavra = 0;
	int haParametro = 0;

	strcpy(copiaComandoUsuario, comando_param);
	palavra=strtok(copiaComandoUsuario, " ");

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

	free(copiaComandoUsuario);
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
	int erro=0;

	switch(comandoExecutado){
		case COMANDO_EXECUCAO_PROGRAMA:
				privada_getParametroComandoUsuario(kernel_param, comando_param, parametro, 1);
				erro = privada_criarProcesso(kernel_param, parametro);
				if(erro == KERNEL_ERRO_NENHUM){
					sprintf(mensagem, "Executando programa %s", parametro);
					tela_escreverNaColuna(&global_tela, mensagem, 3);
				} else if(erro == KERNEL_ERRO_ARQUIVO_INEXISTENTE){
					sprintf(mensagem, "O programa %s nao foi encontrado.", parametro);
					tela_escreverNaColuna(&global_tela, mensagem, 3);
				} else if(erro == KERNEL_ERRO_CRIANDO_PROCESSO){
					tela_escreverNaColuna(&global_tela, "O kernel jah estah criando um processo. Por favor, aguarde.", 3);
				} else if(erro == KERNEL_ERRO_DISCO_OCUPADO){
					tela_escreverNaColuna(&global_tela, "A criacao do processo foi agendada, jah que o disco estah ocupado.", 3);
				} else if(erro == KERNEL_ERRO_MEMORIA_INSUFICIENTE){
					tela_escreverNaColuna(&global_tela, "O programa nao foi criado por falta de memoria.", 3);
				} else {
					tela_escreverNaColuna(&global_tela, "O programa nao foi criado por causa de um erro desconhecido.", 3);
				}
			break;
		case COMANDO_IMPRIMIR:
				privada_getParametroComandoUsuario(kernel_param, comando_param, parametro, 2);
				if(strcmp(parametro, "disco") == 0){
					disco_imprimir(&global_disco);
				} else if(strcmp(parametro, "memoria") == 0){
					memoria_imprimir(&global_memoria);
				} else if(strcmp(parametro, "registradores") == 0){
					memset(parametro, '\0', 200);
					privada_getParametroComandoUsuario(kernel_param, comando_param, parametro, 3);
					if(parametro[0] != '\0'){
						int processoExiste = (privada_buscaProcessoComPID(kernel_param, string_paraInt(parametro)) != NULL);
						if(processoExiste){
							sprintf(mensagem, "Registradores do processo %d.", string_paraInt(parametro));
							tela_escreverNaColuna(&global_tela, mensagem, 5);
							contexto_imprimirRegistradores(
								descritorProcesso_getContexto(
									privada_buscaProcessoComPID(kernel_param, string_paraInt(parametro))), 5);
						} else {
							sprintf(mensagem, "Processo com PID %d nao encontrado.", string_paraInt(parametro));
							tela_escreverNaColuna(&global_tela, mensagem, 5);
						}
					} else {
						sprintf(mensagem, "Registradores do processador.");
						tela_escreverNaColuna(&global_tela, mensagem, 5);
						contexto_imprimirRegistradores(processador_getContexto(&global_processador), 5);
					}
				} else {
					sprintf(mensagem, "Dispositivo %s nao encontrado.", parametro);
					tela_escreverNaColuna(&global_tela, mensagem, 5);
				}
			break;
		case COMANDO_AJUDA:
			tela_escreverNaColuna(&global_tela, "Listando comandos:", 5);
			tela_escreverNaColuna(&global_tela, " ajuda", 5);
			tela_escreverNaColuna(&global_tela, "Mostra todos comandos.", 5);
			tela_escreverNaColuna(&global_tela, " ./<nomeArquivo>", 5);
			tela_escreverNaColuna(&global_tela, "Executa o arquivo com nome especificado.", 5);
			tela_escreverNaColuna(&global_tela, " imprimir <nomeDispositivo>", 5);
			tela_escreverNaColuna(&global_tela, "Imprime todas as celulas de memoria do dispositivo. Os parametros aceitos sao:", 5);
			tela_escreverNaColuna(&global_tela, " disco", 5);
			tela_escreverNaColuna(&global_tela, " memoria", 5);
			tela_escreverNaColuna(&global_tela, " registradores //do processador", 5);
			tela_escreverNaColuna(&global_tela, " registradores <PID-do-processo>", 5);
			break;
		default:
			tela_escreverNaColuna(&global_tela, "Comando desconhecido.", 3);
	}
}

/**
* @param KERNEL 			*kernel_param 			O kernel que irá fazer a operação.
* ATENÇÃO: se não existir processo esperando, não fará nada.
* ATENÇÃO: considera-se que os registradores do contexto do processo contém dados da operação!
*/
void privada_realizaProximaOperacaoDisco(KERNEL *kernel_param){
/*
	DEIXA NA FILA

	PROCESSO_ESPERANDO **esperaProcesso = (PROCESSO_ESPERANDO**) FIFO_espiar(&kernel_param->filaProcessosRequisicaoDisco);

	if((*esperaProcesso)->motivoEspera == OPERACAO_LEITURA_DISCO){
		int posicaoLeitura = 1;//(*esperaProcesso)->processoEsperando
		disco_executarOperacao(&global_disco, OPERACAO_LEITURA_DISCO, posicaoLeitura, 0, 0);
	} else if((*esperaProcesso)->motivoEspera == OPERACAO_ESCRITA_DISCO){
		int posicaoEscrita = 1;
		PALAVRA* dadosEscrita = NULL;
		disco_executarOperacao(&global_disco, OPERACAO_ESCRITA_DISCO, posicaoEscrita, dadosEscrita, 0);
	}*/
}

/**
* Põe o processo que está rodando a esperar pelo disco.
* @param KERNEL 			*kernel_param 			O kernel que irá fazer a operação.
* @param OPERACAO_DISCO		operacao_param			A operação do disco que será executada.
*/
void privada_requisitaOperacaoDisco(KERNEL *kernel_param, OPERACAO_DISCO operacao_param){
/*	PROCESSO_ESPERANDO **esperaProcesso = (PROCESSO_ESPERANDO**) malloc(sizeof(PROCESSO_ESPERANDO*));
	*esperaProcesso = (PROCESSO_ESPERANDO*) malloc(sizeof(PROCESSO_ESPERANDO));

	(*esperaProcesso)->processoEsperando = kernel_param->processoRodando;
	(*esperaProcesso)->motivoEspera = operacao_param;

	if(FIFO_vazia(&kernel_param->filaProcessosRequisicaoDisco)){
		privada_realizaProximaOperacaoDisco(kernel_param);
	}
	FIFO_inserir(&kernel_param->filaProcessosRequisicaoDisco, esperaProcesso);*/
}


void privada_recebeResultadoOperacaoDisco(){
/*
	REMOVE
*/
}

//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/**
* @param KERNEL	*kernel_param	O kernel que será inicializado.
*/
void kernel_inicializar(KERNEL *kernel_param){
	kernel_param->quantidadeProcessos = 0;
	kernel_param->ultimoPIDUsado = -1;
	FIFO_inicializar(&kernel_param->filaProcessosProntos, MAXIMO_PROCESSOS_KERNEL);
	FIFO_inicializar(&kernel_param->filaProcessosBloqueados, MAXIMO_PROCESSOS_KERNEL);

	gerenciadorDisco_inicializar(&kernel_param->gerenciadorAcessoDisco, &global_disco);
	sistemaArquivos_inicializarComArquivosDoHospedeiro(&kernel_param->sistemaDeArquivos, &global_disco);
	mapaAlocacoesMemoria_inicializar(&kernel_param->mapaMemoriaAlocada, &global_MMU, MAXIMO_PROCESSOS_KERNEL);

	memoria_sincronizado_escreverBytes(&global_memoria, 0, 'J', 'P', 'A', 0);
	int adicionou = privada_adicionarProcesso(kernel_param, privada_getPIDNaoUsado(kernel_param), 0, 1, MEMORIA_ENDERECO_INEXISTENTE);

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
	OPERACAO_KERNEL* operacaoKernel;

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
			operacaoKernel = gerenciadorDisco_proximaOperacaoKernel(&kernel_param->gerenciadorAcessoDisco);
			gerenciadorDisco_executarProxima(&kernel_param->gerenciadorAcessoDisco);
			if(operacaoKernel->tipoOperacao == TIPO_OPERACAO_AGENDAVEL_CRIACAO_PROCESSO_KERNEL){

				OPERACAO_CRIACAO_PROCESSO_KERNEL* operacaoExecutada = (OPERACAO_CRIACAO_PROCESSO_KERNEL*) operacaoKernel->operacao;
				FIFO_inserir(&kernel_param->filaProcessosProntos, operacaoExecutada->processoCriado);
				kernel_param->quantidadeProcessos++;
			} else if(operacaoKernel->tipoOperacao == TIPO_OPERACAO_AGENDAVEL_FEITA_POR_PROCESSO_KERNEL){
				//FIFO_inserir(&kernel_param->filaProcessosProntos, FIFO_remover(&kernel_param->filaProcessosBloqueados));
			}

			free(operacaoKernel->operacao);
			free(operacaoKernel);
			break;
		case INTERRUPCAO_SOFTWARE_PARA_DISCO:
//			privada_requisitaOperacaoDisco(kernel_param, OPERACAO_LEITURA_DISCO);
/*
		OPERACAO_DISCO* operacaoDisco = gerenciadorDisco_criarOperacaoDiscoCargaDMA(enderecoMemoria, 
				arquivoTransferido->enderecoInicioDisco, 
				arquivo_getTamanhoEmPalavras(arquivoTransferido));
		OPERACAO_KERNEL* operacaoKernel = gerenciadorDisco_criarOperacaoKernelCriacaoProcesso(*novoProcesso);

		gerenciadorDisco_agendar(&kernel_param->gerenciadorAcessoDisco, operacaoDisco, operacaoKernel);

*/
			break;
		case INTERRUPCAO_SEGMENTACAO_MEMORIA:
			sprintf(mensagem, "O processo %d foi morto por falha de segmentacao.", descritorProcesso_getPID(*kernel_param->processoRodando));
			tela_escreverNaColuna(&global_tela, mensagem, 3);
			sprintf(mensagem,  "Imprimindo contexto do DESCRITOR DO PROCESSO imediatamente anterior 'a falha.");
			tela_escreverNaColuna(&global_tela, mensagem, 3);
			contexto_imprimirRegistradores(descritorProcesso_getContexto(*kernel_param->processoRodando), 3);
			sprintf(mensagem,  "Imprimindo contexto do PROCESSADOR imediatamente anterior 'a falha.");
			tela_escreverNaColuna(&global_tela, mensagem, 3);
			processador_imprimir(&global_processador, 3);

			privada_matarProcessoRodando(kernel_param);
			privada_escalonar(kernel_param);
			break;
		default:
			tela_escreverNaColuna(&global_tela, "Interrupcao desconhecida.", 3);
	}
	processador_setContexto(&global_processador, descritorProcesso_getContexto(*kernel_param->processoRodando));
}






