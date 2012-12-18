/**
* Concentra as funções de manipulação do kernel.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define MAXIMO_PROCESSOS_KERNEL 30
#define DESCRITOR_PROCESSO_INEXISTENTE NULL

enum enum_errosKernel{
	KERNEL_ERRO_NENHUM,
	KERNEL_ERRO_ARQUIVO_INEXISTENTE,
	KERNEL_ERRO_CRIANDO_PROCESSO
};

typedef enum enum_errosKernel ERRO_KERNEL;

enum enum_comandosUsuario{
	COMANDO_NENHUM,
	COMANDO_EXECUCAO_PROGRAMA,
	COMANDO_IMPRIMIR,
	COMANDO_AJUDA
};

typedef enum enum_comandosUsuario COMANDO_USUARIO;

struct str_processoEsperandoDisco{ //Relaciona um processo que está esperando com o motivo pelo qual ele está esperando.
	DESCRITOR_PROCESSO** processoEsperando;
	OPERACAO_DISCO motivoEspera; //Operação que será feita pelo disco.
};

typedef struct str_processoEsperandoDisco PROCESSO_ESPERANDO;

struct str_kernel{
	FIFO filaProcessosBloqueados; //Indica os índices de descritoresProcessos que contém processos bloqueados.
									//Os processos são ordenados em uma fila por ordem de "chegada".
	FIFO filaProcessosProntos; //Indica os índices de descritoresProcessos que contém processos prontos para rodar.
								//Os processos são ordenados em uma fila por ordem de "chegada".
	FIFO filaProcessosRequisicaoDisco; //Os processos que estão esperando para fazer requisição ao disco.
	MAPA_ALOCACOES_MEMORIA mapaMemoriaAlocada; //Representa o que foi alocado de memória, permitindo controle de endereçamento lógico.
	SISTEMA_ARQUIVOS sistemaDeArquivos; //O sistema de arquivos, com todos os arquivos em todos os discos!
	DESCRITOR_PROCESSO** processoRodando; //Ponteiro para o processo que está rodando.
	ARQUIVO *arquivoTransferido; //Arquivo que está sendo transferido para a memória.
	int quantidadeProcessos; //A quantidade de processos no momento.
	int fazendoTransferenciaDiscoMemoria; //Indica se o kernel está realizando uma transferência do disco para a memória.
	int criandoProcesso; //Indica se o kernel está criando um processo.
	int ultimoPIDUsado; //Último PID utilizado para um processo deste kernel.
};

typedef struct str_kernel KERNEL;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* @param KERNEL	*kernel_param	O kernel que será inicializado.
*/
void kernel_inicializar(KERNEL *kernel_param);

/**
* @param KERNEL			*kernel_param		O kernel que irá rodar.
* @param INTERRUPCAO	interrupcao_param	A interrupção que definirá o comportamento do kernel.
*/
void kernel_rodar(KERNEL *kernel_param, INTERRUPCAO interrupcao_param);











