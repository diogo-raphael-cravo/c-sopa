/**
* Concentra as funções de manipulação do disco.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define TAMANHO_DISCO_PALAVRAS 1024
#define POSICAO_VAZIA -99999
#define DADO_NENHUM NULL

enum enum_operacaoDisco{
	TIPO_OPERACAO_LEITURA_DISCO,
	TIPO_OPERACAO_ESCRITA_DISCO,
	TIPO_OPERACAO_CARGA_DMA_DISCO,
	TIPO_OPERACAO_NENHUMA_DISCO
};

typedef enum enum_operacaoDisco TIPO_OPERACAO_DISCO;

enum enum_erroDisco{
	SEM_ERRO_DISCO,
	ERRO_DISCO_ENDERECO_INVALIDO
};

typedef enum enum_erroDisco ERRO_DISCO;

	//Disco
struct str_disco{
	sem_t mutexAcessoDisco;
	PALAVRA conteudo[TAMANHO_DISCO_PALAVRAS];
	int realizandoOperacao;

		//Última operação realizada
	ERRO_DISCO erroUltimaOperacao; //Erro que aconteceu na última operação, caso algum tenha acontecido.
	int tamanhoUltimaLeitura; //A quantidade de palavras lidas na última leitura.
	PALAVRA* dadosUltimaLeitura; //Os dados lidos na última leitura.

		//Próxima operação
	TIPO_OPERACAO_DISCO proximaOperacao; //Próxima operação que deve ser executada pelo disco.
	
	PALAVRA* dadosProximaEscrita; //Dados que serão escritos na próxima escrita.
	int tamanhoPalavrasProximaEscrita; //Para uma escrita, a quantidade de palavras que serão escritas.
	int enderecoProximaOperacao; //Endereço em que a próxima operação será realizada.
	int enderecoMemoriaProximaOperacao; //Endereço em que a próxima operação será realizada na memória.
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

/***********
* Estas operações liberam o disco para executar algo.
* ↓↓↓↓↓↓↓↓↓↓
************/

/**
* Agenda uma operação para ser executada pelo disco.
* @param DISCO				*disco_param			O disco em que a operação será realizada.
* @param int				endereco_param			O endereço lido.
*/
void disco_lerEndereco(DISCO *disco_param, int endereco_param);

/**
* Agenda uma operação para ser executada pelo disco.
* @param DISCO				*disco_param			O disco em que a operação será realizada.
* @param int				endereco_param			O endereço da escrita.
* @param PALAVRA			*dados_param			Os dados que serão escritos.
* @param int				tamanhoDados_param		A quantidade de palavras que serão escritas.
*/
void disco_escreverEndereco(DISCO *disco_param, int endereco_param, PALAVRA *dados_param, int tamanhoDados_param);

/**
* Agenda uma operação para ser executada pelo disco.
* @param DISCO				*disco_param				O disco em que a operação será realizada.
* @param int				enderecoMemoria_param		Endereço em que a escrita será iniciada na memória.
* @param int				enderecoDisco_param			O endereço do disco à partir do qual os dados serão transferidos.
* @param int				quantidadePalavras_param	A quantidade de palavras que serão lidas.
*/
void disco_transferirParaMemoria(DISCO *disco_param, int enderecoMemoria_param, int enderecoDisco_param, int quantidadePalavras_param);

/***********
* Estas operações liberam o disco para executar algo.
************/

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

/**
* Imprimir todo disco, para fins de debug.
* @param DISCO	*disco_param			O disco em que a leitura será feita.
*/
void disco_imprimir(DISCO *disco_param);

/**
* @param DISCO	*disco_param	O disco cuja informação será consultada.
* @return int	Indica se o disco está realizando uma operação agora.
*				Note que, caso ele esteja, não poderá realizar outra até que acabe a que está fazendo.
*/
int disco_estahOcupado(DISCO *disco_param);











