#include "../../include/DadosComuns.h"

/**
* Variáveis globais acessíveis somente neste arquivo.
*/


//---------------------------------------------------------------------
//			DEFINIÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------

struct str_mensagemSocketSOPA{
	char* ip;
	char* conteudo;
	PLACA_REDE *placaRede;
	void* dadosRequerente;
};

typedef struct str_mensagemSocketSOPA MENSAGEM_SOCKET_SOPA;

//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------
/**
* @param int	sockCliente_param	O número socket do cliente.
* @param char*	destino_param		Destino em que os dados serão colocados.
*									Ao final, concatena o IP de origem da mensagem.
*									Separa da mensagem com SOCKET_SOPA_SEPARADOR.
*/
void GerenciaDados(int sockCliente_param, char* destino_param) {
	int bytesRecebidos;
	memset(destino_param, '\0', TAMANHOBUFFER);
	//Recebe os dados do cliente
	if((bytesRecebidos = recv(sockCliente_param, destino_param, TAMANHOBUFFER, 0)) < 0) {
		tela_imprimirTelaAzulDaMorte(&global_tela, "Falha ao receber os dados do cliente");
	}
	destino_param[bytesRecebidos] = '\0';
	close(sockCliente_param);
}

/**
* @param int	*sock_param					Socket que será utilizado para enviar a mensagem.
* @param char	*mensagem_param				Mensagem que será enviada.
* ATENÇÃO: irá destruir a thread que a executar!
*/
void privada_enviarMensagem(MENSAGEM_SOCKET_SOPA *mensagem_param){
	unsigned int tamanhoArray = strlen(mensagem_param->conteudo);
	ERRO_REDE erro = ERRO_REDE_NENHUM;
	int sock;
	struct sockaddr_in server;
	//Criac5ao do Socket TCP
	if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		placaRede_setErroUltimaOperacao(mensagem_param->placaRede, ERRO_REDE_CRIACAO_SOCKET);
		erro = ERRO_REDE_CRIACAO_SOCKET;
	}
	if(erro == ERRO_REDE_NENHUM){
		//Preparando estrutura de enderecamento do socket
		memset(&server, 0, sizeof(server));							//Limpando a area de memoria da estrutura
		server.sin_family = AF_INET;								//IP
		server.sin_addr.s_addr = inet_addr(mensagem_param->ip);		//Aceita conexoes apenas do servidor
		server.sin_port = htons(PORTA_TCP);							//Especifica a porta do servidor
		//Estabelecimento da conexao com o servidor
		if(connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0){
			erro = ERRO_REDE_CONEXAO_SERVIDOR;
		}
	}
	if(erro == ERRO_REDE_NENHUM){
		//Envia o array de bytes para o servidor
		char *mensagemEnviada = (char*) malloc((strlen(mensagem_param->conteudo)+1)*sizeof(char));
		strcpy(mensagemEnviada, mensagem_param->conteudo);
		if(send(sock, mensagemEnviada, tamanhoArray, 0) != tamanhoArray){
			erro = ERRO_REDE_ENVIO_DADOS;
		}
		free(mensagemEnviada);
		close(sock);
	}

	placaRede_travarAcessoDados(mensagem_param->placaRede);
	controladorInterrupcoes_set(&global_controladorInterrupcoes, INTERRUPCAO_PLACA_REDE_SEND);
	placaRede_setErroUltimaOperacao(mensagem_param->placaRede, erro);
	placaRede_setDadosUltimaOperacao(mensagem_param->placaRede, mensagem_param->dadosRequerente);
	placaRede_liberarAcessoDados(mensagem_param->placaRede);
	free(mensagem_param->ip);
	free(mensagem_param->conteudo);
	free(mensagem_param);
	pthread_exit(0);
}

//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/**
* @param SOCKET_SOPA		*socket_param		O socket que será inicializado.
*/
void socketSopa_inicializar(SOCKET_SOPA *socket_param){
	//Criando o Socket TCP
	if((socket_param->serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		tela_imprimirTelaAzulDaMorte(&global_tela, "Falha ao criar o socket TCP");
	}

	//Preparando estrutura de enderecamento do socket
	memset(&socket_param->servidor, 0, sizeof(socket_param->servidor));	//Limpando a area de memoria da estrutura
	socket_param->servidor.sin_family = AF_INET;						//Internet/IP
	socket_param->servidor.sin_addr.s_addr = htonl(INADDR_ANY);			//Aceita conexoes de qualquer endereco IP
	socket_param->servidor.sin_port = htons(PORTA_TCP);					//Especifica a porta do servidor

	//Une o socket a estrutura
	if(bind(socket_param->serverSock, (struct sockaddr *) &socket_param->servidor,sizeof(socket_param->servidor)) < 0){
		tela_imprimirTelaAzulDaMorte(&global_tela, "Falha ao realizar o bind");
	}

	//Coloca o Socket no modo passivo aguardando por conexoes
	if(listen(socket_param->serverSock, TAMANHOFILA) < 0){
		tela_imprimirTelaAzulDaMorte(&global_tela, "Falha ao colocar o socket no modo passivo");
	}
}

/**
* @param SOCKET_SOPA		*socket_param		O socket que irá esperar.
* @param char*				destino_param		String na qual será colocada a mensagem recebida.
*												Ao final, concatena o IP de origem da mensagem.
*												Separa da mensagem com SOCKET_SOPA_SEPARADOR.
*/
void socketSopa_esperarMensagem(SOCKET_SOPA *socket_param, char* destino_param){
	int clientSock;
	struct sockaddr_in cliente;

	unsigned int tamanhoCliente = sizeof(cliente);
	//Aguarda pela conexao de algum cliente

	if((clientSock = accept(socket_param->serverSock, (struct sockaddr *) &cliente, &tamanhoCliente)) < 0){
		tela_imprimirTelaAzulDaMorte(&global_tela, "Falha ao tentar estabelecer comunicacao com o cliente");
	}
	GerenciaDados(clientSock, destino_param);
	strcat(destino_param, inet_ntoa(cliente.sin_addr));
	strcat(destino_param, SOCKET_SOPA_SEPARADOR_STRING);
}

/**
* @param char*			ip_param					IP no formato "127.0.0.1". NENHUM campo deve começar em 0 (algo do tipo "127.0.0.01" causará erro).
* @param char*			mensagem_param				A mensagem que será enviada.
* @param void*			*dadosRequerente_param		Dados que estarão disponíveis depois que uma
*													mensagem for enviada.
*													É de responsabilidade de quem chamar cuidar o TIPO e alocação!
* ATENÇÃO: a mensagem já deve ter sido alocada!
*/
void socketSopa_enviarMensagem(char* ip_param, char* mensagem_param, void* dadosRequerente_param){
	pthread_t *threadIdEnvioMensagem = (pthread_t*) malloc(sizeof(pthread_t));
	MENSAGEM_SOCKET_SOPA *mensagem = (MENSAGEM_SOCKET_SOPA*) malloc(sizeof(MENSAGEM_SOCKET_SOPA));
	mensagem->ip = (char*) malloc((strlen(ip_param)+1)*sizeof(char));
	strcpy(mensagem->ip, ip_param);
	mensagem->conteudo = (char*) malloc((strlen(mensagem_param)+1)*sizeof(char));
	strcpy(mensagem->conteudo, mensagem_param);
	mensagem->placaRede = &global_placaRede;
	mensagem->dadosRequerente = dadosRequerente_param;
	pthread_create(threadIdEnvioMensagem, NULL, privada_enviarMensagem, mensagem);
}












