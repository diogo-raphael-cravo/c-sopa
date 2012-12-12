/**
* Concentra as funções de manipulação do disco.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define CAMINHO_ARQUIVO_BASE_DISCO "dados/disco.txt"
#define TAMANHO_DISCO_PALAVRAS 1024
#define POSICAO_VAZIA -99999

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
	PALAVRA dadosProximaEscrita; //Dados que serão escritos na próxima escrita.
	int enderecoProximaOperacao; //Endereço em que a próxima operação será realizada.
};

typedef struct str_disco DISCO;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* Inicia thread do disco.
* @param DISCO	*disco_param	O disco que irá 'rodar'.
*/
void disco_rodar(DISCO *disco_param);

/**
* Libera o disco para executar uma operação.
* @param DISCO				*disco_param	O disco em que a operação será realizada.
* @param OPERACAO_DISCO 	operacao_param	A operação que será realizada.s
* @param int				endereco_param	Para operações sobre um endereço, o próprio.
* @param int				dados_param		Para uma escrita, os dados que serão escritos.
* ATENÇÃO: A operação não é executada de imediato, mas algum tempo depois.
*/
void disco_executarOperacao(DISCO *disco_param, OPERACAO_DISCO operacao_param, int endereco_param, PALAVRA dados_param);

/**
* Carrega os dados do arquivo, juntando-os aos dados que estão no disco.
* Os dados são colocados à partir da primeira posição livre neste disco.
* @param DISCO	*disco_param			O disco em que a operação será realizada.
* @param char*	*caminhoArquivo_param	Arquivo que contém os dados, formatados da seguinte forma:
*										J P A 0
*										255 255 255 255
*										0 0 0 0
*/
void disco_carregar(DISCO *disco_param, char *caminhoArquivo_param);

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















