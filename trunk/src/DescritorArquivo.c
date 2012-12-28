#include "../include/DadosComuns.h"

/**
* Variáveis globais acessíveis somente neste arquivo.
*/



//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------
/**
* @param DESCRITOR_ARQUIVO	*descritor_param	Arquivo em que a operação será realizada.
* @param char*				nome_param					Nome que o arquivo terá.
* ATENÇÃO: deve ser usada uma só vez!
*/
void privada_setNome(DESCRITOR_ARQUIVO *descritor_param, char* nome_param){
	descritor_param->nome = (char*) malloc((strlen(nome_param)+1)*sizeof(char));
	strcpy(descritor_param->nome, nome_param);
}

/**
* @param DESCRITOR_ARQUIVO		*arquivo_param			O arquivo que será consultado.
* @param ARQUIVO				*segmento_param			O segmento cujo endereço lógico será consultado.
* @return int	Endereço lógico do segmento no arquivo. Se o segmento não existir, retornará DESCRITOR_ARQUIVO_POSICAO_INEXISTENTE.
*/
int privada_getEnderecoLogicoArquivo(DESCRITOR_ARQUIVO *arquivo_param, ARQUIVO *segmento_param){
	int enderecoLogicoDoSegmento = 0;

	ARQUIVO *segmentoAtual;
	int arquivoAtual=0;
	int encontrouArquivo=0;

	while(!encontrouArquivo && arquivoAtual < FIFO_quantidadeElementos(&arquivo_param->segmentos)){
		segmentoAtual = (ARQUIVO*) FIFO_espiarPosicao(&arquivo_param->segmentos, arquivoAtual);
		encontrouArquivo = (arquivo_getEnderecoInicial(segmentoAtual) == arquivo_getEnderecoInicial(segmento_param));
		if(!encontrouArquivo){
			enderecoLogicoDoSegmento += arquivo_getTamanhoEmPalavras(segmentoAtual);
		}
		arquivoAtual++;
	}

	if(!encontrouArquivo){
		enderecoLogicoDoSegmento = DESCRITOR_ARQUIVO_POSICAO_INEXISTENTE;
	}

	return enderecoLogicoDoSegmento;
}

/**
* @param DESCRITOR_ARQUIVO		*arquivo_param			O arquivo que será consultado.
* @param int					enderecoLogico_param	Ponteiro lógico para alguma palavra do arquivo.
* @return ARQUIVO*	O segmento que contém a posição lógica do arquivo. Se não houver, retornará DESCRITOR_ARQUIVO_SEGMENTO_INEXISTENTE.
*/
ARQUIVO* privada_getSegmentoComEnderecoLogico(DESCRITOR_ARQUIVO *arquivo_param, int enderecoLogico_param){
	ARQUIVO *segmentoAtual;
	ARQUIVO *segmentoComEndereco = DESCRITOR_ARQUIVO_SEGMENTO_INEXISTENTE;
	int arquivoAtual=0;
	int enderecoLogicoAtual=0;

	while(segmentoComEndereco == DESCRITOR_ARQUIVO_SEGMENTO_INEXISTENTE
				&& arquivoAtual < FIFO_quantidadeElementos(&arquivo_param->segmentos)){
		segmentoAtual = (ARQUIVO*) FIFO_espiarPosicao(&arquivo_param->segmentos, arquivoAtual);
		enderecoLogicoAtual += arquivo_getTamanhoEmPalavras(segmentoAtual);
		if(enderecoLogico_param < enderecoLogicoAtual){
			segmentoComEndereco = segmentoAtual;
		}
		arquivoAtual++;
	}

	return segmentoComEndereco;
}

//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/**
* @param DESCRITOR_ARQUIVO	*descritor_param	O descritor de arquivo que será inicializado.
* @param char*				nome_param			Nome que será associado ao descritor, visível ao usuário.
* @param int		numeroDescritor_param	Número usado por processos do SOPA para ler e escrever.
*/
void descritorArquivo_inicializar(DESCRITOR_ARQUIVO *descritor_param, char* nome_param, int numeroDescritor_param){
	FIFO_inicializar(&descritor_param->segmentos, MAXIMO_SEGMENTOS_DESCRITOR_ARQUIVO);
	descritor_param->palavraAtual = 0;
	descritor_param->numeroDescritor = numeroDescritor_param;
	descritor_param->processoQueAbriu = NULL;
	privada_setNome(descritor_param, nome_param);
}

/**
* @param DESCRITOR_ARQUIVO	*descritor_param	O descritor de arquivo ao qual o segmento será adicionado.
* @param ARQUIVO			*segmento_param		O segmento que será adicionado ao descritor.
*/
void descritorArquivo_adicionarSegmento(DESCRITOR_ARQUIVO *descritor_param, ARQUIVO *segmento_param){
	if(FIFO_quantidadeElementos(&descritor_param->segmentos) < MAXIMO_SEGMENTOS_DESCRITOR_ARQUIVO){
		FIFO_inserir(&descritor_param->segmentos, segmento_param);
	} else {
		tela_imprimirTelaAzulDaMorte(&global_tela, "Nao eh possivel adicionar mais segmentos ao arquivo em descritorArquivo_adicionarSegmento.");
	}
}

