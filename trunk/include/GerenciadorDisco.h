/**
* Gerencia o acesso de processos a um disco determinado.
* É constituído por um mapeamento de operações do kernel em operações do disco,
* de forma que torna-se simples consultar a próxima operação do disco e a operação do kernel
* que aguarda a realização dessa.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes

	//Operações do disco.
struct str_operacaoLeituraDisco{
	int endereco;
};

typedef struct str_operacaoLeituraDisco OPERACAO_LEITURA_DISCO;

struct str_operacaoEscritaDisco{
	int endereco;
	PALAVRA *dados;
	int tamanhoDados;
};

typedef struct str_operacaoEscritaDisco OPERACAO_ESCRITA_DISCO;

struct str_operacaoCargaDMADisco{
	int enderecoMemoria;
	int enderecoDisco;
	int quantidadePalavras;
};

typedef struct str_operacaoCargaDMADisco OPERACAO_CARGA_DMA_DISCO;

struct str_operacaoDisco{
	void* operacao; 
	TIPO_OPERACAO_DISCO tipoOperacao; 
};

typedef struct str_operacaoDisco OPERACAO_DISCO;

	//Operações do kernel
struct str_operacaoCriacaoProcesso{
	DESCRITOR_PROCESSO **processoCriado;
};

typedef struct str_operacaoCriacaoProcesso OPERACAO_CRIACAO_PROCESSO_KERNEL;

struct str_operacaoFeitaPorProcesso{
	
};

typedef struct str_operacaoFeitaPorProcesso OPERACAO_FEITA_POR_PROCESSO_KERNEL;

enum enum_operacoesAgendaveisKernel{
	TIPO_OPERACAO_AGENDAVEL_CRIACAO_PROCESSO_KERNEL,
	TIPO_OPERACAO_AGENDAVEL_FEITA_POR_PROCESSO_KERNEL
};

typedef enum enum_operacoesAgendaveisKernel TIPO_OPERACAO_AGENDAVEL_KERNEL;

struct str_operacaoKernel{
	void* operacao; 
	TIPO_OPERACAO_AGENDAVEL_KERNEL tipoOperacao; 
};

typedef struct str_operacaoKernel OPERACAO_KERNEL;

	//Gerenciador
struct str_gerenciadorDisco{
	DISCO *discoGerenciado; //O disco cujo acesso é gerenciado.
	OPERACAO_KERNEL* proximaOperacaoKernel; //A operação do kernel que estava relacionada com a última operação do disco
							//que foi mandada executar. Retornará NULL, se não houver.
	int proximaOperacaoKernelConsultada; //Trava a execução de novas operações do disco até que
										//a próxima operação do kernel seja consultada.
	FIFO filaOperacoesDisco; //Fila de operações do disco, que estão aguardando para serem realizadas.
	FIFO filaOperacoesKernel; //Fila de operações que devem ser executadas pelo kernel, 
							//após realização da ação correspondente pelo disco.
};

typedef struct str_gerenciadorDisco GERENCIADOR_DISCO;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* @param GERENCIADOR_DISCO	*gerenciado_param	O gerenciador que será inicializado.
* @param DISCO 				*disco_param		O disco gerenciado.
*/
void gerenciadorDisco_inicializar(GERENCIADOR_DISCO *gerenciador_param, DISCO *disco_param);

/**
* @param GERENCIADOR_DISCO		*gerenciador_param			O gerenciador que irá fazer o agendamento.
* @param OPERACAO_DISCO*		operacaoDisco_param			A operação do disco que irá esperar para executar.
* @param OPERACAO_KERNEL*		operacaoKernel_param		A operação do kernel 
* ATENÇÃO: se o disco estiver livre, o mandará executar a próxima direto.
*/
void gerenciadorDisco_agendar(GERENCIADOR_DISCO *gerenciador_param, OPERACAO_DISCO* operacaoDisco_param, 
		OPERACAO_KERNEL* operacaoKernel_param);

/**
* @param	GERENCIADOR_DISCO		*gerenciador_param	O gerenciador que irá mandar 
*														e que contém o disco que realizará a operação.
* ATENÇÃO: não permitirá execução duas vezes seguidas sem que a próxima operação do kernel seja consultada com 
* gerenciadorDisco_proximaOperacaoKernel!
*/
void gerenciadorDisco_executarProxima(GERENCIADOR_DISCO *gerenciador_param);

/**
* @param 	GERENCIADOR_DISCO		*gerenciador_param	O gerenciador que mandou executar uma operação 
* @return OPERACAO_KERNEL*	A operação do kernel que estava relacionada com a última operação do disco
*							que foi mandada executar. Retornará NULL, se não houver.
*/
OPERACAO_KERNEL* gerenciadorDisco_proximaOperacaoKernel(GERENCIADOR_DISCO *gerenciador_param);

	/**
	* Operações do KERNEL
	**/
/**
* @param DESCRITOR_PROCESSO**	descritorProcesso_param		O processo que será criado.
* @return OPERACAO_KERNEL*	A operação criada.
*/
OPERACAO_KERNEL* gerenciadorDisco_criarOperacaoKernelCriacaoProcesso(DESCRITOR_PROCESSO **descritorProcesso_param);

/**
* @return OPERACAO_KERNEL*	A operação criada.
*/
OPERACAO_KERNEL* gerenciadorDisco_criarOperacaoKernelFeitaPorProcesso();

	/**
	* Operações do DISCO
	**/
/**
* Agenda a operação, para ser feita depois de todas as que estão agendadas.
* @param int					endereco_param				O endereço da leitura.
* @return OPERACAO_DISCO* 	A operação criada.
*/
OPERACAO_DISCO* gerenciadorDisco_criarOperacaoDiscoLeitura(int endereco_param);

/**
* Agenda a operação, para ser feita depois de todas as que estão agendadas.
* @param int					endereco_param				O endereço da escrita.
* @param PALAVRA				*dados_param				Os dados que serão escritos.
* @param int					tamanhoDados_param			A quantidade de palavras que serão escritas.
* @return OPERACAO_DISCO* 	A operação criada.
*/
OPERACAO_DISCO* gerenciadorDisco_criarOperacaoDiscoEscrita(int endereco_param,
		PALAVRA *dados_param, int tamanhoDados_param);

/**
* Agenda a operação, para ser feita depois de todas as que estão agendadas.
* @param int					enderecoMemoria_param		Endereço em que a escrita será iniciada na memória.
* @param int					enderecoDisco_param			O endereço do disco à partir do qual os dados serão transferidos.
* @param int					quantidadePalavras_param	A quantidade de palavras que serão lidas.
* @return OPERACAO_DISCO* 	A operação criada.
*/
OPERACAO_DISCO* gerenciadorDisco_criarOperacaoDiscoCargaDMA(int enderecoMemoria_param,
		int enderecoDisco_param, int quantidadePalavras_param);









