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
#define MAXIMO_ARQUIVOS 20 //A quantidade máxima de arquivos neste sistema de arquivos.
#define DIRETORIO_DADOS_DISCO "dados" //Caminho para o diretório onde ficam os arquivos do disco no hospedeiro.
#define CAMINHO_ARQUIVO_DESCRITOR_SISTEMA_ARQUIVOS "dados/descritorSistemaArquivos"

struct str_sistemaArquivos{
	FIFO arquivos; //Guarda todos arquivos neste sistema de arquivos.
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
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param	O sistema de arquivos em que a operação será realizada.
* @param char*				nomeProcurado_param		O nome do arquivo que se quer.
* @return ARQUIVO*	O arquivo que tem o nome procurado. Caso não haja, retonará NULL.
*/
ARQUIVO* sistemaArquivos_buscaPorNome(SISTEMA_ARQUIVOS *sistemaArquivos_param, char* nomeProcurado_param);

/**
* Atualiza os arquivos na máquina hospedeira.
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param	O sistema de arquivos que será atualizado.
*/
void sistemaArquivos_atualizarNaMaquinaHospedeira(SISTEMA_ARQUIVOS *sistemaArquivos_param);









