/**
* Concentra as funções de manipulação de RPCs.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define PLACA_REDE_PACOTE_INEXISTENTE NULL
#define MAXIMO_PACOTES_GUARDADOS 100

struct str_destinatarioPacote{
	char* IP; //IP no formato "127.0.0.1". NENHUM campo deve começar em 0 (algo do tipo "127.0.0.01" causará erro).
	int portaHospedeiro; //Porta do hospedeiro do SOPA.
};

typedef struct str_destinatarioPacote DESTINATARIO_PACOTE_SOPA;

struct str_placaRede{
	FIFO pacotesRecebidos;
	
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
* @param DESTINATARIO_PACOTE_SOPA	destino_param			O destino do pacote.
* @param PACOTE_APLICACAO_SOPA		*mensagem_param			A mensagem que será enviada.
*/
void placaRede_agendarEnvioMensagem(PLACA_REDE *placaRede_param, DESTINATARIO_PACOTE_SOPA destino_param, PACOTE_APLICACAO_SOPA *mensagem_param);

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












