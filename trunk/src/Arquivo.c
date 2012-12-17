#include "../include/DadosComuns.h"

/**
* Variáveis globais acessíveis somente neste arquivo.
*/



//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------
/**
* @param ARQUIVO	*arquivo_param				Arquivo em que a operação será realizada.
* @param char*		nome_param					Nome que o arquivo terá.
* ATENÇÃO: deve ser usada uma só vez!
*/
void privada_setNome(ARQUIVO *arquivo_param, char* nome_param){
	arquivo_param->nome = (char*) malloc(strlen(nome_param)*sizeof(char));
	strcpy(arquivo_param->nome, nome_param);
}

/**
* @param ARQUIVO	*arquivo_param				Arquivo em que a operação será realizada.
* @return PALAVRA*	Um vetor com as palavras que foram lidas.
* ATENÇÃO: deve estar presente na memória.
*/
PALAVRA* privada_lerDaMemoria(ARQUIVO *arquivo_param){
	if(arquivo_param->memoriaSalvo != NULL){
		PALAVRA* dadosLidos = (PALAVRA*) malloc(arquivo_param->tamanhoEmPalavras*sizeof(PALAVRA));
		int palavraLida;
		for(palavraLida=0; palavraLida<arquivo_param->tamanhoEmPalavras; palavraLida++){
			MMU_sincronizado_lerFisico(arquivo_param->memoriaSalvo, arquivo_param->enderecoInicioMemoria+palavraLida, &dadosLidos[palavraLida]);
		}
		return dadosLidos;
	} else {
		return NULL;
	}
}

//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/**
* Cria um novo arquivo na memória.
* @param ARQUIVO	*arquivo_param				Arquivo que será criado.
* @param MMU		*MMU_param					A memória em que o arquivo será salvo.
* @param int		enderecoFisicoInicio_param	Endereço físico da memória que iniciará o arquivo.
* @param int		tamanhoEmPalavras_param		Tamanho do arquivo em palavras da memória.
* @param char*		nome_param					O nome que o arquivo terá.
*/
void arquivo_criar(ARQUIVO *arquivo_param, MMU *MMU_param, int enderecoFisicoInicio_param, int tamanhoEmPalavras_param, char* nome_param){
	arquivo_param->memoriaSalvo = MMU_param;
	arquivo_param->enderecoInicioMemoria = enderecoFisicoInicio_param;
	arquivo_param->tamanhoEmPalavras = tamanhoEmPalavras_param;
	privada_setNome(arquivo_param, nome_param);
	
	arquivo_param->blocoInicioDisco = -1;
	arquivo_param->tamanhoEmBlocos = -1;
	arquivo_param->discoSalvo = NULL;
}

