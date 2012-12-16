/**
* Concentra as funções de manipulação do disco.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define TAMANHO_DISCO_PALAVRAS 1024
#define QUANTIDADE_BLOCOS_DISCO 32
#define TAMANHO_BLOCO TAMANHO_DISCO_PALAVRAS/QUANTIDADE_BLOCOS_DISCO
#define POSICAO_VAZIA -99999
#define DADO_NENHUM NULL

enum enum_operacaoDisco{
	OPERACAO_LEITURA_DISCO,
	OPERACAO_ESCRITA_DISCO,
	OPERACAO_CARGA_DISCO,
	OPERACAO_NENHUMA_DISCO
};

typedef enum enum_operacaoDisco OPERACAO_DISCO;

enum enum_erroDisco{
	SEM_ERRO_DISCO,
	ERRO_DISCO_ENDERECO_INVALIDO
};

typedef enum enum_erroDisco ERRO_DISCO;

struct str_disco{
	sem_t mutexAcessoDisco;
	PALAVRA conteudo[TAMANHO_DISCO_PALAVRAS];
	ERRO_DISCO erroUltimaOperacao; //Erro que aconteceu na última operação, caso algum tenha acontecido.
	int tamanhoUltimaLeitura; //A quantidade de palavras lidas na última leitura.
	PALAVRA* dadosUltimaLeitura; //Os dados lidos na última leitura.
	OPERACAO_DISCO proximaOperacao; //Próxima operação que deve ser executada pelo disco.
	PALAVRA* dadosProximaEscrita; //Dados que serão escritos na próxima escrita.
	int tamanhoPalavrasProximaEscrita; //Para uma escrita, a quantidade de palavras que serão escritas.
	int enderecoProximaOperacao; //Endereço em que a próxima operação será realizada.
};

typedef struct str_disco DISCO;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* @param DISCO	*disco_param	Inicializa o disco.
*/
void disco_inicializar(DISCO *disco_param);

/**
* Inicia thread do disco.
* @param DISCO	*disco_param	O disco que irá 'rodar'.
*/
void disco_rodar(DISCO *disco_param);

/**
* Libera o disco para executar uma operação.
* @param DISCO				*disco_param			O disco em que a operação será realizada.
* @param OPERACAO_DISCO 	operacao_param			A operação que será realizada.s
* @param int				enderecoOuBloco_param	Para operações sobre um endereço, o próprio. Para carga, o bloco que será carregado.
* @param PALAVRA			*dados_param			Para uma escrita, os dados que serão escritos.
* @param int				tamanhoDados_param		Para uma escrita, a quantidade de palavras que serão escritas.
* ATENÇÃO: A operação não é executada de imediato, mas algum tempo depois.
*/
void disco_executarOperacao(DISCO *disco_param, OPERACAO_DISCO operacao_param, int enderecoOuBloco_param, PALAVRA *dados_param, int tamanhoDados_param);

/**
* @param DISCO	*disco_param			O disco em que a operação será realizada.
* @return ERRO_DISCO 	O erro que aconteceu, caso tenha acontecido, na última operação realizada no disco.
*/
ERRO_DISCO disco_erroUltimaOperacao(DISCO *disco_param);

/**
* @param DISCO	*disco_param			O disco em que a operação será realizada.
* @return int 	Tamanho em palavras do que foi lido na última operação de leitura.
*/
int disco_tamanhoPalavrasUltimaLeitura(DISCO *disco_param);

/**
* @param DISCO	*disco_param			O disco em que a operação será realizada.
* @param int	posicaoBuffer_param		A posição que será retornada do buffer da última leitura.
* @return PALAVRA 	A palavra lida na última leitura do disco e que está na posicaoBuffer_param.
*/
PALAVRA disco_palavrasUltimaLeituraPosicao(DISCO *disco_param, int posicaoBuffer_param);

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
void disco_inicializarPosicao(DISCO *disco_param, int posicao_param, BYTE byte0_param, BYTE byte1_param, BYTE byte2_param, BYTE byte3_param);














