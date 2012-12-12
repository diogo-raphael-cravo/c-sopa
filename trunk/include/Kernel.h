/**
* Concentra as funções de manipulação do kernel.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define MAXIMO_PROCESSOS_KERNEL 10
#define DESCRITOR_PROCESSO_INEXISTENTE NULL

struct str_alocacaoMemoria{
	int enderecoInicio; //Início do programa na memória.
	int tamanhoAreaMemoriaPalavras; //Tamanho da área de memória reservada ao programa, em palavras.
};

typedef struct str_alocacaoMemoria ALOCACAO_MEMORIA;

struct str_kernel{
	FIFO filaProcessosBloqueados; //Indica os índices de descritoresProcessos que contém processos bloqueados.
									//Os processos são ordenados em uma fila por ordem de "chegada".
	FIFO filaProcessosProntos; //Indica os índices de descritoresProcessos que contém processos prontos para rodar.
								//Os processos são ordenados em uma fila por ordem de "chegada".
	FIFO filaMemoriaAlocada; //Representa o que foi alocado de memória, com elementos ALOCACAO_MEMORIA.
	DESCRITOR_PROCESSO** processoRodando; //Ponteiro para o processo que está rodando.
	int quantidadeProcessos; //A quantidade de processos no momento.
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