/**
* Cria um arquivo à partir de um arquivo existente e salvo na máquina hospedeira.
* O novo arquivo é salvo no disco.
* @param ARQUIVO	*arquivo_param				Arquivo que será criado.
* @param DISCO		*disco_param				O disco em que o arquivo será salvo.
* @param char*		nome_param					O nome que será dado ao arquivo no SOPA.
* @param char*		caminho_param				Caminho do arquivo na máquina hospedeira.
* @param int		blocoInicio_param			Bloco do disco que iniciará o arquivo.
* @return int	Indica se foi possível ler o arquivo.
*/
int arquivo_lerDaMaquinaHospedeira(ARQUIVO *arquivo_param, DISCO *disco_param, char* nome_param, char* caminho_param, int blocoInicio_param){
	int conseguiuLer = 0;
	char mensagem[200];

	arquivo_param->memoriaSalvo = NULL;
	arquivo_param->enderecoInicioMemoria = -1;
	arquivo_param->tamanhoEmPalavras = -1;

	privada_setNome(arquivo_param, nome_param);

	int posicaoPalavra = 0;
	int palavra[TAMANHO_INSTRUCAO_PALAVRAS];
	int posicaoEscrita=0;
	char linha[200];
	char* byte;
	char* palavraBytes[TAMANHO_INSTRUCAO_PALAVRAS];
	FILE *arquivoLido = fopen(caminho_param, "r");
	if(arquivoLido != NULL){
		conseguiuLer = 1;
		arquivo_param->blocoInicioDisco = blocoInicio_param;
		
		while(fgets(linha, 200, arquivoLido)){
			byte=strtok(linha, " \n");
			posicaoPalavra = 0;
			do{
				if(byte != NULL){
					palavraBytes[posicaoPalavra] = byte;
					posicaoPalavra++;
				}
			}while(byte=strtok(NULL," \n"));
			palavra[0] = string_paraInt(palavraBytes[0]);
			palavra[1] = string_paraInt(palavraBytes[1]);
			palavra[2] = string_paraInt(palavraBytes[2]);
			palavra[3] = string_paraInt(palavraBytes[3]);
			disco_inicializarPosicao(disco_param, arquivo_param->blocoInicioDisco*TAMANHO_BLOCO+posicaoEscrita, 
				palavra[0], palavra[1], palavra[2], palavra[3]);
			sprintf(mensagem, "File *(%d)=%s %s %s %s", posicaoEscrita, palavraBytes[0], palavraBytes[1], palavraBytes[2], palavraBytes[3]);
			tela_escreverNaColuna(&global_tela, mensagem, 4);
			sprintf(mensagem, "=%d %d %d %d", palavra[0], palavra[1], palavra[2], palavra[3]);
			tela_escreverNaColuna(&global_tela, mensagem, 4);
			sprintf(mensagem, "Disco *(%d)=%d", arquivo_param->blocoInicioDisco*TAMANHO_BLOCO+posicaoEscrita, 
				disco_param->conteudo[arquivo_param->blocoInicioDisco*TAMANHO_BLOCO+posicaoEscrita]);
			tela_escreverNaColuna(&global_tela, mensagem, 4);
			sprintf(mensagem, "=%d %d %d %d", 
				(((disco_param->conteudo[arquivo_param->blocoInicioDisco*TAMANHO_BLOCO+posicaoEscrita] & 0xFF000000)/256)/256)/256,
				((disco_param->conteudo[arquivo_param->blocoInicioDisco*TAMANHO_BLOCO+posicaoEscrita] & 0x00FF0000)/256)/256,
				(disco_param->conteudo[arquivo_param->blocoInicioDisco*TAMANHO_BLOCO+posicaoEscrita] & 0x0000FF00)/256,
				disco_param->conteudo[arquivo_param->blocoInicioDisco*TAMANHO_BLOCO+posicaoEscrita] & 0x000000FF);
			tela_escreverNaColuna(&global_tela, mensagem, 4);
			posicaoEscrita++;
		}
		
		arquivo_param->tamanhoEmBlocos = posicaoEscrita/TAMANHO_BLOCO + 1;
		arquivo_param->discoSalvo = disco_param;
	}

	return conseguiuLer;
}

