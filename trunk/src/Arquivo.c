#include "../include/DadosComuns.h"

/**
* Variáveis globais acessíveis somente neste arquivo.
*/



//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------
/**
* @param ARQUIVO	*arquivo_param				Arquivo em que a operação será realizada.
* @param char*		nome_param					Nome que o arquivo terá.
* ATENÇÃO: deve ser usada uma só vez!
*/
void privada_setNome(ARQUIVO *arquivo_param, char* nome_param){
	arquivo_param->nome = (char*) malloc((strlen(nome_param)+1)*sizeof(char));
	strcpy(arquivo_param->nome, nome_param);
}

//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/**
* @param ARQUIVO	*arquivo_param			O arquivo que será inicializado.
* @param char*		nome_param				Nome do arquivo, amigável ao usuário.
* @param int		enderecoInicio_param	Endereço do disco que iniciará o arquivo.
*/
void arquivo_inicializar(ARQUIVO *arquivo_param, char* nome_param, int enderecoInicio_param){
	privada_setNome(arquivo_param, nome_param);
	arquivo_param->enderecoInicioDisco = enderecoInicio_param;
}

/**
* Cria um arquivo à partir de um arquivo existente e salvo na máquina hospedeira.
* O novo arquivo é salvo no disco.
* @param ARQUIVO	*arquivo_param				Arquivo que será criado.
* @param DISCO		*disco_param				O disco em que o arquivo será salvo.
* @param char*		caminho_param				Caminho do arquivo na máquina hospedeira.
* @return int	Indica se foi possível ler o arquivo.
*/
int arquivo_lerDaMaquinaHospedeira(ARQUIVO *arquivo_param, DISCO *disco_param, char* caminho_param){
	int conseguiuLer = 0;
	char mensagem[200];

	arquivo_param->processoQueAbriu = NULL;

	int posicaoPalavra = 0;
	int palavra[TAMANHO_INSTRUCAO_PALAVRAS];
	int posicaoEscrita=0;
	char linha[200];
	char* byte;
	char* palavraBytes[TAMANHO_INSTRUCAO_PALAVRAS];
	FILE *arquivoLido = fopen(caminho_param, "r");
	if(arquivoLido != NULL){
		conseguiuLer = 1;
		
		while(fgets(linha, 200, arquivoLido)){
			byte=strtok(linha, " \n");
			posicaoPalavra = 0;
			do{
				if(byte != NULL){
					palavraBytes[posicaoPalavra] = byte;
					posicaoPalavra++;
				}
			}while(byte=strtok(NULL," \n"));
			palavra[0] = string_paraInt(palavraBytes[0]);
			palavra[1] = string_paraInt(palavraBytes[1]);
			palavra[2] = string_paraInt(palavraBytes[2]);
			palavra[3] = string_paraInt(palavraBytes[3]);
			disco_inicializarPosicao(disco_param, arquivo_param->enderecoInicioDisco+posicaoEscrita, 
				palavra[0], palavra[1], palavra[2], palavra[3]);

			sprintf(mensagem, "File *(%d)=%s %s %s %s", posicaoEscrita, palavraBytes[0], palavraBytes[1], palavraBytes[2], palavraBytes[3]);
			tela_escreverNaColuna(&global_tela, mensagem, 4);
			sprintf(mensagem, "=%d %d %d %d", palavra[0], palavra[1], palavra[2], palavra[3]);
			tela_escreverNaColuna(&global_tela, mensagem, 4);
			sprintf(mensagem, "Disco *(%d)=%d", arquivo_param->enderecoInicioDisco+posicaoEscrita, 
				disco_param->conteudo[arquivo_param->enderecoInicioDisco+posicaoEscrita]);
			tela_escreverNaColuna(&global_tela, mensagem, 4);
			sprintf(mensagem, "=%d %d %d %d", 
				(((disco_param->conteudo[arquivo_param->enderecoInicioDisco+posicaoEscrita] & 0xFF000000)/256)/256)/256,
				((disco_param->conteudo[arquivo_param->enderecoInicioDisco+posicaoEscrita] & 0x00FF0000)/256)/256,
				(disco_param->conteudo[arquivo_param->enderecoInicioDisco+posicaoEscrita] & 0x0000FF00)/256,
				disco_param->conteudo[arquivo_param->enderecoInicioDisco+posicaoEscrita] & 0x000000FF);
			tela_escreverNaColuna(&global_tela, mensagem, 4);

			posicaoEscrita++;
		}
		
		arquivo_param->tamanhoEmPalavras = posicaoEscrita;
		arquivo_param->discoSalvo = disco_param;
	} else {
		sprintf(mensagem, "Nao consegui abrir o arquivo %s.", arquivo_param->nome);
		tela_escreverNaColuna(&global_tela, mensagem, 4);
	}

	return conseguiuLer;
}

