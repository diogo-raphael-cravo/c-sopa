/**
* Concentra as funções de manipulação de memória.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define QUANTIDADE_PALAVRAS_MEMORIA 1024
#define MEMORIA_ENDERECO_INEXISTENTE -1

typedef int PALAVRA;
typedef int BYTE;

struct str_memoria{
	PALAVRA palavras[QUANTIDADE_PALAVRAS_MEMORIA];
	sem_t mutexAcessoMemoria; //Garante que somente um processo usará a memória por vez.
};

typedef struct str_memoria MEMORIA;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* @param MEMORIA	*memoria_param	A memória que será inicializada.
*/
void memoria_sincronizado_inicializar(MEMORIA *memoria_param);

/**
* @param MEMORIA	*memoria_param			A memória em que a escrita será feita.
* @param int		endereco_param			Endereço da memória em que a escrita será feita.
* @param PALAVRA 	dadosEscritos_param		O que será efetivamente escrito em endereco_param.
*/
void memoria_sincronizado_escreverPalavra(MEMORIA *memoria_param, int endereco_param, PALAVRA dadosEscritos_param);

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
	BYTE byte0_param, BYTE byte1_param, BYTE byte2param, BYTE byte3_param);

/**
* @param MEMORIA	*memoria_param			A memória em que a leitura será feita.
* @param int		endereco_param			Endereço da memória em que a leitura será feita.
* @param PALAVRA	*destino_param			Endereço da variável em que o resultado será colocado.
* @return PALAVRA	Conteúdo da memória no endereço dado.
*/
void memoria_sincronizado_ler(MEMORIA *memoria_param, int endereco_param, PALAVRA *destino_param);

/**
* Imprimir toda memória, para fins de debug.
* @param MEMORIA	*memoria_param			A memória em que a leitura será feita.
*/
void memoria_imprimir(MEMORIA *memoria_param);





