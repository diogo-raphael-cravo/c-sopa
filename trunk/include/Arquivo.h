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
	LER=0,
	ESCREVER=1
};

typedef enum enum_opcoesAberturaArquivo OPCAO_ABERTURA_ARQUIVO;

struct str_arquivo{
		//No disco
	int enderecoInicioDisco; //O bloco de início deste arquivo no disco.
	int tamanhoEmPalavras; //Tamanho deste arquivo em palavras.
	DISCO *discoSalvo; //O disco em que o arquivo está salvo.

		//No kernel
	char* nome; //Nome amigável ao usuário, atribuído ao arquivo.
	DESCRITOR_PROCESSO *processoQueAbriu; //Ponteiro para o processo que abriu este arquivo.
	int numeroDescritor; //Número usado por processos do SOPA para ler e escrever.
};

typedef struct str_arquivo ARQUIVO;

//---------------------------------------------------------------------
//			FUNÇÕES
//---------------------------------------------------------------------
/**
* @param ARQUIVO	*arquivo_param			O arquivo que será inicializado.
* @param char*		nome_param				Nome do arquivo, amigável ao usuário.
* @param int		enderecoInicio_param	Endereço do disco que iniciará o arquivo.
* @param int		numeroDescritor_param	Número usado por processos do SOPA para ler e escrever.
*/
void arquivo_inicializar(ARQUIVO *arquivo_param, char* nome_param, int enderecoInicio_param, int numeroDescritor_param);

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
* @param char*		caminho_param				Caminho do arquivo na máquina hospedeira.
*/
void arquivo_atualizarNaMaquinaHospedeira(ARQUIVO *arquivo_param, char* caminho_param);

/**
* @param ARQUIVO	*arquivo_param	O arquivo cuja informação será retornada.
* @return int	Nome amigável ao usuário, atribuído ao arquivo.
*/
char* arquivo_getNome(ARQUIVO *arquivo_param);

/**
* @param ARQUIVO	*arquivo_param	O arquivo cuja informação será retornada.
* @return int	O tamanho do arquivo em palavras. Retornará -1 caso o arquivo não esteja salvo nem no disco, nem na memória.
*/
int arquivo_getTamanhoEmPalavras(ARQUIVO *arquivo_param);

/**
* @param ARQUIVO	*arquivo_param	O arquivo cuja informação será retornada.
* @return int	Indica se o arquivo pode ser aberto.
*/
int arquivo_podeSerAberto(ARQUIVO *arquivo_param);

/**
* @param ARQUIVO	*arquivo_param	O arquivo cuja informação será retornada.
* @return DESCRITOR_PROCESSO*	O processo que abriu este arquivo. Caso o arquivo não tenha sido aberto, retornará NULL.
*/
DESCRITOR_PROCESSO* arquivo_getProcessoQueAbriu(ARQUIVO *arquivo_param);

/**
* @param ARQUIVO				*arquivo_param		O arquivo cuja informação será retornada.
* @param DESCRITOR_PROCESSO		*processo_param		O processo que abrirá o arquivo.
* ATENÇÃO: não checa se o arquivo já está aberto!
*/
void arquivo_abrirParaProcesso(ARQUIVO *arquivo_param, DESCRITOR_PROCESSO *processo_param);


