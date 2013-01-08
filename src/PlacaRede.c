#include "../include/DadosComuns.h"

/**
* A placa de rede é composta de duas threads:
*	-Thread produtora: recebe pacotes e os coloca no buffer compartilhado de pacotes.
*	-Thread consumidora: verifica se há pacotes no buffer e gera interrupções sempre que houver.
*/


//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------
/**
* Roda o produtor, que recebe pacotes e os coloca no buffer compartilhado de pacotes.
* @param PLACA_REDE		*placaRede_param		A placa que irá rodar.
*/
void placaRede_rodarProdutor(PLACA_REDE *placaRede_param){
	char* stringRecebida = "";
	SOCKET_SOPA socket;

	socketSopa_inicializar(&socket);
	while(1){
		sem_wait(&placaRede_param->semaforoEspacos);
		stringRecebida = (char*) malloc(TAMANHOBUFFER*sizeof(char));
		socketSopa_esperarMensagem(&socket, stringRecebida);
		FIFO_inserir(&placaRede_param->pacotesRecebidos, pacoteAplicacaoSOPA_deString(stringRecebida));
		sem_post(&placaRede_param->semaforoItens);
	}
}

/**
* Roda o consumidor, que verifica se há pacotes no buffer e gera interrupções sempre que houver.
* @param PLACA_REDE		*placaRede_param		A placa que irá rodar.
*/
void placaRede_rodarConsumidor(PLACA_REDE *placaRede_param){
	while(1){
		sem_wait(&placaRede_param->semaforoItens);
		controladorInterrupcoes_set(&global_controladorInterrupcoes, INTERRUPCAO_PLACA_REDE_RECEIVE);
		sem_post(&placaRede_param->semaforoEspacos);
	}
}

//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/**
* @param PLACA_REDE		*placaRede_param		A placa que será inicializada.
*/
void placaRede_inicializar(PLACA_REDE *placaRede_param){
	FIFO_inicializar(&placaRede_param->pacotesRecebidos, MAXIMO_PACOTES_GUARDADOS);
	sem_init(&placaRede_param->semaforoItens, 0, 0);
	sem_init(&placaRede_param->semaforoEspacos, 0, MAXIMO_PACOTES_GUARDADOS);
	sem_init(&placaRede_param->mutexDadosEnvio, 0, 1);
	placaRede_param->erro = ERRO_REDE_NENHUM;
	placaRede_param->dadosRequerente = NULL;
}

/**
* @param PLACA_REDE		*placaRede_param		A placa que irá rodar.
*/
void placaRede_rodar(PLACA_REDE *placaRede_param){
	pthread_create(&placaRede_param->threadIdConsumidor, NULL, placaRede_rodarConsumidor, placaRede_param); //UMA thread
	placaRede_rodarProdutor(placaRede_param);								 //OUTRA thread
}

/**
* @param PLACA_REDE					*placaRede_param		A placa que realizará a operação.
* @param char*						ipDestino_param			IP no formato "127.0.0.1". NENHUM campo 
*															deve começar em 0 (algo do tipo "127.0.0.01" causará erro).
* @param char*						*mensagem_param			A mensagem que será enviada.
* @param void*						*dadosRequerente_param	Dados que estarão disponíveis depois que uma
*															mensagem for enviada.
*															É de responsabilidade de quem chamar cuidar o TIPO e alocação!
*/
void placaRede_agendarEnvioMensagem(PLACA_REDE *placaRede_param, char* ipDestino_param, char* mensagem_param, void* dadosRequerente_param){
	socketSopa_enviarMensagem(ipDestino_param, mensagem_param, dadosRequerente_param);
}

/**
* @param PLACA_REDE					*placaRede_param		A placa que será consultada.
* @return PACOTE_APLICACAO_SOPA*	O primeiro pacote na fila de pacotes ainda não vistos, isto é, aquele que está esperando há mais tempo
*									e que ainda não foi passado com placaRede_avancarFilaPacotesRecebidos.
*									Retornará PLACA_REDE_PACOTE_INEXISTENTE, se não houver.
*/
PACOTE_APLICACAO_SOPA* placaRede_getProximoPacoteRecebido(PLACA_REDE *placaRede_param){
	PACOTE_APLICACAO_SOPA* pacote = (PACOTE_APLICACAO_SOPA*) FIFO_espiar(&placaRede_param->pacotesRecebidos);
	return pacote;
}

/**
* Avança a fila de pacotes não vistos em uma posição, caso seja possível.
* @param PLACA_REDE		*placaRede_param		A placa que será editada.
* ATENÇÃO: irá deletar o pacote que sair da fila!
*/
void placaRede_avancarFilaPacotesRecebidos(PLACA_REDE *placaRede_param){
	if(!FIFO_vazia(&placaRede_param->pacotesRecebidos)){
		PACOTE_APLICACAO_SOPA *pacote = (PACOTE_APLICACAO_SOPA*) FIFO_remover(&placaRede_param->pacotesRecebidos);
		pacoteAplicacaoSOPA_destruir(pacote);
	}
}

/**
* @param PLACA_REDE		*placaRede_param		A placa de rede cujo erro será consultado.
* @return ERRO_REDE		Erro ocorrido na última requisição feita à placa.
*/
ERRO_REDE placaRede_getErroUltimaOperacao(PLACA_REDE *placaRede_param){
	return placaRede_param->erro;
}

/**
* @param PLACA_REDE		*placaRede_param		A placa de rede cujo erro será consultado.
* @param ERRO_REDE		erro_param				Erro ocorrido na última requisição feita à placa.
*/
void placaRede_setErroUltimaOperacao(PLACA_REDE *placaRede_param, ERRO_REDE erro_param){
	placaRede_param->erro = erro_param;
}

/**
* @param PLACA_REDE		*placaRede_param		A placa de rede cujo erro será consultado.
* @return void*		Dados que foram fornecidos junto a mensagem que acaba de ser enviada.
*					É de responsabilidade de quem chamar cuidar o TIPO e alocação!
*/
void* placaRede_getDadosUltimaOperacao(PLACA_REDE *placaRede_param){
	return placaRede_param->dadosRequerente;
}

/**
* @param PLACA_REDE		*placaRede_param			A placa de rede cujo erro será consultado.
* @param void*			*dadosRequerente_param		Dados que estarão disponíveis depois que uma
*													mensagem for enviada.
*													É de responsabilidade de quem chamar cuidar o TIPO e alocação!
*/
void placaRede_setDadosUltimaOperacao(PLACA_REDE *placaRede_param, void* dadosRequerente_param){
	placaRede_param->dadosRequerente = dadosRequerente_param;
}

/**
* @param PLACA_REDE		*placaRede_param		Tranca acesso aos dados de envio (erro e requerente).
*/
void placaRede_travarAcessoDados(PLACA_REDE *placaRede_param){
	sem_wait(&placaRede_param->mutexDadosEnvio);
}

/**
* @param PLACA_REDE		*placaRede_param		Espera acesso aos dados de envio (erro e requerente) ser liberado.
*/
void placaRede_esperarAcessoDados(PLACA_REDE *placaRede_param){
	sem_wait(&placaRede_param->mutexDadosEnvio);
}

/**
* @param PLACA_REDE		*placaRede_param		Libera acesso aos dados de envio (erro e requerente).
*/
void placaRede_liberarAcessoDados(PLACA_REDE *placaRede_param){
	sem_post(&placaRede_param->mutexDadosEnvio);
}


