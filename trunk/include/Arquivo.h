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


struct str_arquivo{
		//No disco
	int enderecoInicioDisco; //O bloco de início deste arquivo no disco.
	int tamanhoEmPalavras; //Tamanho deste arquivo em palavras.
	DISCO *discoSalvo; //O disco em que o arquivo está salvo.

		//No kernel
	char* nome; //Nome amigável ao usuário, atribuído ao arquivo.
	DESCRITOR_PROCESSO *processoQueAbriu; //Ponteiro para o processo que abriu este arquivo.
};

typedef struct str_arquivo ARQUIVO;

//---------------------------------------------------------------------
//			FUNÇÕES
//---------------------------------------------------------------------
/**
* Cria um arquivo à partir de um arquivo existente e salvo na máquina hospedeira.
* O novo arquivo é salvo no disco.
* @param ARQUIVO	*arquivo_param				Arquivo que será criado.
* @param DISCO		*disco_param				O disco em que o arquivo será salvo.
* @param char*		nome_param					O nome que será dado ao arquivo no SOPA.
* @param char*		caminho_param				Caminho do arquivo na máquina hospedeira.
* @param int		enderecoInicio_param_param	Endereço do disco que iniciará o arquivo.
* @return int	Indica se foi possível ler o arquivo.
*/
int arquivo_lerDaMaquinaHospedeira(ARQUIVO *arquivo_param, DISCO *disco_param, char* nome_param, char* caminho_param, int enderecoInicio_param_param);

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