/**
* @param ARQUIVO	*arquivo_param	Arquivo que será atualizado na máquina hospedeira.
* @param char*		caminho_param				Caminho do arquivo na máquina hospedeira.
*/
void arquivo_atualizarNaMaquinaHospedeira(ARQUIVO *arquivo_param, char* caminho_param){
	FILE *arquivoMaquinaHospedeira;
	char mensagem[200];
	PALAVRA palavra;
	BYTE palavraEmBytes[TAMANHO_INSTRUCAO_PALAVRAS];
	int palavraImpressa=0;

	arquivoMaquinaHospedeira = fopen(caminho_param, "w");
	if(arquivoMaquinaHospedeira != NULL){
		for(; palavraImpressa<arquivo_param->tamanhoEmPalavras; palavraImpressa++){
			palavra = arquivo_param->discoSalvo->conteudo[arquivo_param->enderecoInicioDisco+palavraImpressa];
			palavraEmBytes[0] = (((palavra & 0xFF000000)/256)/256)/256;
			palavraEmBytes[1] = ((palavra & 0x00FF0000)/256)/256;
			palavraEmBytes[2] = (palavra & 0x0000FF00)/256;
			palavraEmBytes[3] = palavra & 0x000000FF;

			fprintf(arquivoMaquinaHospedeira, "%d %d %d %d\n", palavraEmBytes[0], palavraEmBytes[1],
				palavraEmBytes[2], palavraEmBytes[3]);
		}
		fclose(arquivoMaquinaHospedeira);
	} else {
		sprintf(mensagem, "Nao consegui salvar o arquivo %s.", arquivo_param->nome);
		tela_escreverNaColuna(&global_tela, mensagem, 4);
	}
}

/**
* @param ARQUIVO	*arquivo_param	O arquivo cuja informação será retornada.
* @return int	Nome amigável ao usuário, atribuído ao arquivo.
*/
char* arquivo_getNome(ARQUIVO *arquivo_param){
	return arquivo_param->nome;
}

/**
* @param ARQUIVO	*arquivo_param	O arquivo cuja informação será retornada.
* @return int	O tamanho do arquivo em palavras. Retornará -1 caso o arquivo não esteja salvo nem no disco, nem na memória.
*/
int arquivo_getTamanhoEmPalavras(ARQUIVO *arquivo_param){
	return arquivo_param->tamanhoEmPalavras;
}

/**
* @param ARQUIVO	*arquivo_param	O arquivo cuja informação será retornada.
* @return int	Indica se o arquivo pode ser aberto.
*/
int arquivo_podeSerAberto(ARQUIVO *arquivo_param){
	return (arquivo_param->processoQueAbriu == NULL);
}

/**
* @param ARQUIVO	*arquivo_param	O arquivo cuja informação será retornada.
* @return DESCRITOR_PROCESSO*	O processo que abriu este arquivo. Caso o arquivo não tenha sido aberto, retornará NULL.
*/
DESCRITOR_PROCESSO* arquivo_getProcessoQueAbriu(ARQUIVO *arquivo_param){
	return arquivo_param->processoQueAbriu;
}

/**
* @param ARQUIVO				*arquivo_param		O arquivo cuja informação será retornada.
* @param DESCRITOR_PROCESSO		*processo_param		O processo que abrirá o arquivo.
* ATENÇÃO: não checa se o arquivo já está aberto!
*/
void arquivo_abrirParaProcesso(ARQUIVO *arquivo_param, DESCRITOR_PROCESSO *processo_param){
	arquivo_param->processoQueAbriu = processo_param;
}







