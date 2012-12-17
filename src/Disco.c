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
	disco_inicializarPosicao(disco_param, primeiraPosicaoLivre, byte0_param, byte1_param, byte2_param, byte3_param);
}

/**
* Efetivamente executa a operação dada.
* @param DISCO				*disco_param		O disco em que a operação será realizada.
* @param OPERACAO_DISCO 	operacao_param		A operação que será realizada.s
* @param int				endereco_param		Para operações sobre um endereço, o próprio.
* @param PALAVRA			*dados_param		Para uma escrita, os dados que serão escritos.
* @param int				bloco_param			Para uma carga, o bloco que será carregado. Para escrita, o bloco que a inicia.
* @param int				tamanhoDados_param	Quantidade de palavras nos dados que serão escritos.
*/
void privada_executar(DISCO *disco_param, OPERACAO_DISCO operacao_param, int endereco_param, PALAVRA *dados_param, int bloco_param, int tamanhoDados_param){
	char mensagem[200];
	if(	(operacao_param == OPERACAO_LEITURA_DISCO 
				&& (endereco_param < 0 || TAMANHO_DISCO_PALAVRAS <= endereco_param))
		|| (operacao_param == OPERACAO_ESCRITA_DISCO 
				&& (bloco_param < 0 || TAMANHO_DISCO_PALAVRAS <= bloco_param*TAMANHO_BLOCO+tamanhoDados_param))
		|| (operacao_param == OPERACAO_CARGA_DISCO 
				&& (bloco_param < 0 || QUANTIDADE_BLOCOS_DISCO <= bloco_param)) ){
		disco_param->erroUltimaOperacao = ERRO_DISCO_ENDERECO_INVALIDO;
	} else {
		disco_param->erroUltimaOperacao = SEM_ERRO_DISCO;
		switch(operacao_param){
			case OPERACAO_LEITURA_DISCO:
				tela_escreverNaColuna(&global_tela, "Leitura.",4);
				disco_param->tamanhoUltimaLeitura = 1;
				if(disco_param->dadosUltimaLeitura != NULL){
					free(disco_param->dadosUltimaLeitura);
				}
				disco_param->dadosUltimaLeitura = (PALAVRA*) malloc(disco_param->tamanhoUltimaLeitura*sizeof(PALAVRA));
				disco_param->dadosUltimaLeitura[0] = disco_param->conteudo[endereco_param];
				break;
			case OPERACAO_ESCRITA_DISCO:
				tela_escreverNaColuna(&global_tela, "Escrita.",4);
				int palavraEscrita = disco_param->tamanhoPalavrasProximaEscrita;
				for(; palavraEscrita<tamanhoDados_param; palavraEscrita++){
					disco_param->conteudo[bloco_param*TAMANHO_BLOCO+palavraEscrita] = dados_param[palavraEscrita];
				}
				free(dados_param);
				break;
			case OPERACAO_CARGA_DISCO:
				sprintf(mensagem, "Carregar bloco %d", bloco_param);
				tela_escreverNaColuna(&global_tela, mensagem, 4);
				disco_param->tamanhoUltimaLeitura = TAMANHO_BLOCO;
				if(disco_param->dadosUltimaLeitura != NULL){
					free(disco_param->dadosUltimaLeitura);
				}
				disco_param->dadosUltimaLeitura = (PALAVRA*) malloc(disco_param->tamanhoUltimaLeitura*sizeof(PALAVRA));
				int palavraLida;
				for(palavraLida=0; palavraLida<tamanhoDados_param; palavraLida++){
					disco_param->dadosUltimaLeitura[palavraLida] = disco_param->conteudo[bloco_param*TAMANHO_BLOCO+palavraLida];
				}
disco_imprimir(disco_param);
				break;
			case OPERACAO_NENHUMA_DISCO:
				tela_escreverNaColuna(&global_tela, "ERRO: A operacao especificada nao existe.",4);
		}
	}
}

//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/**
* @param DISCO	*disco_param	Inicializa o disco.
*/
void disco_inicializar(DISCO *disco_param){
	sem_init(&disco_param->mutexAcessoDisco, 0, 1);
	sem_wait(&disco_param->mutexAcessoDisco);
	int posicaoDisco=0;
	for(; posicaoDisco<TAMANHO_DISCO_PALAVRAS; posicaoDisco++){
		disco_param->conteudo[posicaoDisco] = POSICAO_VAZIA;
	}
	disco_param->proximaOperacao = OPERACAO_NENHUMA_DISCO;
	disco_param->erroUltimaOperacao = SEM_ERRO_DISCO;
	disco_param->tamanhoUltimaLeitura = 0;
	disco_param->dadosUltimaLeitura = NULL;
	disco_param->dadosProximaEscrita = DADO_NENHUM;
	disco_param->tamanhoPalavrasProximaEscrita = 0;
	disco_param->enderecoProximaOperacao = 0;
}

/**
* Inicia thread do disco.
* @param DISCO	*disco_param	O disco que irá 'rodar'.
*/
void disco_rodar(DISCO *disco_param){
	while(1){
		sem_wait(&disco_param->mutexAcessoDisco);

		usleep(1000*1000*2);

		privada_executar(disco_param, disco_param->proximaOperacao, 
			disco_param->enderecoProximaOperacao, 
			disco_param->dadosProximaEscrita, 
			disco_param->enderecoProximaOperacao,
			disco_param->tamanhoPalavrasProximaEscrita);

		controladorInterrupcoes_set(&global_controladorInterrupcoes, INTERRUPCAO_DISCO);
	}
}

