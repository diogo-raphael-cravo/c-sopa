#include "../include/DadosComuns.h"

/**
* Variáveis globais acessíveis somente neste arquivo.
*/



//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------

/**
* @param KERNEL 		*kernel_param		O kernel cujo erro será consultado.
* @param ERRO_KERNEL	erro_param			Erro que será impresso.
*/
void privada_imprimirErro(ERRO_KERNEL erro_param){
	switch(erro_param){
		case KERNEL_ERRO_NENHUM:
			tela_escreverNaColuna(&global_tela, "Sucesso!", 3);
			break;
		case KERNEL_ERRO_ARQUIVO_INEXISTENTE:
			tela_escreverNaColuna(&global_tela, "Arquivo inexistente.", 3);
			break;
		case KERNEL_ERRO_CRIANDO_PROCESSO:
			tela_escreverNaColuna(&global_tela, "O kernel jah estah criando um processo.", 3);
			break;
		case KERNEL_ERRO_MEMORIA_INSUFICIENTE:
			tela_escreverNaColuna(&global_tela, "O espaco livre em memoria eh insuficiente.", 3);
			break;
		case KERNEL_ERRO_MEMORIA_DISCO_INSUFICIENTE:
			tela_escreverNaColuna(&global_tela, "O espaco livre em disco eh insuficiente.", 3);
			break;
		case KERNEL_ERRO_MAXIMO_PROCESSOS_ATINGIDO:
			tela_escreverNaColuna(&global_tela, "O maximo de processos foi atingido.", 3);
			break;
		case KERNEL_ERRO_PROCESSO_NAO_TEM_ARQUIVO:
			tela_escreverNaColuna(&global_tela, "O arquivo nao pertence ao processo.", 3);
			break;
		case KERNEL_ERRO_DISCO_OCUPADO:
			tela_escreverNaColuna(&global_tela, "O disco estah ocupado.", 3);
			break;
		case KERNEL_ERRO_FIM_DO_ARQUIVO:
			tela_escreverNaColuna(&global_tela, "Fim do arquivo alcancado.", 3);
			break;
		case KERNEL_ERRO_ARQUIVO_ABERTO_COM_OUTRO_FIM:
			tela_escreverNaColuna(&global_tela, "Arquivo foi aberto com outro fim.", 3);
			break;
		case KERNEL_ERRO_OPERACAO_RPC_INVALIDA:
			tela_escreverNaColuna(&global_tela, "Operacao de RPC invalida.", 3);
			break;
		case KERNEL_ERRO_NAO_IDENTIFICADO:
			tela_escreverNaColuna(&global_tela, "Erro desconhecido!", 3);
			break;
		case KERNEL_ERRO_PROCESSO_INEXISTENTE:
			tela_escreverNaColuna(&global_tela, "O processo nao existe.", 3);
			break;
		default:
			tela_escreverNaColuna(&global_tela, "Erro desconhecido (e nao reconhecido como desconhecido)!", 3);
	}
}

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
			FIFO_quantidadeElementos(&kernel_param->filaProcessosBloqueadosRPC); posicaoTestada++){
		processoTestado = * (DESCRITOR_PROCESSO**) FIFO_espiarPosicao(&kernel_param->filaProcessosBloqueadosRPC, posicaoTestada);
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
	MMU_sincronizado_setBase(&global_MMU, descritorProcesso_getEnderecoInicioCodigo(*descritorProcesso_param));
	MMU_sincronizado_setLimite(&global_MMU, descritorProcesso_getTamanhoAreaCodigoPalavras(*descritorProcesso_param));
	descritorProcesso_inicializarStack(*descritorProcesso_param);
}

/**
* Tira o processo que está rodando, deixando no estado pronto.
* @param KERNEL					*kernel_param				O kernel ao qual pertencem o processo.
*/
void privada_pararDeRodarProcessoRodando(KERNEL *kernel_param){
	FIFO_inserir(&kernel_param->filaProcessosProntos, kernel_param->processoRodando);
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
		tela_escreverNaColuna(&global_tela, "Nao ha processo elegivel para ser executado. Vou executar o dummy.", 3);
	}
}