/**
* @param DESCRITOR_ARQUIVO	*descritor_param	O arquivo lógico cuja informação será retornada.
* @return int	O número de segmentos do arquivo.
*/
int descritorArquivo_getQuantidadeSegmentos(DESCRITOR_ARQUIVO *descritor_param){
	return FIFO_quantidadeElementos(&descritor_param->segmentos);
}

/**
* @param DESCRITOR_ARQUIVO	*descritor_param	O arquivo lógico cuja informação será retornada.
* @param int				segmento_param		A ordem do segmento que deve ser retornado.
* @return ARQUIVO*	O arquivo na ordem especificada deste arquivo lógico.
*/
ARQUIVO* descritorArquivo_getSegmento(DESCRITOR_ARQUIVO *descritor_param, int segmento_param){
	return FIFO_espiarPosicao(&descritor_param->segmentos, segmento_param);
}

/**
* @param DESCRITOR_ARQUIVO	*descritor_param	O arquivo lógico cuja informação será retornada.
* @return int	Número usado por processos do SOPA para ler e escrever.
*/
int descritorArquivo_getNumeroDescritor(DESCRITOR_ARQUIVO *descritor_param){
	return descritor_param->numeroDescritor;
}

/**
* @param DESCRITOR_ARQUIVO	*descritor_param		O descritor de arquivo que será desfragmentado.
* @param int				posicaoInicial_param	A posição do disco à partir da qual o arquivo será guardado.
* ATENÇÃO: colocará todos os segmentos do arquivo contiguamente! Certifique-se de que há espaço antes de chamar!
*/
void descritorArquivo_desfragmentar(DESCRITOR_ARQUIVO *descritor_param, int posicaoInicial_param){
	ARQUIVO **segmento = (ARQUIVO**) malloc(sizeof(ARQUIVO*));
	int arquivoDesfragmentado=0;
	int posicaoAtual = posicaoInicial_param;
	for(; arquivoDesfragmentado<FIFO_quantidadeElementos(&descritor_param->segmentos); arquivoDesfragmentado++){
		*segmento = (ARQUIVO*) FIFO_espiarPosicao(&descritor_param->segmentos, arquivoDesfragmentado);
		arquivo_relocalizar(*segmento, posicaoAtual);
		posicaoAtual += arquivo_getTamanhoEmPalavras(*segmento);
	}
	free(segmento);
}

/**
* @param DESCRITOR_ARQUIVO	*descritor_param	O arquivo lógico cuja informação será retornada.
* @return int	Quantidade total de palavras ocupadas pelo descritor.
*/
int descritorArquivo_tamanhoEmPalavras(DESCRITOR_ARQUIVO *descritor_param){
	int arquivoAtual=0;
	int tamanhoEmPalavras=0;
	for(; arquivoAtual<FIFO_quantidadeElementos(&descritor_param->segmentos); arquivoAtual++){
		tamanhoEmPalavras += arquivo_getTamanhoEmPalavras(FIFO_espiarPosicao(&descritor_param->segmentos, arquivoAtual));
	}
	return tamanhoEmPalavras;
}

/**
* @param DESCRITOR_ARQUIVO	*descritor_param	Arquivo que será atualizado na máquina hospedeira.
* @param char*				caminho_param		Caminho do arquivo na máquina hospedeira.
*/
void descritorArquivo_atualizarNaMaquinaHospedeira(DESCRITOR_ARQUIVO *descritor_param, char* caminho_param){
	char* conteudoArquivoLogico = (char*) malloc((descritorArquivo_tamanhoEmPalavras(descritor_param)+1)*sizeof(char));
	char* conteudoSegmento;
	int arquivoAtual=0;

	memset(conteudoArquivoLogico, '\0', (descritorArquivo_tamanhoEmPalavras(descritor_param)+1));
	for(; arquivoAtual<FIFO_quantidadeElementos(&descritor_param->segmentos); arquivoAtual++){
		conteudoSegmento = arquivo_getConteudo(FIFO_espiarPosicao(&descritor_param->segmentos, arquivoAtual));
		strcat(conteudoArquivoLogico, conteudoSegmento);
		free(conteudoSegmento);
	}

	FILE *arquivoMaquinaHospedeira;
	char mensagem[200];
	arquivoMaquinaHospedeira = fopen(caminho_param, "w");
	if(arquivoMaquinaHospedeira != NULL){
		fprintf(arquivoMaquinaHospedeira, "%s", conteudoArquivoLogico);
		fclose(arquivoMaquinaHospedeira);
	} else {
		sprintf(mensagem, "Nao consegui salvar o arquivo %s.", descritor_param->nome);
		tela_escreverNaColuna(&global_tela, mensagem, 4);
	}
	free(conteudoArquivoLogico);
}

/**
* @param DESCRITOR_ARQUIVO	*descritor_param	O arquivo lógico cuja informação será retornada.
* @return char* 	Nome que será associado ao descritor, visível ao usuário.
*/
char* descritorArquivo_getNome(DESCRITOR_ARQUIVO *descritor_param){
	return descritor_param->nome;
}

