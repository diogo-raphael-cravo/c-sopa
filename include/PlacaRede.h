/**
* Concentra as funções de manipulação de RPCs.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define PLACA_REDE_PACOTE_INEXISTENTE NULL
#define MAXIMO_PACOTES_GUARDADOS 100

struct str_placaRede{
	FIFO pacotesRecebidos;
	pthread_t threadIdConsumidor;
	sem_t semaforoItens;
	sem_t semaforoEspacos;
	sem_t mutexDadosEnvio;
	ERRO_REDE erro;
	DADOS_REQUERENTE* dadosRequerente;
};

typedef struct str_placaRede PLACA_REDE;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* @param PLACA_REDE		*placaRede_param		A placa que será inicializada.
*/
void placaRede_inicializar(PLACA_REDE *placaRede_param);

/**
* @param PLACA_REDE		*placaRede_param		A placa que irá rodar.
*/
void placaRede_rodar(PLACA_REDE *placaRede_param);

/**
* @param PLACA_REDE					*placaRede_param		A placa que realizará a operação.
* @param char*						ipDestino_param			IP no formato "127.0.0.1". NENHUM campo 
*															deve começar em 0 (algo do tipo "127.0.0.01" causará erro).
* @param char*						*mensagem_param			A mensagem que será enviada.
* @param DADOS_REQUERENTE*			*dadosRequerente_param	Dados que estarão disponíveis depois que uma
*															mensagem for enviada.
*/
void placaRede_agendarEnvioMensagem(PLACA_REDE *placaRede_param, char* ipDestino_param, char* mensagem_param, 
		DADOS_REQUERENTE* dadosRequerente_param);

/**
* @param PLACA_REDE					*placaRede_param		A placa que será consultada.
* @return PACOTE_APLICACAO_SOPA*	O primeiro pacote na fila de pacotes ainda não vistos, isto é, aquele que está esperando há mais tempo
*									e que ainda não foi passado com placaRede_avancarFilaPacotesRecebidos.
*									Retornará PLACA_REDE_PACOTE_INEXISTENTE, se não houver.
*/
PACOTE_APLICACAO_SOPA* placaRede_getProximoPacoteRecebido(PLACA_REDE *placaRede_param);

/**
* Avança a fila de pacotes não vistos em uma posição, caso seja possível.
* @param PLACA_REDE		*placaRede_param		A placa que será editada.
* ATENÇÃO: irá deletar o pacote que sair da fila!
*/
void placaRede_avancarFilaPacotesRecebidos(PLACA_REDE *placaRede_param);

/**
* @param PLACA_REDE		*placaRede_param		A placa de rede cujo erro será consultado.
* @return ERRO_REDE		Erro ocorrido na última requisição feita à placa.
*/
ERRO_REDE placaRede_getErroUltimaOperacao(PLACA_REDE *placaRede_param);

/**
* @param PLACA_REDE		*placaRede_param		A placa de rede cujo erro será consultado.
* @param ERRO_REDE		erro_param				Erro ocorrido na última requisição feita à placa.
*/
void placaRede_setErroUltimaOperacao(PLACA_REDE *placaRede_param, ERRO_REDE erro_param);

/**
* @param PLACA_REDE		*placaRede_param		A placa de rede cujo erro será consultado.
* @return DADOS_REQUERENTE*		Dados que estarão disponíveis depois que uma mensagem for enviada.
*/
DADOS_REQUERENTE* placaRede_getDadosUltimaOperacao(PLACA_REDE *placaRede_param);

/**
* @param PLACA_REDE			*placaRede_param			A placa de rede cujo erro será consultado.
* @param DADOS_REQUERENTE*	*dadosRequerente_param		Dados que estarão disponíveis depois que uma
*														mensagem for enviada.
*/
void placaRede_setDadosUltimaOperacao(PLACA_REDE *placaRede_param, DADOS_REQUERENTE* dadosRequerente_param);

/**
* @param PLACA_REDE		*placaRede_param		Tranca acesso aos dados de envio (erro e requerente).
*/
void placaRede_travarAcessoDados(PLACA_REDE *placaRede_param);

/**
* @param PLACA_REDE		*placaRede_param		Espera acesso aos dados de envio (erro e requerente) ser liberado.
*/
void placaRede_esperarAcessoDados(PLACA_REDE *placaRede_param);

/**
* @param PLACA_REDE		*placaRede_param		Libera acesso aos dados de envio (erro e requerente).
*/
void placaRede_liberarAcessoDados(PLACA_REDE *placaRede_param);










