#include "../include/DadosComuns.h"

/**
* Variáveis globais acessíveis somente neste arquivo.
*/



//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------
/**
* @param DISCO		*disco_param		O disco em que será gravado.
* @param int		posicao_param		A posição do disco que será testada.
* @return int	Indica se a posição passada está livre. Se ela pode ser sobrescrita.
*/
int privada_posicaoEstahLivre(DISCO *disco_param, int posicao_param){
	if(disco_param->conteudo[posicao_param] == POSICAO_VAZIA){
		return 1;
	} else {
		return 0;
	}
}

/**
* Escreve os bytes na primeira palavra livre do disco.
* Corre o risco de não escrever os dados em posições contíguas quando chamada sucessivamente.
* @param DISCO		*disco_param		O disco em que será gravado.
* @param BYTE		byte0_param			O byte que ficará na posição 0 da palavra, que conterá o bit mais significativo da palavra.
* @param BYTE		byte1_param			O byte que ficará na posição 1 da palavra.
* @param BYTE		byte2_param			O byte que ficará na posição 2 da palavra.
* @param BYTE		byte3_param			O byte que ficará na posição 3 da palavra.
*/
void privada_escreverNaProximaPalavraLivre(DISCO *disco_param, BYTE byte0_param, BYTE byte1_param, BYTE byte2_param, BYTE byte3_param){
	int primeiraPosicaoLivre=0;
	int posicaoDisco=0;
	while(!privada_posicaoEstahLivre(disco_param, posicaoDisco) && posicaoDisco<TAMANHO_DISCO_PALAVRAS){
		posicaoDisco++;
	}
	primeiraPosicaoLivre = posicaoDisco;
	disco_param->conteudo[primeiraPosicaoLivre] = 
		((((byte0_param & 0xFF000000)/256)/256)/256)
		| (((byte1_param & 0x00FF0000)/256)/256)
		| ((byte2_param & 0x0000FF00)/256)
		| (byte3_param & 0x000000FF);
}

//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/**
* Inicia thread do disco.
* @param DISCO	*disco_param	O disco que irá 'rodar'.
*/
void disco_rodar(DISCO *disco_param){
	sem_init(&disco_param->mutexAcessoDisco, 0, 1);
	sem_wait(&disco_param->mutexAcessoDisco);
	int posicaoDisco=0;
	for(; posicaoDisco<TAMANHO_DISCO_PALAVRAS; posicaoDisco++){
		disco_param->conteudo[posicaoDisco] = POSICAO_VAZIA;
	}
	disco_carregar(disco_param, CAMINHO_ARQUIVO_BASE_DISCO);

	while(1){
		sem_wait(&disco_param->mutexAcessoDisco);
		controladorInterrupcoes_set(&global_controladorInterrupcoes, INTERRUPCAO_DISCO);
		tela_escreverNaColuna(&global_tela, "Mensagem do DISCO.",4);
	}
}

/**
* Libera o disco para dar uma volta.
* @param DISCO	*disco_param	O disco em que a operação será realizada.
*/
void disco_darUmaVolta(DISCO *disco_param){
	sem_post(&disco_param->mutexAcessoDisco);
}

/**
* Carrega os dados do arquivo, juntando-os aos dados que estão no disco.
* Os dados são colocados à partir da primeira posição livre neste disco.
* @param DISCO	*disco_param			O disco em que a operação será realizada.
* @param char*	*caminhoArquivo_param	Arquivo que contém os dados, formatados da seguinte forma:
*										J P A 0
*										255 255 255 255
*										0 0 0 0
*/
void disco_carregar(DISCO *disco_param, char *caminhoArquivo_param){
	int posicaoPalavra = 0;
	int palavra[TAMANHO_INSTRUCAO_PALAVRAS];
	int posicaoEscrita=0;
	char linha[200];
	char* byte;
	char* palavraBytes[TAMANHO_INSTRUCAO_PALAVRAS];
	char mensagem[200];
	FILE *arquivoLido;
	arquivoLido = fopen(caminhoArquivo_param, "r");
	if(arquivoLido == NULL){
		sprintf(mensagem, "Nao consegui carregar o arquivo '%s'.", caminhoArquivo_param);
		tela_escreverNaColuna(&global_tela, mensagem, 4);
	} else {
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
			tela_escreverNaColuna(&global_tela, mensagem, 4);
			privada_escreverNaProximaPalavraLivre(disco_param, palavra[0], palavra[1], palavra[2], palavra[3]);
			sprintf(mensagem, "Lido '%s %s %s %s', gravado '%d'.", 
				palavraBytes[0], palavraBytes[1], palavraBytes[2], palavraBytes[3], disco_param->conteudo[posicaoEscrita]);
			posicaoEscrita++;
		}
	}
}
