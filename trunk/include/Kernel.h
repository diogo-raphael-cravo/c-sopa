/**
* Concentra as funções de manipulação do kernel.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define MAXIMO_PROCESSOS_KERNEL 10
#define INDICE_PROCESSO_INEXISTENTE -1

struct str_kernel{
	DESCRITOR_PROCESSO descritoresProcessos[MAXIMO_PROCESSOS_KERNEL]; //Contém os processos no kernel.
    int indicesDescritoresProcessosExistentes[MAXIMO_PROCESSOS_KERNEL]; //Indica os índices de descritoresProcessos que contém processos neste kernel.
                                                                        //Índices que não estiverem aqui não são processos, não existem.
	int filaProcessosBloqueados[MAXIMO_PROCESSOS_KERNEL]; //Indica os índices de descritoresProcessos que contém processos bloqueados.
															//Os processos são ordenados em uma fila por ordem de "chegada".
	int filaProcessosProntos[MAXIMO_PROCESSOS_KERNEL]; //Indica os índices de descritoresProcessos que contém processos prontos para rodar.
														//Os processos são ordenados em uma fila por ordem de "chegada".
	int processoRodando; //Índice em descritoresProcessos do processo que está rodando.
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
