/**
* Concentra as funções de manipulação do processador.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define TAMANHO_REGISTRADOR_BYTES 4
#define BYTE_QUALQUER '*'

struct str_registrador{
	BYTE conteudo[TAMANHO_REGISTRADOR_BYTES];
};

typedef struct str_registrador REGISTRADOR;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* Inicializa o registrador.
* @param REGISTRADOR	*registrador_param	O registrador em que será feita a operação.
*/
void registrador_inicializar(REGISTRADOR *registrador_param);

/**
* Carrega no registrador os bytes.
* @param REGISTRADOR	*registrador_param	O registrador em que será feita a escrita.
* @param BYTE			byte0_param			O byte que ficará na posição 0 da palavra, que conterá o bit mais significativo da palavra.
* @param BYTE			byte1_param			O byte que ficará na posição 1 da palavra.
* @param BYTE			byte2_param			O byte que ficará na posição 2 da palavra.
* @param BYTE			byte3_param			O byte que ficará na posição 3 da palavra.
*/
void registrador_carregarBytes(REGISTRADOR *registrador_param, BYTE byte0_param, BYTE byte1_param, BYTE byte2_param, BYTE byte3_param);

/**
* Carrega no registrador os bytes.
* @param REGISTRADOR	*registrador_param	O registrador em que será feita a escrita.
* @param PALAVRA		palavra_param		A palavra que será carregada no registrador.
*/
void registrador_carregarPalavra(REGISTRADOR *registrador_param, PALAVRA palavra_param);

/**
* @param REGISTRADOR						*registrador_param	O registrador que será lido.
* @return BYTE*	O conteúdo do registrador em bytes. O índice 0 contém os bits mais significativos.
*/
BYTE* registrador_lerBytes(REGISTRADOR *registrador_param);

/**
* @param REGISTRADOR	*registrador_param	O registrador que será lido.
* @return PALAVRA	A palavra que estava guardada no registrador.
*/
PALAVRA registrador_lerPalavra(REGISTRADOR *registrador_param);

/**
* Compara o conteúdo do registrador com os valores passados.
* @param REGISTRADOR	*registrador_param	Registrador cujo conteúdo será testado.
* @param BYTE			byte0_param			O byte que ficará na posição 0 da palavra, que conterá o bit mais significativo da palavra.
* @param BYTE			byte1_param			O byte que ficará na posição 1 da palavra.
* @param BYTE			byte2_param			O byte que ficará na posição 2 da palavra.
* @param BYTE			byte3_param			O byte que ficará na posição 3 da palavra.
* @return int	Indica se o registrador contém os bytes passados.
* ATENÇÃO: BYTE_QUALQUER é interpretado como wildcard, isto é, significa que o byte será ignorado para o resultado final.
*		exemplo: registrador_contem('*', 'A', 'Y', 'P'); Considerará somente os bytes 1, 2 e 3.
*/
int registrador_contem(REGISTRADOR *registrador_param, BYTE byte0_param, BYTE byte1_param, BYTE byte2_param, BYTE byte3_param);

/**
* Copia conteúdo de outro registrador.
* @param REGISTRADOR	*registradorDestino_param	O destino da cópia.
* @param REGISTRADOR	*registradorOrigem_param	A origem da cópia.
*/
void registrador_copiar(REGISTRADOR *registradorDestino_param, REGISTRADOR *registradorOrigem_param);

/**
* @param REGISTRADOR	*registrador_param	Registrador cujo conteúdo será alterado.
* @param int			parcela_param	A quantidade que deve ser somada ao valor do registrador.
*/
void registrador_somar(REGISTRADOR *registrador_param, int parcela_param);



