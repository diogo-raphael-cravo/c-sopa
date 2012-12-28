/**
* Concentra as funções de manipulação do kernel.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define MAXIMO_PROCESSOS_KERNEL 30 //Sabemos que não há limite, mas nossa implementação necessita de um limite, ainda que altíssimo.
#define DESCRITOR_PROCESSO_INEXISTENTE NULL
#define FATIA_TEMPO_TICKS_PROCESSO 5 //Fatia de tempo dada a cada processo, medida em ticks do timer.

enum enum_errosKernel{
	KERNEL_ERRO_NENHUM,
	KERNEL_ERRO_ARQUIVO_INEXISTENTE,
	KERNEL_ERRO_CRIANDO_PROCESSO,
	KERNEL_ERRO_MEMORIA_INSUFICIENTE,
	KERNEL_ERRO_MAXIMO_PROCESSOS_ATINGIDO,
	KERNEL_ERRO_PROCESSO_NAO_TEM_ARQUIVO,
	KERNEL_ERRO_DISCO_OCUPADO,
	KERNEL_ERRO_FIM_DO_ARQUIVO
};

typedef enum enum_errosKernel ERRO_KERNEL;

enum enum_comandosUsuario{
	COMANDO_NENHUM,
	COMANDO_EXECUCAO_PROGRAMA,
	COMANDO_IMPRIMIR,
	COMANDO_AJUDA
};

typedef enum enum_comandosUsuario COMANDO_USUARIO;

struct str_kernel{
		//Espera para escalonamento
	FIFO filaProcessosBloqueados; //Processos que estão bloqueados, por algum motivo. 
									//DIFERENTE de filaProcessosRequisicaoDisco.
	FIFO filaProcessosProntos; //Indica os índices de descritoresProcessos que contém processos prontos para rodar.
								//Os processos são ordenados em uma fila por ordem de "chegada".

		//Kernel
	MAPA_ALOCACOES_MEMORIA mapaMemoriaAlocada; //Representa o que foi alocado de memória, permitindo controle de endereçamento lógico.
	SISTEMA_ARQUIVOS sistemaDeArquivos; //O sistema de arquivos, com todos os arquivos em todos os discos!
	DESCRITOR_PROCESSO** processoRodando; //Ponteiro para o processo que está rodando.
	ARQUIVO *arquivoTransferido; //Arquivo que está sendo transferido para a memória.
	GERENCIADOR_DISCO gerenciadorAcessoDisco; //Gerencia acesso às operações do disco.
	int quantidadeProcessos; //A quantidade de processos no momento.
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











