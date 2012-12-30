/**
* Concentra as funções de manipulação de descritores de arquivos.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define MAXIMO_SEGMENTOS_DESCRITOR_ARQUIVO 20
#define DESCRITOR_ARQUIVO_POSICAO_INEXISTENTE -1
#define DESCRITOR_ARQUIVO_SEGMENTO_INEXISTENTE NULL

struct str_descritorArquivo{
	FIFO segmentos; //Os segmentos deste arquivo. A ordem da fila é a ordem para reconstrução do arquivo.
	int palavraAtual; //Palavra sendo lida/escrita pelo usuário.
	char* nome; //Nome que será associado ao descritor, visível ao usuário.
	int numeroDescritor; //Número usado por processos do SOPA para ler e escrever.
	DESCRITOR_PROCESSO *processoQueAbriu; //Ponteiro para o processo que abriu este arquivo.
	OPCAO_ABERTURA_ARQUIVO opcaoAbertura; //O que será feito com o arquivo.
};

typedef struct str_descritorArquivo DESCRITOR_ARQUIVO;


//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* @param DESCRITOR_ARQUIVO	*descritor_param	O descritor de arquivo que será inicializado.
* @param char*				nome_param			Nome que será associado ao descritor, visível ao usuário.
* @param int		numeroDescritor_param	Número usado por processos do SOPA para ler e escrever.
*/
void descritorArquivo_inicializar(DESCRITOR_ARQUIVO *descritor_param, char* nome_param, int numeroDescritor_param);

/**
* @param DESCRITOR_ARQUIVO	*descritor_param	O descritor de arquivo ao qual o segmento será adicionado.
* @param ARQUIVO			*segmento_param		O segmento que será adicionado ao descritor.
*/
void descritorArquivo_adicionarSegmento(DESCRITOR_ARQUIVO *descritor_param, ARQUIVO *segmento_param);

/**
* @param DESCRITOR_ARQUIVO	*descritor_param	O arquivo lógico cuja informação será retornada.
* @return int	O número de segmentos do arquivo.
*/
int descritorArquivo_getQuantidadeSegmentos(DESCRITOR_ARQUIVO *descritor_param);

/**
* @param DESCRITOR_ARQUIVO	*descritor_param	O arquivo lógico cuja informação será retornada.
* @return int	Número usado por processos do SOPA para ler e escrever.
*/
int descritorArquivo_getNumeroDescritor(DESCRITOR_ARQUIVO *descritor_param);

/**
* @param DESCRITOR_ARQUIVO	*descritor_param	O arquivo lógico cuja informação será retornada.
* @param int				segmento_param		A ordem do segmento que deve ser retornado.
* @return ARQUIVO*	O arquivo na ordem especificada deste arquivo lógico.
*/
ARQUIVO* descritorArquivo_getSegmento(DESCRITOR_ARQUIVO *descritor_param, int segmento_param);

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

/**
* @param DESCRITOR_ARQUIVO	*descritor_param	O arquivo lógico cuja informação será retornada.
* @return char* 	Nome que será associado ao descritor, visível ao usuário.
*/
char* descritorArquivo_getNome(DESCRITOR_ARQUIVO *descritor_param);

/**
* @param DESCRITOR_ARQUIVO	*descritor_param	O arquivo que será testado.
* @return int	Indica se o arquivo está fragmentado no disco.
*/
int descritorArquivo_estahFragmentado(DESCRITOR_ARQUIVO *descritor_param);

/**
* @param DESCRITOR_ARQUIVO	*arquivo_param	O arquivo cuja informação será retornada.
* @return DESCRITOR_PROCESSO*	O processo que abriu este arquivo. Caso o arquivo não tenha sido aberto, retornará NULL.
*/
DESCRITOR_PROCESSO* descritorArquivo_getProcessoQueAbriu(DESCRITOR_ARQUIVO *arquivo_param);

/**
* @param DESCRITOR_ARQUIVO			*arquivo_param		O arquivo cuja informação será retornada.
* @param DESCRITOR_PROCESSO			*processo_param		O processo que abrirá o arquivo.
* @param OPCAO_ABERTURA_ARQUIVO		opcao_param			O que será feito com o arquivo.
* ATENÇÃO: não checa se o arquivo já está aberto!
*/
void descritorArquivo_abrirParaProcesso(DESCRITOR_ARQUIVO *arquivo_param, DESCRITOR_PROCESSO *processo_param,
		OPCAO_ABERTURA_ARQUIVO opcao_param);

/**
* @param DESCRITOR_ARQUIVO		*arquivo_param		O arquivo que será fechado.
*/
void descritorArquivo_fechar(DESCRITOR_ARQUIVO *arquivo_param);

/**
* @param DESCRITOR_ARQUIVO		*arquivo_param		O arquivo que será consultado.
* @return int	Palavra sendo lida/escrita pelo usuário. Se ultrapassar o limite, retonará DESCRITOR_ARQUIVO_POSICAO_INEXISTENTE.
*/
int descritorArquivo_getPalavraAtual(DESCRITOR_ARQUIVO *arquivo_param);

/**
* @param DESCRITOR_ARQUIVO		*arquivo_param		O arquivo que será consultado.
* @param int					palavraAtual_param	Palavra sendo lida/escrita pelo usuário.
*/
void descritorArquivo_setPalavraAtual(DESCRITOR_ARQUIVO *arquivo_param, int palavraAtual_param);

/**
* @param DESCRITOR_ARQUIVO		*arquivo_param			O arquivo que será consultado.
* @param int					enderecoLogico_param	Ponteiro lógico para alguma palavra do arquivo.
* @param int	Endereço da palavra no disco. Caso não haja, retornará DISCO_ENDERECO_INEXISTENTE.
*/
int descritorArquivo_getEnderecoDiscoPosicao(DESCRITOR_ARQUIVO *arquivo_param, int enderecoLogico_param);

/**
* @param DESCRITOR_ARQUIVO			*arquivo_param			O arquivo que será consultado.
* @param OPCAO_ABERTURA_ARQUIVO		opcao_param				Opção que será testada.
* @return int	Indica se o arquivo está aberto com a opção dada.
*/
int descritorArquivo_abertoComOpcao(DESCRITOR_ARQUIVO *arquivo_param, OPCAO_ABERTURA_ARQUIVO opcao_param);