/**
* Libera o disco para executar uma operação.
* @param DISCO				*disco_param			O disco em que a operação será realizada.
* @param OPERACAO_DISCO 	operacao_param			A operação que será realizada.s
* @param int				enderecoOuBloco_param	Para operações sobre um endereço, o próprio. Para carga, o bloco que será carregado.
* @param PALAVRA			*dados_param			Para uma escrita, os dados que serão escritos.
* @param int				tamanhoDados_param		Para uma escrita, a quantidade de palavras que serão escritas.
* ATENÇÃO: A operação não é executada de imediato, mas algum tempo depois.
*/
void disco_executarOperacao(DISCO *disco_param, OPERACAO_DISCO operacao_param, int enderecoOuBloco_param, PALAVRA *dados_param, int tamanhoDados_param){
	disco_param->tamanhoUltimaLeitura = 0;
	disco_param->erroUltimaOperacao = SEM_ERRO_DISCO;
	disco_param->enderecoProximaOperacao = enderecoOuBloco_param;
	disco_param->proximaOperacao = operacao_param;
	if(operacao_param != OPERACAO_LEITURA_DISCO
			&& operacao_param != OPERACAO_CARGA_DISCO
			&& operacao_param != OPERACAO_ESCRITA_DISCO){
		disco_param->enderecoProximaOperacao = 0;
		disco_param->dadosProximaEscrita = DADO_NENHUM;
		disco_param->proximaOperacao = OPERACAO_NENHUMA_DISCO;
	} else if(operacao_param == OPERACAO_ESCRITA_DISCO){
		disco_param->dadosProximaEscrita = dados_param;
		disco_param->tamanhoPalavrasProximaEscrita = tamanhoDados_param;
	} else if(operacao_param == OPERACAO_CARGA_DISCO){
		disco_param->tamanhoPalavrasProximaEscrita = tamanhoDados_param;
	}
	sem_post(&disco_param->mutexAcessoDisco);
}

/**
* @param DISCO	*disco_param			O disco em que a operação será realizada.
* @return ERRO_DISCO 	O erro que aconteceu, caso tenha acontecido, na última operação realizada no disco.
*/
ERRO_DISCO disco_erroUltimaOperacao(DISCO *disco_param){
	return disco_param->erroUltimaOperacao;
}

/**
* @param DISCO	*disco_param			O disco em que a operação será realizada.
* @return int 	Tamanho em palavras do que foi lido na última operação de leitura.
*/
int disco_tamanhoPalavrasUltimaLeitura(DISCO *disco_param){
	return disco_param->tamanhoUltimaLeitura;
}

/**
* @param DISCO	*disco_param			O disco em que a operação será realizada.
* @param int	posicaoBuffer_param		A posição que será retornada do buffer da última leitura.
* @return PALAVRA 	A palavra lida na última leitura do disco e que está na posicaoBuffer_param.
*/
PALAVRA disco_palavrasUltimaLeituraPosicao(DISCO *disco_param, int posicaoBuffer_param){
	if(posicaoBuffer_param < disco_param->tamanhoUltimaLeitura){
		return disco_param->dadosUltimaLeitura[posicaoBuffer_param];
	} else {
		tela_imprimirTelaAzulDaMorte(&global_tela, "Ocorreu um erro fatal! Um programa tentou ler um endereco inexistente do buffer de carga do disco!");
		return 0;
	}
}

/*
* Escreve imediatamente os bytes na posição dada livre do disco.
* Corre o risco de não escrever os dados em posições contíguas quando chamada sucessivamente.
* @param DISCO		*disco_param		O disco em que será gravado.
* @param int		posicao_param		A posição do disco em que será gravado.
* @param BYTE		byte0_param			O byte que ficará na posição 0 da palavra, que conterá o bit mais significativo da palavra.
* @param BYTE		byte1_param			O byte que ficará na posição 1 da palavra.
* @param BYTE		byte2_param			O byte que ficará na posição 2 da palavra.
* @param BYTE		byte3_param			O byte que ficará na posição 3 da palavra.
* ATENÇÃO: esta função serve somente para a inicialização do disco, de forma que dê a impressão de ser persistente.
*/
void disco_inicializarPosicao(DISCO *disco_param, int posicao_param, BYTE byte0_param, BYTE byte1_param, BYTE byte2_param, BYTE byte3_param){
	disco_param->conteudo[posicao_param] = 
		(byte0_param*256*256*256 & 0xFF000000)
		| (byte1_param*256*256 & 0x00FF0000)
		| (byte2_param*256 & 0x0000FF00)
		| (byte3_param & 0x000000FF);
}

/**
* Imprimir todo disco, para fins de debug.
* @param DISCO	*disco_param			O disco em que a leitura será feita.
*/
void disco_imprimir(DISCO *disco_param){
	int palavra = 0;
	char mensagem[200];
	tela_escreverNaColuna(&global_tela, "Imprimindo disco.", 5);
	for(; palavra<TAMANHO_DISCO_PALAVRAS; palavra++){
		if(disco_param->conteudo[palavra] != POSICAO_VAZIA){
			sprintf(mensagem, "*(%d)=%d=", palavra, disco_param->conteudo[palavra]);
			tela_escreverNaColuna(&global_tela, mensagem, 5);
			sprintf(mensagem, "%d %d %d %d", 
				(((disco_param->conteudo[palavra] & 0xFF000000)/256)/256)/256,
				((disco_param->conteudo[palavra] & 0x00FF0000)/256)/256,
				(disco_param->conteudo[palavra] & 0x0000FF00)/256,
				disco_param->conteudo[palavra] & 0x000000FF);
			tela_escreverNaColuna(&global_tela, mensagem, 5);
		}
	}
}

