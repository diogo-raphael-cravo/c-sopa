/**
* Concentra as funções de manipulação de descritores de arquivos.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define MAXIMO_SEGMENTOS_DESCRITOR_ARQUIVO 20

struct str_descritorArquivo{
	FIFO segmentos; //Os segmentos deste arquivo. A ordem da fila é a ordem para reconstrução do arquivo.
	int palavraAtual; //Palavra sendo lida/escrita pelo usuário.
	char* nome; //Nome que será associado ao descritor, visível ao usuário.
};

typedef struct str_descritorArquivo DESCRITOR_ARQUIVO;


//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* @param DESCRITOR_ARQUIVO	*descritor_param	O descritor de arquivo que será inicializado.
* @param char*				nome_param			Nome que será associado ao descritor, visível ao usuário.
*/
void descritorArquivo_inicializar(DESCRITOR_ARQUIVO *descritor_param, char* nome_param);

/**
* @param DESCRITOR_ARQUIVO	*descritor_param	O descritor de arquivo ao qual o segmento será adicionado.
* @param ARQUIVO			*segmento_param		O segmento que será adicionado ao descritor.
*/
void descritorArquivo_adicionarSegmento(DESCRITOR_ARQUIVO *descritor_param, ARQUIVO *segmento_param);

/**
* @param DESCRITOR_ARQUIVO	*descritor_param		O descritor de arquivo que será desfragmentado.
* @param int				posicaoInicial_param	A posição do disco à partir da qual o arquivo será guardado.
* ATENÇÃO: colocará todos os segmentos do arquivo contiguamente! Certifique-se de que há espaço antes de chamar!
*/
void descritorArquivo_desfragmentar(DESCRITOR_ARQUIVO *descritor_param, int posicaoInicial_param);

/**
* @param DESCRITOR_ARQUIVO	*descritor_param	O arquivo lógico cuja informação será retornada.
* @return int	Quantidade total de palavras ocupadas pelo descritor.
*/
int descritorArquivo_tamanhoEmPalavras(DESCRITOR_ARQUIVO *descritor_param);

/**
* @param DESCRITOR_ARQUIVO	*descritor_param	Arquivo que será atualizado na máquina hospedeira.
* @param char*				caminho_param		Caminho do arquivo na máquina hospedeira.
*/
void descritorArquivo_atualizarNaMaquinaHospedeira(DESCRITOR_ARQUIVO *descritor_param, char* caminho_param);