/**
* Adiciona um processo a este kernel.
* @param KERNEL 				*kernel_param					O kernel que receberá o processo.
* @param int					PID_param						PID do descritor do processo que será adicionado.
* @param int					PC_param						PC do descritor do processo que será adicionado.
* @param int					tamanhoCodigoPalavras_param		Tamanho da região de código do processo, quando criado.
* @param int					enderecoMemoria_param			Caso o processo esteja na memória. 
*																Caso contrário, deverá conter MEMORIA_ENDERECO_INEXISTENTE.
* @return int	Indica se conseguiu adicionar o processo.
* ATENÇÃO: O PROCESSO NÃO TERÁ STACK!!!!!
*/
int privada_adicionarProcesso(KERNEL *kernel_param, int PID_param, int PC_param, int tamanhoCodigoPalavras_param, 
		int enderecoMemoria_param){
	int adicionou = 0;
	int alocouMemoria;
	int enderecoAlocado;
	DESCRITOR_PROCESSO **novoProcesso = (DESCRITOR_PROCESSO**) malloc(sizeof(DESCRITOR_PROCESSO*));
	*novoProcesso = (DESCRITOR_PROCESSO*) malloc(sizeof(DESCRITOR_PROCESSO));

	if(kernel_param->quantidadeProcessos < MAXIMO_PROCESSOS_KERNEL){
		if(enderecoMemoria_param == MEMORIA_ENDERECO_INEXISTENTE){
			enderecoAlocado = mapaAlocacoesMemoria_alocar(&kernel_param->mapaMemoriaAlocada, tamanhoCodigoPalavras_param+1);
		} else {
			enderecoAlocado = enderecoMemoria_param;
		}
		alocouMemoria = (enderecoAlocado != MEMORIA_ENDERECO_INEXISTENTE);

		if(alocouMemoria){
			descritorProcesso_inicializar(*novoProcesso, PID_param, enderecoAlocado, tamanhoCodigoPalavras_param, 0);
			contexto_setPC(descritorProcesso_getContexto(*novoProcesso), PC_param);
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
	sistemaArquivos_fecharArquivosAbertosPara(&kernel_param->sistemaDeArquivos, *kernel_param->processoRodando);
	mapaAlocacoesMemoria_liberar(&kernel_param->mapaMemoriaAlocada, descritorProcesso_getEnderecoInicio(*kernel_param->processoRodando));
	kernel_param->processoRodando = DESCRITOR_PROCESSO_INEXISTENTE;
}

/**
* Inicia a criação de um processo.
* @param KERNEL			*kernel_param				O kernel que criará o processo.
* @param char*			nomeArquivo_param			Nome do arquivo que contém o processo.
* @param CONTEXTO		*contextoInicial_param		Contexto inicial do processo. NULL, caso seja default.
* @return ERRO_KERNEL	Indica o erro que aconteceu, caso algum erro tenha acontecido.
* ATENÇÃO: se o disco não estiver rodando, vai travar o processo até que o disco rode!
*/
int privada_criarProcesso(KERNEL *kernel_param, char* nomeArquivo_param, CONTEXTO *contextoInicial_param){
	int PID, enderecoMemoria;
	int erro = KERNEL_ERRO_NENHUM;
	int tamanhoStackProcesso = TAMANHO_PADRAO_STACK_PALAVRAS;
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
			arquivo_getTamanhoEmPalavras(arquivoTransferido)+tamanhoStackProcesso+1);
//1, pois a primeira palavra da stack guarda o tamanho dela
		int faltouMemoria = (enderecoMemoria == MEMORIA_ENDERECO_INEXISTENTE);
		if(faltouMemoria){
			erro = KERNEL_ERRO_MEMORIA_INSUFICIENTE;
		}
	}

	if(erro == KERNEL_ERRO_NENHUM){
		DESCRITOR_PROCESSO **novoProcesso = (DESCRITOR_PROCESSO**) malloc(sizeof(DESCRITOR_PROCESSO*));
		*novoProcesso = (DESCRITOR_PROCESSO*) malloc(sizeof(DESCRITOR_PROCESSO));
		descritorProcesso_inicializar(*novoProcesso, PID, enderecoMemoria, 
			arquivo_getTamanhoEmPalavras(arquivoTransferido), tamanhoStackProcesso);
		if(contextoInicial_param != NULL){
			contexto_copiar(descritorProcesso_getContexto(*novoProcesso), contextoInicial_param);
			free(contextoInicial_param);
		}
		contexto_setPC(descritorProcesso_getContexto(*novoProcesso), 0);
		memoria_sincronizado_escreverPalavra(&global_memoria, 
			descritorProcesso_getEnderecoInicioCodigo(*novoProcesso)-1, 
			descritorProcesso_getTamanhoStackPalavras(*novoProcesso)-1); //-1, porque deve ser somente a área útil da stack

		kernel_param->arquivoTransferido = arquivoTransferido;

		OPERACAO_DISCO* operacaoDisco = gerenciadorDisco_criarOperacaoDiscoCargaDMA(
				descritorProcesso_getEnderecoInicioCodigo(*novoProcesso), 
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
		}while((palavra=strtok(NULL," ")));
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
				erro = privada_criarProcesso(kernel_param, parametro, NULL);
				if(erro == KERNEL_ERRO_NENHUM){
					sprintf(mensagem, "Executando programa %s", parametro);
					tela_escreverNaColuna(&global_tela, mensagem, 3);
				} else if(erro == KERNEL_ERRO_ARQUIVO_INEXISTENTE){
					sprintf(mensagem, "O programa %s nao foi encontrado ou estah fragmentado e nao pode ser carregado (a ultima opcao caracteriza erro de implementacao).", parametro);
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
* Abre um arquivo para o processo que está rodando.
* @param KERNEL						*kernel_param		O kernel que abrirá o arquivo.
* @param char*						nome_param			O nome do arquivo.
* @param OPCAO_ABERTURA_ARQUIVO		opcao_param			O que será feito com o arquivo.
* @param DESCRITOR_PROCESSO			*processo_param		O processo que terá posse do arquivo aberto.
* @return int	Número descritor do arquivo (usado por processos do SOPA para operar sobre o arquivo).
*				Caso não tenha sido possível abrir, retornará NUMERO_DESCRITOR_ARQUIVO_INEXISTENTE.
*/
int privada_abrirArquivo(KERNEL *kernel_param, char* nome_param, OPCAO_ABERTURA_ARQUIVO opcao_param, DESCRITOR_PROCESSO *processo_param){
	char mensagem[200];
	sprintf(mensagem, "Abrindo arquivo %s.", nome_param);
	tela_escreverNaColuna(&global_tela, mensagem, 3);

	int descritorArquivoCriado = NUMERO_DESCRITOR_ARQUIVO_INEXISTENTE;
	int criandoArquivoNovo = (sistemaArquivos_buscaPorNome(&kernel_param->sistemaDeArquivos, nome_param) == NULL);

	if(criandoArquivoNovo){
		descritorArquivoCriado = sistemaArquivos_criarArquivo(&kernel_param->sistemaDeArquivos, nome_param, &global_disco);
		if(descritorArquivoCriado == NUMERO_DESCRITOR_ARQUIVO_INEXISTENTE){
			sprintf(mensagem, "Nao encontrei espaco para %s.", nome_param);
			tela_escreverNaColuna(&global_tela, mensagem, 3);
		} else {
			sistemaArquivos_abrirArquivoExistentePara(&kernel_param->sistemaDeArquivos, nome_param, processo_param, opcao_param);
		}
	} else {
		descritorArquivoCriado = sistemaArquivos_abrirArquivoExistentePara(&kernel_param->sistemaDeArquivos, nome_param, 
			processo_param, opcao_param);
	}

	return descritorArquivoCriado;
}

/**
* Agenda a leitura da próxima palavra do arquivo.
* @param KERNEL						*kernel_param					O kernel que executará a operação.
* @param int						numeroDescritorArquivo_param	O arquivo de cuja posse será testada.
* @param PALAVRA					palavraEscrita_param			A palavra que será escrita.
* @return ERRO_KERNEL	Um erro que descreve se a operação foi realizada ou o erro que aconteceu.
*/
ERRO_KERNEL privada_agendarPutPalavraDeArquivoParaProcessoRodando(KERNEL *kernel_param, int numeroDescritorArquivo_param,
		PALAVRA palavraEscrita_param){
	ERRO_KERNEL erro = KERNEL_ERRO_NENHUM;
	DESCRITOR_ARQUIVO *arquivo;
	int enderecoLogicoEscrita;
	int enderecoFisicoEscrita;

	if(!sistemaArquivos_arquivoEstahAbertoPara(&kernel_param->sistemaDeArquivos, 
			*kernel_param->processoRodando, numeroDescritorArquivo_param)){
		erro = KERNEL_ERRO_PROCESSO_NAO_TEM_ARQUIVO;
	}

	if(erro == KERNEL_ERRO_NENHUM){
		arquivo = sistemaArquivos_buscaPorNumeroDescritor(&kernel_param->sistemaDeArquivos, 
			numeroDescritorArquivo_param);
		if(!descritorArquivo_abertoComOpcao(arquivo, OPCAO_ABERTURA_ARQUIVO_ESCREVER)){
			erro = KERNEL_ERRO_ARQUIVO_ABERTO_COM_OUTRO_FIM;
		}
	}

	if(erro == KERNEL_ERRO_NENHUM){
		enderecoLogicoEscrita = descritorArquivo_getPalavraAtual(arquivo);
		if(enderecoLogicoEscrita == DESCRITOR_ARQUIVO_POSICAO_INEXISTENTE){
			int posicaoLivreDisco = sistemaArquivos_getPosicaoLivreDisco(&kernel_param->sistemaDeArquivos, &global_disco);
			if(posicaoLivreDisco != MEMORIA_ENDERECO_INEXISTENTE){
				ARQUIVO *segmentoNovo = (ARQUIVO*) malloc(sizeof(ARQUIVO));
				arquivo_inicializar(segmentoNovo, posicaoLivreDisco);
				descritorArquivo_adicionarSegmento(arquivo, segmentoNovo);
			} else {
				erro = KERNEL_ERRO_MEMORIA_DISCO_INSUFICIENTE;
			}
		}
	}

	if(erro == KERNEL_ERRO_NENHUM){
		enderecoFisicoEscrita = descritorArquivo_getEnderecoDiscoPosicao(arquivo, enderecoLogicoEscrita);
		descritorArquivo_setPalavraAtual(arquivo, descritorArquivo_getPalavraAtual(arquivo)+1);

		//privada_escreverEndereco(...) em Disco.c vai dar o free(palavraEscrita).
		PALAVRA* palavraEscrita = (PALAVRA*) malloc(sizeof(PALAVRA)); 
		*palavraEscrita = palavraEscrita_param;

		OPERACAO_DISCO* operacaoDisco = gerenciadorDisco_criarOperacaoDiscoEscrita(enderecoFisicoEscrita, palavraEscrita, 1);
		OPERACAO_KERNEL* operacaoKernel;
		operacaoKernel = gerenciadorDisco_criarOperacaoKernelFeitaPorProcesso(
			kernel_param->processoRodando, TIPO_OPERACAO_ESCRITA_DISCO);

		gerenciadorDisco_agendar(&kernel_param->gerenciadorAcessoDisco, operacaoDisco, operacaoKernel);
	}

	return erro;
}

/**
* Agenda a leitura da próxima palavra do arquivo.
* @param KERNEL						*kernel_param					O kernel que executará a operação.
* @param int						numeroDescritorArquivo_param	O arquivo de cuja posse será testada.
* @return ERRO_KERNEL	Um erro que descreve se a operação foi realizada ou o erro que aconteceu.
*/
ERRO_KERNEL privada_agendarGetPalavraDeArquivoParaProcessoRodando(KERNEL *kernel_param, int numeroDescritorArquivo_param){
	ERRO_KERNEL erro = KERNEL_ERRO_NENHUM;
	DESCRITOR_ARQUIVO *arquivoLido;
	int enderecoLogicoLeitura;
	int enderecoFisicoLeitura;

	if(!sistemaArquivos_arquivoEstahAbertoPara(&kernel_param->sistemaDeArquivos, 
			*kernel_param->processoRodando, numeroDescritorArquivo_param)){
		erro = KERNEL_ERRO_PROCESSO_NAO_TEM_ARQUIVO;
	}

	if(erro == KERNEL_ERRO_NENHUM){
		arquivoLido = sistemaArquivos_buscaPorNumeroDescritor(&kernel_param->sistemaDeArquivos, 
			numeroDescritorArquivo_param);

		if(!descritorArquivo_abertoComOpcao(arquivoLido, OPCAO_ABERTURA_ARQUIVO_LER)){
			erro = KERNEL_ERRO_ARQUIVO_ABERTO_COM_OUTRO_FIM;
		}
	}

	if(erro == KERNEL_ERRO_NENHUM){
		enderecoLogicoLeitura = descritorArquivo_getPalavraAtual(arquivoLido);
		if(enderecoLogicoLeitura == DESCRITOR_ARQUIVO_POSICAO_INEXISTENTE){
			erro = KERNEL_ERRO_FIM_DO_ARQUIVO;
		}
	}

	if(erro == KERNEL_ERRO_NENHUM){
		enderecoFisicoLeitura = descritorArquivo_getEnderecoDiscoPosicao(arquivoLido, enderecoLogicoLeitura);
		descritorArquivo_setPalavraAtual(arquivoLido, descritorArquivo_getPalavraAtual(arquivoLido)+1);

		OPERACAO_DISCO* operacaoDisco = gerenciadorDisco_criarOperacaoDiscoLeitura(enderecoFisicoLeitura);
		OPERACAO_KERNEL* operacaoKernel;
		operacaoKernel = gerenciadorDisco_criarOperacaoKernelFeitaPorProcesso(
			kernel_param->processoRodando, TIPO_OPERACAO_LEITURA_DISCO);

		gerenciadorDisco_agendar(&kernel_param->gerenciadorAcessoDisco, operacaoDisco, operacaoKernel);
	}

	return erro;
}

/**
* Faz uma chamada RPC, enviando mensagem ao IP destino.
* @param KERNEL					*kernel_param				O kernel que realizará a chamada.
* @param DESCRITOR_PROCESSO		*processoRequerente_param	Processo que solicita a chamada.
*/
ERRO_KERNEL privada_solicitarRPC(KERNEL *kernel_param, DESCRITOR_PROCESSO *processoRequerente_param){
	ERRO_KERNEL erro = KERNEL_ERRO_NENHUM;

	PALAVRA r0  = registrador_lerPalavra(contexto_getRegistrador(descritorProcesso_getContexto(*kernel_param->processoRodando),  0));
	PALAVRA r1  = registrador_lerPalavra(contexto_getRegistrador(descritorProcesso_getContexto(*kernel_param->processoRodando),  1));
	PALAVRA r2  = registrador_lerPalavra(contexto_getRegistrador(descritorProcesso_getContexto(*kernel_param->processoRodando),  2));
	PALAVRA r3  = registrador_lerPalavra(contexto_getRegistrador(descritorProcesso_getContexto(*kernel_param->processoRodando),  3));
	PALAVRA r4  = registrador_lerPalavra(contexto_getRegistrador(descritorProcesso_getContexto(*kernel_param->processoRodando),  4));
	PALAVRA r5  = registrador_lerPalavra(contexto_getRegistrador(descritorProcesso_getContexto(*kernel_param->processoRodando),  5));
	PALAVRA r6  = registrador_lerPalavra(contexto_getRegistrador(descritorProcesso_getContexto(*kernel_param->processoRodando),  6));
	PALAVRA r7  = registrador_lerPalavra(contexto_getRegistrador(descritorProcesso_getContexto(*kernel_param->processoRodando),  7));
	PALAVRA r8  = registrador_lerPalavra(contexto_getRegistrador(descritorProcesso_getContexto(*kernel_param->processoRodando),  8));

	int ip0 =  (((r0 & 0xFF000000)/256)/256)/256;
	int ip1 =   ((r0 & 0x00FF0000)/256)/256;
	int ip2 =    (r0 & 0x0000FF00)/256;
	int ip3 =     r0 & 0x000000FF;
	int portaDestino = PORTA_QUALQUER;

	PACOTE_APLICACAO_SOPA *pacote;
	FIFO *parametros;
	OPERACAO_RPC operacao;
	char* ipDestino;

	operacao = r1;
	if(erro == KERNEL_ERRO_NENHUM){
		if(operacao == RPC_OPERACAO_ADD){
			tela_escreverNaColuna(&global_tela, "Solicitando ADD.", 3);
			parametros = (FIFO*) malloc(sizeof(FIFO));
			FIFO_inicializar(parametros, 2);
			int *parametro = (int*) malloc(sizeof(int));
			*parametro = r2;
			FIFO_inserir(parametros, (void*) parametro);
			parametro = (int*) malloc(sizeof(int));
			*parametro = r3;
			FIFO_inserir(parametros, (void*) parametro);
		} else if(operacao == RPC_OPERACAO_RESULTADO){
			tela_escreverNaColuna(&global_tela, "Solicitando RESULTADO_OPERACAO.", 3);
			portaDestino = r2;

			parametros = (FIFO*) malloc(sizeof(FIFO));
			FIFO_inicializar(parametros, 6);
			int *parametro = (int*) malloc(sizeof(int));
			*parametro = r3;
			FIFO_inserir(parametros, (void*) parametro);
			parametro = (int*) malloc(sizeof(int));
			*parametro = r4;
			FIFO_inserir(parametros, (void*) parametro);
			parametro = (int*) malloc(sizeof(int));
			*parametro = r5;
			FIFO_inserir(parametros, (void*) parametro);
			parametro = (int*) malloc(sizeof(int));
			*parametro = r6;
			FIFO_inserir(parametros, (void*) parametro);
			parametro = (int*) malloc(sizeof(int));
			*parametro = r7;
			FIFO_inserir(parametros, (void*) parametro);
			parametro = (int*) malloc(sizeof(int));
			*parametro = r8;
			FIFO_inserir(parametros, (void*) parametro);
		} else {
			erro = KERNEL_ERRO_OPERACAO_RPC_INVALIDA;
		}
	}

	if(erro == KERNEL_ERRO_NENHUM){
		pacote = pacoteAplicacaoSOPA_criarPacoteRPC(operacao, parametros, 
			descritorProcesso_getPID(processoRequerente_param), portaDestino);
		tela_escreverNaColuna(&global_tela, "Pacote Enviado:", 3);
		pacoteAplicacaoSOPA_imprimir(pacote);
		ipDestino = (char*) malloc(3*5*sizeof(char));
		sprintf(ipDestino, "%d.%d.%d.%d", ip0, ip1, ip2, ip3);
//		void* dadosRequerente = ;
		placaRede_agendarEnvioMensagem(&global_placaRede, ipDestino, pacoteAplicacaoSOPA_paraString(pacote), dadosRequerente);
		FIFO_destruir(parametros);
		free(pacote);
		free(ipDestino);
	}

	return erro;
}

/**
* @param KERNEL 					*kernel_param		O kernel que atendará ao pedido.
* @param RPC						*rpc_param			RPC que será atendido.
* @param PACOTE_APLICACAO_SOPA		*pacote_param		Pacote da mensagem que será atendida.
* @return ERRO_KERNEL 	Erro ocorrido durante atendimento do pedido.
*/
ERRO_KERNEL privada_atenderRPC(KERNEL *kernel_param, RPC *rpc_param, PACOTE_APLICACAO_SOPA *pacote_param){
	ERRO_KERNEL erro = KERNEL_ERRO_NENHUM;

	if(rpc_getOperacao(rpc_param) == RPC_OPERACAO_ADD){
		tela_escreverNaColuna(&global_tela, "Atendendo ADD.", 3);
		CONTEXTO *contextoProcesso = (CONTEXTO*) malloc(sizeof(CONTEXTO));
		contexto_inicializar(contextoProcesso);
		contexto_setRegistradorPalavra(contextoProcesso, pacoteAplicacaoSOPA_getPalavraOrigemIP(pacote_param), 0);
		contexto_setRegistradorPalavra(contextoProcesso, pacoteAplicacaoSOPA_getPortaOrigem(pacote_param), 1);
		contexto_setRegistradorPalavra(contextoProcesso, * (int*) rpc_getParametro(rpc_param, 0), 2);
		contexto_setRegistradorPalavra(contextoProcesso, * (int*) rpc_getParametro(rpc_param, 1), 3);
		privada_criarProcesso(kernel_param, RPC_NOME_ARQUIVO_OPERACAO_ADD, contextoProcesso);
	} else if(rpc_getOperacao(rpc_param) == RPC_OPERACAO_RESULTADO){
		tela_escreverNaColuna(&global_tela, "Atendendo RESULTADO_OPERACAO.", 3);
		int PID_processoQueEsperou = pacoteAplicacaoSOPA_getPortaDestino(pacote_param);
		DESCRITOR_PROCESSO *processoQueEsperou = privada_buscaProcessoComPID(kernel_param, PID_processoQueEsperou);
		if(processoQueEsperou == NULL){
			erro = KERNEL_ERRO_PROCESSO_INEXISTENTE;
		} else {
			int resultadoOperacao = * (int*) rpc_getParametro(rpc_param, 0);
			contexto_setRegistradorPalavra(descritorProcesso_getContexto(processoQueEsperou), 0, 0);
			contexto_setRegistradorPalavra(descritorProcesso_getContexto(processoQueEsperou), resultadoOperacao, 1);
			int processo=0;
			int encontrouProcesso=0;
			int haMaisProcessosBloqueadosRPC = processo<MAXIMO_PROCESSOS_KERNEL
				&& processo<FIFO_quantidadeElementos(&kernel_param->filaProcessosBloqueadosRPC);
char mensagem[200];
sprintf(mensagem, "ha %d processos bloqueados", FIFO_quantidadeElementos(&kernel_param->filaProcessosBloqueadosRPC));
tela_escreverNaColuna(&global_tela, mensagem, 3);
			for(; haMaisProcessosBloqueadosRPC; processo++){
				encontrouProcesso = (descritorProcesso_getPID(FIFO_espiarPosicao(&kernel_param->filaProcessosBloqueadosRPC, processo))
									 == PID_processoQueEsperou);
				if(encontrouProcesso){tela_escreverNaColuna(&global_tela, "Encontrou.", 3);
					FIFO_inserir(&kernel_param->filaProcessosProntos, 
								FIFO_removerPosicao(&kernel_param->filaProcessosBloqueadosRPC, processo));
				}
				haMaisProcessosBloqueadosRPC = processo<MAXIMO_PROCESSOS_KERNEL
					&& processo<FIFO_quantidadeElementos(&kernel_param->filaProcessosBloqueadosRPC);
			}
tela_escreverNaColuna(&global_tela, "deu.", 3);
		}
	} else {
		erro = KERNEL_ERRO_OPERACAO_RPC_INVALIDA;
	}

	return erro;
}

/**
* @param KERNEL 					*kernel_param		O kernel que atendará ao pedido.
* @param PACOTE_APLICACAO_SOPA		*pacote_param		Pacote da mensagem que será atendida.
* @return ERRO_KERNEL 	Erro ocorrido durante atendimento do pedido.
*/
ERRO_KERNEL privada_atenderMensagemRede(KERNEL *kernel_param, PACOTE_APLICACAO_SOPA *pacote_param){
	ERRO_KERNEL erro = KERNEL_ERRO_NENHUM;

	tela_escreverNaColuna(&global_tela, "Pacote Recebido:", 3);
	pacoteAplicacaoSOPA_imprimir(pacote_param);
	if(pacoteAplicacaoSOPA_getTipo(pacote_param) == TIPO_PACOTE_APLICACAO_SOPA_RPC){
		erro = privada_atenderRPC(kernel_param, (RPC*) pacoteAplicacaoSOPA_getConteudo(pacote_param), pacote_param);
	}

	return erro;
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
	FIFO_inicializar(&kernel_param->filaProcessosBloqueadosRPC, MAXIMO_PROCESSOS_KERNEL);

	gerenciadorDisco_inicializar(&kernel_param->gerenciadorAcessoDisco, &global_disco);
	sistemaArquivos_inicializarComArquivosDoHospedeiro(&kernel_param->sistemaDeArquivos, &global_disco);
	mapaAlocacoesMemoria_inicializar(&kernel_param->mapaMemoriaAlocada, &global_MMU, MAXIMO_PROCESSOS_KERNEL);

	memoria_sincronizado_escreverBytes(&global_memoria, 1, 'J', 'P', 'A', 0);
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
	char** nomeArquivo;
	OPERACAO_KERNEL* operacaoKernel;
	int numeroDescritorArquivo;
	int erro;

	sprintf(mensagem, "Kernel chamado para a interrupcao %d.", interrupcao_param);
	tela_escreverNaColuna(&global_tela, mensagem, 3);

	contexto_copiar(descritorProcesso_getContexto(*kernel_param->processoRodando),
					processador_getContexto(&global_processador));
	//descritorProcesso_setContexto(*kernel_param->processoRodando, processador_getContexto(&global_processador));
	switch(interrupcao_param){
		case INTERRUPCAO_PROCESSADOR:
			sprintf(mensagem, "O processo %d foi morto porque tentou executar uma instrucao ilegal.", descritorProcesso_getPID(*kernel_param->processoRodando));
			tela_escreverNaColuna(&global_tela, mensagem, 3);
			sprintf(mensagem,  "Imprimindo contexto do DESCRITOR DO PROCESSO imediatamente anterior 'a falha.");
			tela_escreverNaColuna(&global_tela, mensagem, 3);
			contexto_imprimirRegistradores(descritorProcesso_getContexto(*kernel_param->processoRodando), 3);

			privada_matarProcessoRodando(kernel_param);
			privada_escalonar(kernel_param);
			break;
		case INTERRUPCAO_CONSOLE:
			mensagemOperador = console_ultimaLinhaDigitada(&global_console);
			sprintf(mensagem, "Operador digitou: %s", mensagemOperador);
			tela_escreverNaColuna(&global_tela, mensagem, 3);
			privada_executarComandoUsuario(kernel_param, mensagemOperador);
			break;
		case INTERRUPCAO_TIMER:
			descritorProcesso_setFatiaTempo(*kernel_param->processoRodando, descritorProcesso_getFatiaTempo(*kernel_param->processoRodando)+1);
			sprintf(mensagem, "Processo %d roda ha %d de %d ticks.", 
				descritorProcesso_getPID(*kernel_param->processoRodando), 
				descritorProcesso_getFatiaTempo(*kernel_param->processoRodando),
				FATIA_TEMPO_TICKS_PROCESSO);
			tela_escreverNaColuna(&global_tela, mensagem, 3);
			if(descritorProcesso_getFatiaTempo(*kernel_param->processoRodando) == FATIA_TEMPO_TICKS_PROCESSO){
				descritorProcesso_setFatiaTempo(*kernel_param->processoRodando, 0);
				privada_escalonar(kernel_param);
			}
			break;
		case INTERRUPCAO_DISCO:
			if(disco_erroUltimaOperacao(&global_disco) == ERRO_DISCO_ENDERECO_INVALIDO){
				tela_imprimirTelaAzulDaMorte(&global_tela, "Alguem tentou acessar um endereco invalido do disco!");
			} else {
				operacaoKernel = gerenciadorDisco_proximaOperacaoKernel(&kernel_param->gerenciadorAcessoDisco);
				if(operacaoKernel->tipoOperacao == TIPO_OPERACAO_AGENDAVEL_CRIACAO_PROCESSO_KERNEL){
					OPERACAO_CRIACAO_PROCESSO_KERNEL* operacaoExecutada = (OPERACAO_CRIACAO_PROCESSO_KERNEL*) operacaoKernel->operacao;
					FIFO_inserir(&kernel_param->filaProcessosProntos, operacaoExecutada->processoCriado);
					kernel_param->quantidadeProcessos++;
				} else if(operacaoKernel->tipoOperacao == TIPO_OPERACAO_AGENDAVEL_FEITA_POR_PROCESSO_KERNEL){
					OPERACAO_FEITA_POR_PROCESSO_KERNEL* operacaoExecutada = (OPERACAO_FEITA_POR_PROCESSO_KERNEL*) operacaoKernel->operacao;
					if(operacaoExecutada->tipoOperacaoFeita == TIPO_OPERACAO_LEITURA_DISCO){
						contexto_setRegistradorPalavra(
							descritorProcesso_getContexto(*operacaoExecutada->processoQuePediu), 
							disco_palavrasUltimaLeituraPosicao(&global_disco, 0), 
							0);
						contexto_setRegistradorPalavra(
							descritorProcesso_getContexto(*operacaoExecutada->processoQuePediu), 
							0, 
							1);
						FIFO_inserir(&kernel_param->filaProcessosProntos, operacaoExecutada->processoQuePediu);
					} else if(operacaoExecutada->tipoOperacaoFeita == TIPO_OPERACAO_ESCRITA_DISCO){
						contexto_setRegistradorPalavra(
							descritorProcesso_getContexto(*operacaoExecutada->processoQuePediu), 
							0, 
							1);
						FIFO_inserir(&kernel_param->filaProcessosProntos, operacaoExecutada->processoQuePediu);
					}
				}

				free(operacaoKernel->operacao);
				free(operacaoKernel);
			}
			gerenciadorDisco_executarProxima(&kernel_param->gerenciadorAcessoDisco);
			break;
		case INTERRUPCAO_ESTOURO_PILHA:
			sprintf(mensagem, "O processo %d foi morto por estouro de pilha.", descritorProcesso_getPID(*kernel_param->processoRodando));
			tela_escreverNaColuna(&global_tela, mensagem, 3);
			sprintf(mensagem,  "Imprimindo contexto do DESCRITOR DO PROCESSO imediatamente anterior 'a falha.");
			tela_escreverNaColuna(&global_tela, mensagem, 3);
			contexto_imprimirRegistradores(descritorProcesso_getContexto(*kernel_param->processoRodando), 3);

			privada_matarProcessoRodando(kernel_param);
			privada_escalonar(kernel_param);
			break;
		case INTERRUPCAO_SEGMENTACAO_MEMORIA:
			sprintf(mensagem, "O processo %d foi morto por falha de segmentacao.", descritorProcesso_getPID(*kernel_param->processoRodando));
			tela_escreverNaColuna(&global_tela, mensagem, 3);
			sprintf(mensagem,  "Imprimindo contexto do DESCRITOR DO PROCESSO imediatamente anterior 'a falha.");
			tela_escreverNaColuna(&global_tela, mensagem, 3);
			contexto_imprimirRegistradores(descritorProcesso_getContexto(*kernel_param->processoRodando), 3);

			privada_matarProcessoRodando(kernel_param);
			privada_escalonar(kernel_param);
			break;
		case INTERRUPCAO_SOFTWARE_EXIT:
			sprintf(mensagem, "O processo %d foi morto voluntariamente.", descritorProcesso_getPID(*kernel_param->processoRodando));
			tela_escreverNaColuna(&global_tela, mensagem, 3);
			sprintf(mensagem,  "Imprimindo contexto do DESCRITOR DO PROCESSO.");
			tela_escreverNaColuna(&global_tela, mensagem, 3);
			contexto_imprimirRegistradores(descritorProcesso_getContexto(*kernel_param->processoRodando), 3);

			privada_matarProcessoRodando(kernel_param);
			privada_escalonar(kernel_param);
			break;
		case INTERRUPCAO_SOFTWARE_KILL:
			sprintf(mensagem, "A interrupcao KILL nao estah implementada.");
			tela_escreverNaColuna(&global_tela, mensagem, 3);
			break;
		case INTERRUPCAO_SOFTWARE_OPEN:
			nomeArquivo = (char**) malloc(sizeof(char*));
			*nomeArquivo = contexto_lerStringDosRegistradores(processador_getContexto(&global_processador), 2, 10);
			numeroDescritorArquivo = 
				privada_abrirArquivo(kernel_param, *nomeArquivo,
					registrador_lerPalavra(contexto_getRegistrador(descritorProcesso_getContexto(*kernel_param->processoRodando), 0)),
					*kernel_param->processoRodando);
			if(numeroDescritorArquivo == NUMERO_DESCRITOR_ARQUIVO_INEXISTENTE){
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(*kernel_param->processoRodando), 1, 1);
			} else {
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(*kernel_param->processoRodando), 
					numeroDescritorArquivo, 0);
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(*kernel_param->processoRodando), 0, 1);
			}
			free(*nomeArquivo);
			free(nomeArquivo);
			break;
		case INTERRUPCAO_SOFTWARE_CLOSE:
			numeroDescritorArquivo = 
				registrador_lerPalavra(contexto_getRegistrador(descritorProcesso_getContexto(*kernel_param->processoRodando), 0));
			sprintf(mensagem, "Fechando arquivo %d.", numeroDescritorArquivo);
			tela_escreverNaColuna(&global_tela, mensagem, 3);
			sistemaArquivos_fecharArquivo(&kernel_param->sistemaDeArquivos, numeroDescritorArquivo);
			break;
		case INTERRUPCAO_SOFTWARE_GET:
			erro = privada_agendarGetPalavraDeArquivoParaProcessoRodando(kernel_param, 
				registrador_lerPalavra(contexto_getRegistrador(descritorProcesso_getContexto(*kernel_param->processoRodando), 0)));
			if(erro == KERNEL_ERRO_NENHUM){
				kernel_param->processoRodando = DESCRITOR_PROCESSO_INEXISTENTE;
				privada_escalonar(kernel_param);
				tela_escreverNaColuna(&global_tela, "GET: leitura agendada.", 3);
			} else if(erro == KERNEL_ERRO_PROCESSO_NAO_TEM_ARQUIVO){
				tela_escreverNaColuna(&global_tela, "GET: arquivo nao estah aberto para o processo.", 3);
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(*kernel_param->processoRodando), 1, 0);
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(*kernel_param->processoRodando), 1, 1);
			} else if(erro == KERNEL_ERRO_FIM_DO_ARQUIVO){
				tela_escreverNaColuna(&global_tela, "GET: fim do arquivo!", 3);
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(*kernel_param->processoRodando), 0, 0);
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(*kernel_param->processoRodando), 1, 1);
			} else if(erro == KERNEL_ERRO_ARQUIVO_ABERTO_COM_OUTRO_FIM){
				tela_escreverNaColuna(&global_tela, "GET: o arquivo nao foi aberto para leitura.", 3);
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(*kernel_param->processoRodando), 2, 0);
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(*kernel_param->processoRodando), 1, 1);
			} else {
				tela_escreverNaColuna(&global_tela, "GET: erro desconhecido.", 3);
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(*kernel_param->processoRodando), 2, 0);
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(*kernel_param->processoRodando), 1, 1);
			}
			break;
		case INTERRUPCAO_SOFTWARE_PUT:
			erro = privada_agendarPutPalavraDeArquivoParaProcessoRodando(kernel_param, 
				registrador_lerPalavra(contexto_getRegistrador(descritorProcesso_getContexto(*kernel_param->processoRodando), 0)),
				registrador_lerPalavra(contexto_getRegistrador(descritorProcesso_getContexto(*kernel_param->processoRodando), 1)));
			if(erro == KERNEL_ERRO_NENHUM){
				kernel_param->processoRodando = DESCRITOR_PROCESSO_INEXISTENTE;
				privada_escalonar(kernel_param);
				tela_escreverNaColuna(&global_tela, "PUT: escrita agendada.", 3);
			} else if(erro == KERNEL_ERRO_PROCESSO_NAO_TEM_ARQUIVO){
				tela_escreverNaColuna(&global_tela, "PUT: arquivo nao estah aberto para o processo.", 3);
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(*kernel_param->processoRodando), 1, 0);
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(*kernel_param->processoRodando), 1, 1);
			} else if(erro == KERNEL_ERRO_MEMORIA_DISCO_INSUFICIENTE){
				tela_escreverNaColuna(&global_tela, "PUT: nao ha espaco suficiente no disco!", 3);
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(*kernel_param->processoRodando), 2, 0);
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(*kernel_param->processoRodando), 1, 1);
			} else if(erro == KERNEL_ERRO_ARQUIVO_ABERTO_COM_OUTRO_FIM){
				tela_escreverNaColuna(&global_tela, "PUT: o arquivo nao foi aberto para escrita.", 3);
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(*kernel_param->processoRodando), 2, 0);
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(*kernel_param->processoRodando), 1, 1);
			} else {
				tela_escreverNaColuna(&global_tela, "PUT: erro desconhecido.", 3);
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(*kernel_param->processoRodando), 2, 0);
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(*kernel_param->processoRodando), 1, 1);
			}
			break;
		case INTERRUPCAO_SOFTWARE_READ:
			sprintf(mensagem, "A interrupcao READ nao estah implementada.");
			tela_escreverNaColuna(&global_tela, mensagem, 3);
			break;
		case INTERRUPCAO_SOFTWARE_WRITE:
			sprintf(mensagem, "A interrupcao WRITE nao estah implementada.");
			tela_escreverNaColuna(&global_tela, mensagem, 3);
			break;
		case INTERRUPCAO_SOFTWARE_SEMINIT:
			sprintf(mensagem, "A interrupcao SEMINIT nao estah implementada.");
			tela_escreverNaColuna(&global_tela, mensagem, 3);
			break;
		case INTERRUPCAO_SOFTWARE_P:
			sprintf(mensagem, "A interrupcao P nao estah implementada.");
			tela_escreverNaColuna(&global_tela, mensagem, 3);
			break;
		case INTERRUPCAO_SOFTWARE_V:
			sprintf(mensagem, "A interrupcao V nao estah implementada.");
			tela_escreverNaColuna(&global_tela, mensagem, 3);
			break;
		case INTERRUPCAO_SOFTWARE_PRINT:
			sprintf(mensagem, "%c %c %c %c", 
				(((registrador_lerPalavra(contexto_getRegistrador(
					descritorProcesso_getContexto(*kernel_param->processoRodando), 0)) & 0xFF000000)/256)/256)/256,
				((registrador_lerPalavra(contexto_getRegistrador(
					descritorProcesso_getContexto(*kernel_param->processoRodando), 0)) & 0x00FF0000)/256)/256,
				(registrador_lerPalavra(contexto_getRegistrador(
					descritorProcesso_getContexto(*kernel_param->processoRodando), 0)) & 0x0000FF00)/256,
				registrador_lerPalavra(contexto_getRegistrador(
					descritorProcesso_getContexto(*kernel_param->processoRodando), 0)) & 0x000000FF);
			tela_escreverNaColuna(&global_tela, mensagem, 5);
			break;
		case INTERRUPCAO_SOFTWARE_REQUISICAO_RPC:
			erro = privada_solicitarRPC(kernel_param, *kernel_param->processoRodando);
			if(erro == KERNEL_ERRO_NENHUM){
				tela_escreverNaColuna(&global_tela, "RPC: chamada solicitada.", 3);
				FIFO_inserir(&kernel_param->filaProcessosBloqueadosRPC, kernel_param->processoRodando);
				kernel_param->processoRodando = DESCRITOR_PROCESSO_INEXISTENTE;
				privada_escalonar(kernel_param);
			} else if(erro == KERNEL_ERRO_OPERACAO_RPC_INVALIDA){
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(
					FIFO_espiar(&kernel_param->filaProcessosBloqueadosRPC)), 1, 0);
				tela_escreverNaColuna(&global_tela, "RPC: a operacao eh invalida.", 3);
			} else {
				tela_escreverNaColuna(&global_tela, "RPC: ocorreu um erro desconhecido.", 3);
			}
			break;
		case INTERRUPCAO_PLACA_REDE_SEND:
			placaRede_esperarAcessoDados(&global_placaRede);

			if(placaRede_getErroUltimaOperacao(&global_placaRede) == ERRO_REDE_NENHUM){
				tela_escreverNaColuna(&global_tela, "Mensagem enviada. Fica a pergunta: quem queria envia-la mesmo?", 3);
			} else if(placaRede_getErroUltimaOperacao(&global_placaRede) == ERRO_REDE_CRIACAO_SOCKET){
				tela_escreverNaColuna(&global_tela, "SEND: erro na criacao do SOCKET.", 3);
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(
					FIFO_espiar(&kernel_param->filaProcessosBloqueadosRPC)), 2, 0);
			} else if(placaRede_getErroUltimaOperacao(&global_placaRede) == ERRO_REDE_CONEXAO_SERVIDOR){
				tela_escreverNaColuna(&global_tela, "SEND: erro na conexao com o SERVIDOR.", 3);
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(
					FIFO_espiar(&kernel_param->filaProcessosBloqueadosRPC)), 3, 0);
			} else if(placaRede_getErroUltimaOperacao(&global_placaRede) == ERRO_REDE_ENVIO_DADOS){
				tela_escreverNaColuna(&global_tela, "SEND: erro na efetivacao do ENVIO.", 3);
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(
					FIFO_espiar(&kernel_param->filaProcessosBloqueadosRPC)), 4, 0);
			} else if(placaRede_getErroUltimaOperacao(&global_placaRede) != ERRO_REDE_NENHUM){
				tela_escreverNaColuna(&global_tela, "SEND: corram para as colinas! erro DESCONHECIDO!", 3);
				contexto_setRegistradorPalavra(descritorProcesso_getContexto(
					FIFO_espiar(&kernel_param->filaProcessosBloqueadosRPC)), 5, 0);
			}

			if(placaRede_getErroUltimaOperacao(&global_placaRede) != ERRO_REDE_NENHUM
					OR ){
				FIFO_inserir(&kernel_param->filaProcessosProntos, FIFO_remover(&kernel_param->filaProcessosBloqueadosRPC));
			}
			break;
		case INTERRUPCAO_PLACA_REDE_RECEIVE:
			tela_escreverNaColuna(&global_tela, "Acabo de receber uma mensagem via rede. Mensagem:", 3);
			tela_escreverNaColuna(&global_tela, 
				pacoteAplicacaoSOPA_paraString(
					placaRede_getProximoPacoteRecebido(&global_placaRede)), 3);
			erro = privada_atenderMensagemRede(kernel_param, placaRede_getProximoPacoteRecebido(&global_placaRede));
			if(erro == KERNEL_ERRO_NENHUM){
				tela_escreverNaColuna(&global_tela, "A mensagem foi atendida com sucesso.", 3);
			} else {
				privada_imprimirErro(erro);
			}
			placaRede_avancarFilaPacotesRecebidos(&global_placaRede);
			break;
		default:
			tela_escreverNaColuna(&global_tela, "Interrupcao desconhecida.", 3);
	}
	contexto_copiar(processador_getContexto(&global_processador),
					descritorProcesso_getContexto(*kernel_param->processoRodando));
//	processador_setContexto(&global_processador, descritorProcesso_getContexto(*kernel_param->processoRodando));
}






