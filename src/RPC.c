#include "../include/DadosComuns.h"

/**
* Variáveis globais acessíveis somente neste arquivo.
*/



//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/* servidor
enum erros {WSTARTUP, ABRESOCK, BIND, ACCEPT, LISTEN,RECEIVE}; 

void TrataErro(SOCKET, int);

int main(int argc, char* argv[])
{
  SOCKET s=0, s_cli;
  struct sockaddr_in  addr_serv, addr_cli;
  int addr_cli_len=sizeof(addr_cli);

#ifdef _WIN32
	 WSADATA wsaData;
  
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
		TrataErro(s, WSTARTUP);
		exit(1);
		}
#endif

  char recvbuf[MAX_PACKET];

  // Cria o socket na familia AF_INET (Internet) e do tipo TCP (SOCK_STREAM)
  if ((s = socket(AF_INET, SOCK_STREAM, 0))==INVALID_SOCKET)
	  TrataErro(s, ABRESOCK);

  // Define domínio, IP e porta a receber dados
  addr_serv.sin_family = AF_INET;
  addr_serv.sin_addr.s_addr = htonl(INADDR_ANY); // recebe de qualquer IP
  addr_serv.sin_port = htons(PORTA_SRV);

  // Associa socket com estrutura addr_serv
  if ((bind(s, (struct sockaddr *)&addr_serv, sizeof(addr_serv))) != 0)
	  TrataErro(s, BIND);

  // Coloca socket em estado de escuta para as conexoes na porta especificada
  if((listen(s, 8)) != 0) // permite ateh 8 conexoes simultaneas
	  TrataErro(s, LISTEN);

  // permite conexoes entrantes utilizarem o socket
  if((s_cli=accept(s, (struct sockaddr *)&addr_cli, &addr_cli_len)) < 0)
	  TrataErro(s, ACCEPT);

  // fica esperando chegar mensagem
  while(1)
  {
    if ((recv(s_cli, recvbuf, MAX_PACKET, 0)) < 0)
    {
      close(s_cli);
	  TrataErro(s, RECEIVE);
    }

    // mostra na tela
    if(strcmp((const char *)&recvbuf, "q")==0)
      break;
    else
      printf(" - msg recv - %s\n", recvbuf);
  }

  // fecha socket e termina programa
  printf("Fim da conexao\n");
  close(s);
  close(s_cli);
  exit(1);
}

void TrataErro(SOCKET s, int tipoerro)
{
	char tipo[20];

	switch(tipoerro) {
		case WSTARTUP:
			strcpy(tipo, "Windows Startup");
			break;
		case ABRESOCK:
			strcpy(tipo, "Open Socket");
			break;
		case BIND:
			strcpy(tipo, "Bind");
			break;
		case ACCEPT:
			strcpy(tipo, "Accept");
			break;
		case LISTEN:
			strcpy(tipo, "Listen");
			break;
		case RECEIVE:
			strcpy(tipo, "Receive");
			break;
		default:
			strcpy(tipo, "Indefinido. Verificar");
			break;
	}
    printf("erro no %s", tipo);
    close(s);
    exit(1);
}

*/
/* cliente
#define PORTA_CLI 2345 // porta TCP do cliente
#define PORTA_SRV 2023 // porta TCP do servidor
#define STR_IPSERVIDOR "127.0.0.1"
//#define STR_IPSERVIDOR "192.168.0.146"

int main(int argc, char* argv[])
{
  SOCKET s;
  struct sockaddr_in  s_cli, s_serv;
  

  // abre socket TCP
  if ((s = socket(AF_INET, SOCK_STREAM, 0))==INVALID_SOCKET)
  {
    printf("Erro iniciando socket\n");
    return(0);
  }

  // seta informacoes IP/Porta locais
  s_cli.sin_family = AF_INET;
  s_cli.sin_addr.s_addr = htonl(INADDR_ANY);
  s_cli.sin_port = htons(PORTA_CLI);

  // associa configuracoes locais com socket
  if ((bind(s, (struct sockaddr *)&s_cli, sizeof(s_cli))) != 0)
  {
    printf("erro no bind\n");
    close(s);
    return(0);
  }

  // seta informacoes IP/Porta do servidor remoto
  s_serv.sin_family = AF_INET;
  s_serv.sin_addr.s_addr = inet_addr(STR_IPSERVIDOR);
  s_serv.sin_port = htons(PORTA_SRV);

  // connecta socket aberto no cliente com o servidor
  if(connect(s, (struct sockaddr*)&s_serv, sizeof(s_serv)) != 0)
  {
    //printf("erro na conexao - %d\n", WSAGetLastError());
    printf("erro na conexao");
    close(s);
    exit(1);
  }

  // recebe do teclado e envia ao servidor
  char str[1250];
  char ch;
  int i;

  while(1)
  {
    printf("$ ");

    for(i=0; (i<80) &&  (ch = getchar()) != '\n'; i++ )
      str[i] = (char)ch;
    str[i] = '\0';
    
    //strcpy(str, "mensagem\0");

    if ((send(s, (const char *)&str, sizeof(str),0)) < 0)
    {
      //printf("erro na transmissão - %d\n", WSAGetLastError());
      printf("erro na transmissão\n");
      close(s);
      return 0;
    }
    if(strcmp((const char *)&str, "q")==0)
      break;
     
     //usleep(100000);
  }

  // fecha socket e termina programa
  printf("Fim da conexao\n");
  close(s);
  return 0;
}

*/
