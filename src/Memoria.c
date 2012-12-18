#include "../include/DadosComuns.h"
/**
* Variáveis globais acessíveis somente neste arquivo.
*/



//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------
/**
* Limpa completamente a memória fornecida.
* @param MEMORIA	*memoria_param	A memória que será limpa.
*/
void privada_limpar(MEMORIA *memoria_param){
	int palavra=0;
	for(; palavra<QUANTIDADE_PALAVRAS_MEMORIA; palavra++){
		memoria_param->palavras[palavra] = MEMORIA_POSICAO_VAZIA;
	}
}

/**
* Converte bytes em uma palavra.
* @param MEMORIA	*memoria_param	A memória que será limpa.
* @param BYTE		byte0_param			O byte que ficará na posição 0 da palavra, que conterá o bit mais significativo da palavra.
* @param BYTE		byte1_param			O byte que ficará na posição 1 da palavra.
* @param BYTE		byte2_param			O byte que ficará na posição 2 da palavra.
* @param BYTE		byte3_param			O byte que ficará na posição 3 da palavra.
*/
PALAVRA privada_bytesParaPalavra(MEMORIA *memoria_param, BYTE byte0_param, BYTE byte1_param, BYTE byte2_param, BYTE byte3_param){
	PALAVRA palavraFinal;
	palavraFinal = 0;
	palavraFinal = palavraFinal | (byte0_param*256*256*256 & 0xFF000000);
	palavraFinal = palavraFinal | (byte1_param*256*256 & 0x00FF0000);
	palavraFinal = palavraFinal | (byte2_param*256 & 0x0000FF00);
	palavraFinal = palavraFinal | (byte3_param & 0x000000FF);
	return palavraFinal;
}

/**
* @param MEMORIA	*memoria_param		A memória em que a escrita será feita.
* @param int		endereco_param		Endereço da memória em que a escrita será feita.
* @param BYTE		byte0_param			O byte que ficará na posição 0 da palavra, que conterá o bit mais significativo da palavra.
* @param BYTE		byte1_param			O byte que ficará na posição 1 da palavra.
* @param BYTE		byte2_param			O byte que ficará na posição 2 da palavra.
* @param BYTE		byte3_param			O byte que ficará na posição 3 da palavra.
* ATENÇÃO: Escreve uma PALAVRA na memória, mas permite especifica-la byte-a-byte.
*/
void privada_escreverBytes(MEMORIA *memoria_param, int endereco_param, 
		BYTE byte0_param, BYTE byte1_param, BYTE byte2_param, BYTE byte3_param){
	int palavra = endereco_param;
	PALAVRA palavraReconstruida = privada_bytesParaPalavra(memoria_param, byte0_param, byte1_param, byte2_param, byte3_param);
	memoria_param->palavras[palavra] = palavraReconstruida;
}

//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/**
* @param MEMORIA	*memoria_param	A memória que será inicializada.
*/
void memoria_sincronizado_inicializar(MEMORIA *memoria_param){
	sem_init(&memoria_param->mutexAcessoMemoria, 0, 1);
	sem_wait(&memoria_param->mutexAcessoMemoria);
	privada_limpar(memoria_param);

	/*privada_escreverBytes(memoria_param, 0,  'X', 'M',  0,  10);
	privada_escreverBytes(memoria_param, 1,  'X', 'D',  0,  10);
	privada_escreverBytes(memoria_param, 2,  'S', 'M',  0,  10);
	privada_escreverBytes(memoria_param, 3,  'J', 'P', 'A', 0);
	privada_escreverBytes(memoria_param, 30, 'L', 'D',  0,  10);
	privada_escreverBytes(memoria_param, 31, 'L', 'D',  0,  10);
	privada_escreverBytes(memoria_param, 32, 'I', 'N', 'T', 36);
	privada_escreverBytes(memoria_param, 33, 'J', 'P', 'A', 30);*/
	privada_escreverBytes(memoria_param, 100, 'J', 'P', 'A', 200);
	sem_post(&memoria_param->mutexAcessoMemoria);
}

