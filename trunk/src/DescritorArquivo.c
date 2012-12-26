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
* @param int				segmento_param		A ordem do segmento que deve ser retornado.
* @return ARQUIVO*	O arquivo na ordem especificada deste arquivo lógico.
*/
ARQUIVO* descritorArquivo_getSegmento(DESCRITOR_ARQUIVO *descritor_param, int segmento_param){
	return FIFO_espiarPosicao(&descritor_param->segmentos, segmento_param);
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

