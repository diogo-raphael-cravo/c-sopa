#include "../include/DadosComuns.h"

/**
* Variáveis globais acessíveis somente neste arquivo.
*/



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
	
	destino_param = (char*) malloc(TAMANHOBUFFER*sizeof(char));
	memset(destino_param, '\0', TAMANHOBUFFER);
	//Recebe os dados do cliente
	if((bytesRecebidos = recv(sockCliente_param, destino_param, TAMANHOBUFFER, 0)) < 0) {
		tela_imprimirTelaAzulDaMorte(&global_tela, "Falha ao receber os dados do cliente");
	}	
	
	destino_param[bytesRecebidos] = SOCKET_SOPA_SEPARADOR;
	strcat(destino_param, inet_ntoa(cliente.sin_addr));

	close(sockCliente_param);
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
    if((clientSock = accept(serverSock, (struct sockaddr *) &cliente, &tamanhoCliente)) < 0){
		tela_imprimirTelaAzulDaMorte(&global_tela, "Falha ao tentar estabelecer comunicacao com o cliente");
    }

    GerenciaDados(clientSock, destino_param);
}

/**
* @param char*		ip_param			IP no formato "127.0.0.1". NENHUM campo deve começar em 0 (algo do tipo "127.0.0.01" causará erro).
* @param char*		mensagem_param		A mensagem que será enviada.
* ATENÇÃO: a mensagem já deve ter sido alocada!
*/
void socketSopa_enviarMensagem(char* ip_param, char* mensagem_param){
	int sock;
	struct sockaddr_in server;
	char buffer[TAMANHOBUFFER];
	unsigned int tamanhoArray;
	int totalDeBytes = 0;

	//Criacao do Socket TCP
	if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		tela_imprimirTelaAzulDaMorte(&global_tela, "Falha ao criar o socket");
	}

	//Preparando estrutura de enderecamento do socket
	memset(&server, 0, sizeof(server));		//Limpando a area de memoria da estrutura
	server.sin_family = AF_INET;			//IP
	server.sin_addr.s_addr = inet_addr();	//Aceita conexoes apenas do servidor
	server.sin_port = htons(PORTA_TCP);		//Especifica a porta do servidor

	//Estabelecimento da conexao com o servidor
	if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0){
		tela_imprimirTelaAzulDaMorte(&global_tela, "Falha ao conectar com o servidor");
	}

	//Envia o array de bytes para o servidor
	tamanhoArray = strlen(mensagem_param);
	if(send(sock, mensagem_param, tamanhoArray, 0) != tamanhoArray){
		tela_imprimirTelaAzulDaMorte(&global_tela, "Erro no envio dos dados");
	}

	close(sock);
}











