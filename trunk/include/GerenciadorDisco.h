/**
* Gerencia o acesso de processos a um disco determinado.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes

struct str_gerenciadorDisco{
	DISCO *discoGerenciado; //O disco cujo acesso é gerenciado.
	FIFO 
};

typedef struct str_gerenciadorDisco GERENCIADOR_DISCO;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* @param GERENCIADOR_DISCO	*gerenciado_param	O gerenciador que será inicializado.
*/
void gerenciadorDisco_inicializar(GERENCIADOR_DISCO *gerenciado_param);

/**
* Agenda a operação, para ser feita depois de todas as que estão agendadas.
* @param GERENCIADOR_DISCO		*gerenciador_param			O gerenciador que irá fazer o agendamento.
* @param int					endereco_param				O endereço da leitura.
*/
void gerenciadorDisco_agendarOperacaoLeitura(GERENCIRADOR_DISCO *gerenciador_param, int endereco_param);

/**
* Agenda a operação, para ser feita depois de todas as que estão agendadas.
* @param GERENCIADOR_DISCO		*gerenciador_param			O gerenciador que irá fazer o agendamento.
* @param int					endereco_param				O endereço da escrita.
* @param PALAVRA				*dados_param				Os dados que serão escritos.
* @param int					tamanhoDados_param			A quantidade de palavras que serão escritas.
*/
void gerenciadorDisco_agendarOperacaoEscrita(GERENCIRADOR_DISCO *gerenciador_param, int endereco_param,
		PALAVRA *dados_param, int tamanhoDados_param);

/**
* Agenda a operação, para ser feita depois de todas as que estão agendadas.
* @param GERENCIADOR_DISCO		*gerenciador_param			O gerenciador que irá fazer o agendamento.
* @param int					enderecoMemoria_param		Endereço em que a escrita será iniciada na memória.
* @param int					enderecoDisco_param			O endereço do disco à partir do qual os dados serão transferidos.
* @param int					quantidadePalavras_param	A quantidade de palavras que serão lidas.
*/
void gerenciadorDisco_agendarOperacaoCargaDMA(GERENCIRADOR_DISCO *gerenciador_param, int enderecoMemoria_param,
		int enderecoDisco_param, int quantidadePalavras_param);









