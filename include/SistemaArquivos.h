/**
* Concentra as funções de manipulação do sistema de arquivos.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define MAXIMO_ARQUIVOS 500 //A quantidade máxima de arquivos neste sistema de arquivos.
#define DIRETORIO_DADOS_DISCO "dados" //Caminho para o diretório onde ficam os arquivos do disco no hospedeiro.
#define CAMINHO_ARQUIVO_DESCRITOR_SISTEMA_ARQUIVOS "dados/descritorSistemaArquivos"
#define NUMERO_DESCRITOR_ARQUIVO_INEXISTENTE -1
#define TAMANHO_ARQUIVO_RECEM_CRIADO 32

struct str_sistemaArquivos{
	FIFO arquivos; //Guarda todos arquivos neste sistema de arquivos.
	int numeroDescritorArquivoLivre; //Sempre conterá um numero de descritor de arquivo que não 
							// está sendo usado por arquivo algum (utilizado por processos do SOPA).
};

typedef struct str_sistemaArquivos SISTEMA_ARQUIVOS;


//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* Inicializa o sistema de arquivos e o disco com os arquivos do sistema hospedeiro.
* Cria a ilusão de persistência do disco.
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param	O sistema de arquivos que será inicializado.
* @param DISCO				*disco_param			O disco que será preenchido com os dados lidos.
*/
void sistemaArquivos_inicializarComArquivosDoHospedeiro(SISTEMA_ARQUIVOS *sistemaArquivos_param, DISCO *disco_param);

/**
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param	O sistema de arquivos que realizará a busca.
* @param DISCO				*disco_param			O disco em que a busca é feita.
* @return int	Posição do disco que está livre para receber um arquivo novo (com tamanho TAMANHO_ARQUIVO_RECEM_CRIADO). 
* 				Retornará MEMORIA_ENDERECO_INEXISTENTE, caso não haja posição livre.
*/
int sistemaArquivos_getPosicaoLivreDisco(SISTEMA_ARQUIVOS *sistemaArquivos_param, DISCO *disco_param);

/**
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param	O sistema de arquivos em que a operação será realizada.
* @param char*				nomeProcurado_param		O nome do arquivo que se quer.
* @return ARQUIVO*	O arquivo que tem o nome procurado. Caso não haja ou esteja fragmentado, retonará NULL.
*/
ARQUIVO* sistemaArquivos_buscaPorNome(SISTEMA_ARQUIVOS *sistemaArquivos_param, char* nomeProcurado_param);

/**
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param	O sistema de arquivos em que a operação será realizada.
* @param int				numeroDescritor_param	Número descritor do arquivo que se quer.
* @return DESCRITOR_ARQUIVO*	O arquivo que tem o nome procurado. Caso não haja, retonará NULL.
*/
DESCRITOR_ARQUIVO* sistemaArquivos_buscaPorNumeroDescritor(SISTEMA_ARQUIVOS *sistemaArquivos_param, int numeroDescritor_param);

/**
* @param SISTEMA_ARQUIVOS			*sistemaArquivos_param	O sistema de arquivos em que a operação será realizada.
* @param char*						nomeProcurado_param		O nome do arquivo que se quer.
* @param DESCRITOR_PROCESSO			*processo_param			O processo que terá posse do arquivo aberto.
* @param OPCAO_ABERTURA_ARQUIVO		opcao_param				O que será feito com o arquivo.
* @return int	Número descritor do arquivo (usado por processos do SOPA para operar sobre o arquivo).
*				Caso não tenha sido possível encontrar/abrir, retornará NUMERO_DESCRITOR_ARQUIVO_INEXISTENTE.
*/
int sistemaArquivos_abrirArquivoExistentePara(SISTEMA_ARQUIVOS *sistemaArquivos_param, char* nomeProcurado_param,
	DESCRITOR_PROCESSO *processo_param, OPCAO_ABERTURA_ARQUIVO opcao_param);

/**
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param	O sistema de arquivos em que a operação será realizada.
* @param int				numeroDescritor_param	Número descritor do arquivo que será fechado.
*/
void sistemaArquivos_fecharArquivo(SISTEMA_ARQUIVOS *sistemaArquivos_param, int numeroDescritor_param);

/**
* Atualiza os arquivos na máquina hospedeira.
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param	O sistema de arquivos que será atualizado.
*/
void sistemaArquivos_atualizarNaMaquinaHospedeira(SISTEMA_ARQUIVOS *sistemaArquivos_param);

/**
* Cria um arquivo novo.
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param	O sistema de arquivos no qual o arquivo será criado.
* @param char*				nome_param				Nome do arquivo, amigável ao usuário.
* @param DISCO				*disco_param			Disco em que o arquivo será salvo.
* @return int	Número descritor do arquivo (usado por processos do SOPA para operar sobre o arquivo).
*				Caso não tenha sido possível criar, retornará NUMERO_DESCRITOR_ARQUIVO_INEXISTENTE.
*/
int sistemaArquivos_criarArquivo(SISTEMA_ARQUIVOS *sistemaArquivos_param, char* nome_param, DISCO *disco_param);

/**
* Fecha os arquivos que ainda estão abertos para o processo passado.
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param		O sistema de arquivos em que a operação será realizada.
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O processo cujos arquivos abertos serão fechados.
*/
void sistemaArquivos_fecharArquivosAbertosPara(SISTEMA_ARQUIVOS *sistemaArquivos_param, DESCRITOR_PROCESSO *descritorProcesso_param);

/**
* @param SISTEMA_ARQUIVOS		*sistemaArquivos_param			O sistema de arquivos em que a operação será realizada.
* @param DESCRITOR_PROCESSO		*descritorProcesso_param		O processo cuja possa do arquivo será testada.
* @param int					numeroDescritorArquivo_param	O arquivo de cuja posse será testada.
* @return int	Indica se o arquivo está aberto para o processo.
* ATENÇÃO: retornará false (0) se o arquivo não existir.
*/
int sistemaArquivos_arquivoEstahAbertoPara(SISTEMA_ARQUIVOS *sistemaArquivos_param, DESCRITOR_PROCESSO *descritorProcesso_param,
	int numeroDescritorArquivo_param);