/**
* @param MEMORIA	*memoria_param			A memória em que a escrita será feita.
* @param int		endereco_param			Endereço da memória em que a escrita será feita.
* @param PALAVRA 	dadosEscritos_param		O que será efetivamente escrito em endereco_param.
*/
void memoria_sincronizado_escreverPalavra(MEMORIA *memoria_param, int endereco_param, PALAVRA dadosEscritos_param){
	sem_wait(&memoria_param->mutexAcessoMemoria);
	int palavra = endereco_param;
	if(palavra < QUANTIDADE_PALAVRAS_MEMORIA){
		memoria_param->palavras[palavra] = dadosEscritos_param;
	} else {
		tela_imprimirTelaAzulDaMorte(&global_tela, "Ocorreu um erro fatal! Um programa tentou escrever em um endereco fisico inexistente!");
	}
	sem_post(&memoria_param->mutexAcessoMemoria);
}

/**
* @param MEMORIA	*memoria_param		A memória em que a escrita será feita.
* @param int		endereco_param		Endereço da memória em que a escrita será feita.
* @param BYTE		byte0_param			O byte que ficará na posição 0 da palavra, que conterá o bit mais significativo da palavra.
* @param BYTE		byte1_param			O byte que ficará na posição 1 da palavra.
* @param BYTE		byte2_param			O byte que ficará na posição 2 da palavra.
* @param BYTE		byte3_param			O byte que ficará na posição 3 da palavra.
* ATENÇÃO: Escreve uma PALAVRA na memória, mas permite especifica-la byte-a-byte.
*/
void memoria_sincronizado_escreverBytes(MEMORIA *memoria_param, int endereco_param, 
		BYTE byte0_param, BYTE byte1_param, BYTE byte2_param, BYTE byte3_param){
	sem_wait(&memoria_param->mutexAcessoMemoria);
	if(endereco_param < QUANTIDADE_PALAVRAS_MEMORIA){
		privada_escreverBytes(memoria_param, endereco_param, byte0_param, byte1_param, byte2_param, byte3_param);
	} else {
		tela_imprimirTelaAzulDaMorte(&global_tela, "Ocorreu um erro fatal! Um programa tentou escrever em um endereco fisico da memoria inexistente!");
	}
	sem_post(&memoria_param->mutexAcessoMemoria);
}


/**
* @param MEMORIA	*memoria_param			A memória em que a leitura será feita.
* @param int		endereco_param			Endereço da memória em que a leitura será feita.
* @param PALAVRA	*destino_param			Endereço da variável em que o resultado será colocado.
* @return PALAVRA	Conteúdo da memória no endereço dado.
*/
void memoria_sincronizado_ler(MEMORIA *memoria_param, int endereco_param, PALAVRA *destino_param){
	sem_wait(&memoria_param->mutexAcessoMemoria);
	PALAVRA dadosLidos;
	int palavra = endereco_param;
	if(palavra < QUANTIDADE_PALAVRAS_MEMORIA){
		dadosLidos = memoria_param->palavras[palavra];
	} else {
		tela_imprimirTelaAzulDaMorte(&global_tela, "Ocorreu um erro fatal! Um programa tentou ler um endereco fisico da memoria inexistente!");
		dadosLidos = 0;
	}
	*destino_param = dadosLidos;
	sem_post(&memoria_param->mutexAcessoMemoria);
}

/**
* Imprimir toda memória, para fins de debug.
* @param MEMORIA	*memoria_param			A memória em que a leitura será feita.
*/
void memoria_imprimir(MEMORIA *memoria_param){
	int palavra = 0;
	char mensagem[200];
	tela_escreverNaColuna(&global_tela, "Imprimindo memoria.", 5);
	for(; palavra<QUANTIDADE_PALAVRAS_MEMORIA; palavra++){
		if(memoria_param->palavras[palavra] != MEMORIA_POSICAO_VAZIA){
			sprintf(mensagem, "*(%d)=%d=", palavra, memoria_param->palavras[palavra]);
			tela_escreverNaColuna(&global_tela, mensagem, 5);
			sprintf(mensagem, "%d %d %d %d", 
				(((memoria_param->palavras[palavra] & 0xFF000000)/256)/256)/256,
				((memoria_param->palavras[palavra] & 0x00FF0000)/256)/256,
				(memoria_param->palavras[palavra] & 0x0000FF00)/256,
				memoria_param->palavras[palavra] & 0x000000FF);
			tela_escreverNaColuna(&global_tela, mensagem, 5);
		}
	}
}