/**
* @param DESCRITOR_ARQUIVO	*descritor_param	O arquivo que será testado.
* @return int	Indica se o arquivo está fragmentado no disco.
*/
int descritorArquivo_estahFragmentado(DESCRITOR_ARQUIVO *descritor_param){
	int estahFragmentado = 0;
	if(1 < FIFO_quantidadeElementos(&descritor_param->segmentos)){
		estahFragmentado = 1;
	}
	return estahFragmentado;
}

/**
* @param DESCRITOR_ARQUIVO	*arquivo_param	O arquivo cuja informação será retornada.
* @return DESCRITOR_PROCESSO*	O processo que abriu este arquivo. Caso o arquivo não tenha sido aberto, retornará NULL.
*/
DESCRITOR_PROCESSO* descritorArquivo_getProcessoQueAbriu(DESCRITOR_ARQUIVO *arquivo_param){
	return arquivo_param->processoQueAbriu;
}

/**
* @param DESCRITOR_ARQUIVO			*arquivo_param		O arquivo cuja informação será retornada.
* @param DESCRITOR_PROCESSO			*processo_param		O processo que abrirá o arquivo.
* @param OPCAO_ABERTURA_ARQUIVO		opcao_param			O que será feito com o arquivo.
* ATENÇÃO: não checa se o arquivo já está aberto!
*/
void descritorArquivo_abrirParaProcesso(DESCRITOR_ARQUIVO *arquivo_param, DESCRITOR_PROCESSO *processo_param,
		OPCAO_ABERTURA_ARQUIVO opcao_param){
	arquivo_param->palavraAtual = 0;
	arquivo_param->processoQueAbriu = processo_param;
	arquivo_param->opcaoAbertura = opcao_param;
}

/**
* @param DESCRITOR_ARQUIVO		*arquivo_param		O arquivo que será fechado.
*/
void descritorArquivo_fechar(DESCRITOR_ARQUIVO *arquivo_param){
	char mensagem[200];
	sprintf(mensagem, "Fechando arquivo %s.", arquivo_param->nome);
	tela_escreverNaColuna(&global_tela, mensagem, 3);
	arquivo_param->processoQueAbriu = NULL;
}

/**
* @param DESCRITOR_ARQUIVO		*arquivo_param		O arquivo que será consultado.
* @return int	Palavra sendo lida/escrita pelo usuário. Se ultrapassar o limite, retonará DESCRITOR_ARQUIVO_POSICAO_INEXISTENTE.
*/
int descritorArquivo_getPalavraAtual(DESCRITOR_ARQUIVO *arquivo_param){
	int ehPossivelLerEscrever = (arquivo_param->palavraAtual < descritorArquivo_tamanhoEmPalavras(arquivo_param));
	if(ehPossivelLerEscrever){
		return arquivo_param->palavraAtual;
	} else {
		return DESCRITOR_ARQUIVO_POSICAO_INEXISTENTE;
	}
}

/**
* @param DESCRITOR_ARQUIVO		*arquivo_param		O arquivo que será consultado.
* @param int					palavraAtual_param	Palavra sendo lida/escrita pelo usuário.
*/
void descritorArquivo_setPalavraAtual(DESCRITOR_ARQUIVO *arquivo_param, int palavraAtual_param){
	arquivo_param->palavraAtual = palavraAtual_param;
}

/**
* @param DESCRITOR_ARQUIVO		*arquivo_param			O arquivo que será consultado.
* @param int					enderecoLogico_param	Ponteiro lógico para alguma palavra do arquivo.
* @param int	Endereço da palavra no disco. Caso não haja, retornará DISCO_ENDERECO_INEXISTENTE.
*/
int descritorArquivo_getEnderecoDiscoPosicao(DESCRITOR_ARQUIVO *arquivo_param, int enderecoLogico_param){
	int enderecoDisco = DISCO_ENDERECO_INEXISTENTE;
	ARQUIVO *segmentoComEndereco = privada_getSegmentoComEnderecoLogico(arquivo_param, enderecoLogico_param);
	if(segmentoComEndereco != DESCRITOR_ARQUIVO_SEGMENTO_INEXISTENTE){
		enderecoDisco = enderecoLogico_param - privada_getEnderecoLogicoArquivo(arquivo_param, segmentoComEndereco);
		enderecoDisco += arquivo_getEnderecoInicial(segmentoComEndereco);
	}
	return enderecoDisco;
}

/**
* @param DESCRITOR_ARQUIVO			*arquivo_param			O arquivo que será consultado.
* @param OPCAO_ABERTURA_ARQUIVO		opcao_param			Opção que será testada.
* @return int	Indica se o arquivo está aberto com a opção dada.
*/
int descritorArquivo_abertoComOpcao(DESCRITOR_ARQUIVO *arquivo_param, OPCAO_ABERTURA_ARQUIVO opcao_param){
	int estahAbertoComOpcao = 0;
	if(arquivo_param->processoQueAbriu != NULL && arquivo_param->opcaoAbertura == opcao_param){
		estahAbertoComOpcao = 1;
	}
	return estahAbertoComOpcao;
}