/**
* @param ARQUIVO	*arquivo_param				Arquivo sobre o qual a operação será realizada.
* @param MMU		*MMU_param					Memória na qual o arquivo será salvo.
* @param int		enderecoFisicoInicio_param	Endereço físico à partir do qual o arquivo será salvo.
* @return int	Indica se foi possível.
* ATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃO
* ATENÇÃO: o arquivo deve estar no buffer de leitura do disco!!!!!!!
* ATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃO
*/
int arquivo_transferirParaMemoria(ARQUIVO *arquivo_param, MMU *MMU_param, int enderecoFisicoInicio_param){
	int transferiu = 0;
	char mensagem[200];

	if(arquivo_param->discoSalvo != NULL && disco_erroUltimaOperacao(arquivo_param->discoSalvo) == SEM_ERRO_DISCO){
		transferiu = 1;
		arquivo_param->memoriaSalvo = MMU_param;
		arquivo_param->enderecoInicioMemoria = enderecoFisicoInicio_param;
		arquivo_param->tamanhoEmPalavras = disco_tamanhoPalavrasUltimaLeitura(arquivo_param->discoSalvo);
		
		int palavra = 0;
		for(; palavra<disco_tamanhoPalavrasUltimaLeitura(arquivo_param->discoSalvo); palavra++){
			MMU_sincronizado_escreverFisico(MMU_param, enderecoFisicoInicio_param+palavra, 
					disco_palavrasUltimaLeituraPosicao(arquivo_param->discoSalvo, palavra));
			sprintf(mensagem, "*(%d)=%d=", enderecoFisicoInicio_param+palavra, 
				disco_palavrasUltimaLeituraPosicao(arquivo_param->discoSalvo, palavra));
			tela_escreverNaColuna(&global_tela, mensagem, 5);
			sprintf(mensagem, "%d %d %d %d", 
				(((disco_palavrasUltimaLeituraPosicao(arquivo_param->discoSalvo, palavra) & 0xFF000000)/256)/256)/256,
				((disco_palavrasUltimaLeituraPosicao(arquivo_param->discoSalvo, palavra) & 0x00FF0000)/256)/256,
				(disco_palavrasUltimaLeituraPosicao(arquivo_param->discoSalvo, palavra) & 0x0000FF00)/256,
				disco_palavrasUltimaLeituraPosicao(arquivo_param->discoSalvo, palavra) & 0x000000FF);
			tela_escreverNaColuna(&global_tela, mensagem, 5);

			
		}
	}
	return transferiu;
}

/**
* @param ARQUIVO	*arquivo_param				Arquivo sobre o qual a operação será realizada.
* @return int	Indica se foi possível.
* ATENÇÃO: o arquivo deve existir no disco e na memória!
*/
int arquivo_atualizarNoDisco(ARQUIVO *arquivo_param){
	int atualizou=0;
	if(arquivo_param->discoSalvo != NULL && arquivo_param->memoriaSalvo != NULL){
		disco_executarOperacao(arquivo_param->discoSalvo, OPERACAO_ESCRITA_DISCO, arquivo_param->blocoInicioDisco, 
				privada_lerDaMemoria(arquivo_param), arquivo_param->tamanhoEmPalavras);
	}
	return atualizou;
}

/**
* @param ARQUIVO	*arquivo_param	O arquivo cuja informação será retornada.
* @return int	Nome amigável ao usuário, atribuído ao arquivo.
*/
char* arquivo_getNome(ARQUIVO *arquivo_param){
	return arquivo_param->nome;
}

/**
* @param ARQUIVO	*arquivo_param	O arquivo cuja informação será retornada.
* @return int	O tamanho do arquivo em palavras. Retornará -1 caso o arquivo não esteja salvo nem no disco, nem na memória.
*/
int arquivo_getTamanhoEmPalavras(ARQUIVO *arquivo_param){
	if(arquivo_param->memoriaSalvo != NULL){
		return arquivo_param->tamanhoEmPalavras;
	} else if(arquivo_param->discoSalvo != NULL){
		return arquivo_param->tamanhoEmBlocos*TAMANHO_BLOCO;
	} else {
		return -1;
	}
}

/**
* @param ARQUIVO	*arquivo_param	O arquivo cuja informação será retornada.
* @return int	O tamanho do arquivo em blocos. Retornará -1 caso o arquivo não esteja salvo nem no disco, nem na memória.
*/
int arquivo_getTamanhoEmBlocos(ARQUIVO *arquivo_param){
	if(arquivo_param->memoriaSalvo != NULL){
		return arquivo_param->tamanhoEmPalavras/TAMANHO_BLOCO + 1;
	} else if(arquivo_param->discoSalvo != NULL){
		return arquivo_param->tamanhoEmBlocos;
	} else {
		return -1;
	}
}



