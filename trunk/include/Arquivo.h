/**
* Concentra as funções de manipulação do console.
* ====================================
*
* 			ATENÇÃO: Não confundir o console com a tela!
*
* ====================================
*/
//---------------------------------------------------------------------
//			DADOS
//---------------------------------------------------------------------

//Constantes


enum enum_opcoesAberturaArquivo{
	OPCAO_ABERTURA_ARQUIVO_LER=0,
	OPCAO_ABERTURA_ARQUIVO_ESCREVER=1
};

typedef enum enum_opcoesAberturaArquivo OPCAO_ABERTURA_ARQUIVO;

struct str_arquivo{
		//No disco
	int enderecoInicioDisco; //O bloco de início deste arquivo no disco.
	int tamanhoEmPalavras; //Tamanho deste arquivo em palavras.
	DISCO *discoSalvo; //O disco em que o arquivo está salvo.
};

typedef struct str_arquivo ARQUIVO;

//---------------------------------------------------------------------
//			FUNÇÕES
//---------------------------------------------------------------------
/**
* @param ARQUIVO	*arquivo_param			O arquivo que será inicializado.
* @param int		enderecoInicio_param	Endereço do disco que iniciará o arquivo.
*/
void arquivo_inicializar(ARQUIVO *arquivo_param, int enderecoInicio_param);

/**
* Cria um arquivo à partir de um arquivo existente e salvo na máquina hospedeira.
* O novo arquivo é salvo no disco.
* @param ARQUIVO	*arquivo_param				Arquivo que será criado.
* @param DISCO		*disco_param				O disco em que o arquivo será salvo.
* @param char*		caminho_param				Caminho do arquivo na máquina hospedeira.
* @return int	Indica se foi possível ler o arquivo.
*/
int arquivo_lerDaMaquinaHospedeira(ARQUIVO *arquivo_param, DISCO *disco_param, char* caminho_param);

/**
* @param ARQUIVO	*arquivo_param	Arquivo que será atualizado na máquina hospedeira.
* @param char*		caminho_param	Caminho do arquivo na máquina hospedeira.
*/
void arquivo_atualizarNaMaquinaHospedeira(ARQUIVO *arquivo_param, char* caminho_param);

/**
* @param ARQUIVO	*arquivo_param	Arquivo cuja imagem será retornada em forma de string.
* @return char*	O conteúdo que deve ter o arquivo da máquina hospedeira que representará este.
* ATENÇÃO: para uso somente em atualizações na máquina hospedeira!
*/
char* arquivo_getConteudo(ARQUIVO *arquivo_param);

/**
* @param ARQUIVO	*arquivo_param	O arquivo cuja informação será retornada.
* @return int	O tamanho do arquivo em palavras. Retornará -1 caso o arquivo não esteja salvo nem no disco, nem na memória.
*/
int arquivo_getTamanhoEmPalavras(ARQUIVO *arquivo_param);

/**
* @param ARQUIVO		*arquivo_param	O arquivo que será movido.
* @param int			posicao_param	Posição do disco para onde o arquivo vai.
*/
void arquivo_relocalizar(ARQUIVO *arquivo_param, int posicao_param);

/**
* @param ARQUIVO	*arquivo_param	O arquivo cuja informação será retornada.
* @return int	O endereço de início do arquivo.
*/
int arquivo_getEnderecoInicial(ARQUIVO *arquivo_param);



