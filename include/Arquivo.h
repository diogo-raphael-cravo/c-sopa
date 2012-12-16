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
	int blocoInicioDisco; //O bloco de início deste arquivo no disco.
	int tamanhoEmBlocos; //Tamanho deste arquivo em blocos do disco.
	DISCO *discoSalvo; //O disco em que o arquivo está salvo.

		//Na memória
	MMU *memoriaSalvo; //A memória em que o arquivo está salvo.
	int enderecoInicioMemoria; //Endereço em que o arquivo foi salvo na memória.
	int tamanhoEmPalavras; //Tamanho do arquivo em palavras da memória.

		//No kernel
	char* nome; //Nome amigável ao usuário, atribuído ao arquivo.
};

typedef struct str_arquivo ARQUIVO;

//---------------------------------------------------------------------
//			FUNÇÕES
//---------------------------------------------------------------------
/**
* Cria um novo arquivo na memória.
* @param ARQUIVO	*arquivo_param				Arquivo que será criado.
* @param MMU		*MMU_param					A memória em que o arquivo será salvo.
* @param int		enderecoFisicoInicio_param	Endereço físico da memória que iniciará o arquivo.
* @param int		tamanhoEmPalavras_param		Tamanho do arquivo em palavras da memória.
* @param char*		nome_param					O nome que o arquivo terá.
*/
void arquivo_criar(ARQUIVO *arquivo_param, MMU *MMU_param, int enderecoFisicoInicio_param, int tamanhoEmPalavras_param, char* nome_param);

/**
* Cria um arquivo à partir de um arquivo existente e salvo na máquina hospedeira.
* O novo arquivo é salvo no disco.
* @param ARQUIVO	*arquivo_param				Arquivo que será criado.
* @param DISCO		*disco_param				O disco em que o arquivo será salvo.
* @param char*		nome_param					O nome que será dado ao arquivo no SOPA.
* @param char*		caminho_param				Caminho do arquivo na máquina hospedeira.
* @param int		blocoInicio_param			Bloco do disco que iniciará o arquivo.
* @return int	Indica se foi possível ler o arquivo.
*/
int arquivo_lerDaMaquinaHospedeira(ARQUIVO *arquivo_param, DISCO *disco_param, char* nome_param, char* caminho_param, int blocoInicio_param);

/**
* @param ARQUIVO	*arquivo_param				Arquivo sobre o qual a operação será realizada.
* @param MMU		*MMU_param					Memória na qual o arquivo será salvo.
* @param int		enderecoFisicoInicio_param	Endereço físico à partir do qual o arquivo será salvo.
* @return int	Indica se foi possível.
* ATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃO
* ATENÇÃO: o arquivo deve estar no buffer de leitura do disco!!!!!!!
* ATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃOATENÇÃO
*/
int arquivo_transferirParaMemoria(ARQUIVO *arquivo_param, MMU *MMU_param, int enderecoFisicoInicio_param);

/**
* @param ARQUIVO	*arquivo_param				Arquivo sobre o qual a operação será realizada.
* @return int	Indica se foi possível.
* ATENÇÃO: o arquivo deve existir no disco e na memória!
*/
int arquivo_atualizarNoDisco(ARQUIVO *arquivo_param);

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
* @return int	O tamanho do arquivo em blocos. Retornará -1 caso o arquivo não esteja salvo nem no disco, nem na memória.
*/
int arquivo_getTamanhoEmBlocos(ARQUIVO *arquivo_param);



