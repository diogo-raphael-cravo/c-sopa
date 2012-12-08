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
/**
* Inicia thread do disco.
* @param DISCO	*disco_param	O disco que irá 'rodar'.
*/
void disco_rodar(DISCO *disco_param){
	sem_init(&disco_param->mutexAcessoDisco, 0, 0);
	disco_carregar(disco_param, CAMINHO_ARQUIVO_BASE_DISCO);

	while(1){
		sem_wait(&disco_param->mutexAcessoDisco);
		controladorInterrupcoes_set(&global_controladorInterrupcoes, INTERRUPCAO_DISCO);
		tela_escreverNaColuna(&global_tela, "Mensagem do DISCO.",4);
	}
}

/**
* Libera o disco para dar uma volta.
* @param DISCO	*disco_param	O disco em que a operação será realizada.
*/
void disco_darUmaVolta(DISCO *disco_param){
	sem_post(&disco_param->mutexAcessoDisco);
}

/**
* Carrega os dados do arquivo, juntando-os aos dados que estão no disco.
* Os dados são colocados à partir da primeira posição livre neste disco.
* @param DISCO	*disco_param			O disco em que a operação será realizada.
* @param char*	*caminhoArquivo_param	Arquivo que contém os dados, formatados da seguinte forma:
*										J P A 0
*										255 255 255 255
*										0 0 0 0
*/
void disco_carregar(DISCO *disco_param, char *caminhoArquivo_param){
	char linha[200];
	char* byte;
	char mensagem[200];
	FILE *arquivoLido;
	arquivoLido = fopen(caminhoArquivo_param, "r");
	if(arquivoLido == NULL){
		sprintf(mensagem, "Nao consegui carregar o arquivo '%s'.", caminhoArquivo_param);
		tela_escreverNaColuna(&global_tela, mensagem, 4);
	} else {
		while(fgets(linha, 200, arquivoLido)){
			byte=strtok(linha, " \n");
			do{
				if(byte != NULL){
					sprintf(mensagem, "Acabei de ler '%s'.", byte);
					tela_escreverNaColuna(&global_tela, mensagem, 4);
				}
			}while(byte=strtok(NULL," \n"));
		}
	}
}
