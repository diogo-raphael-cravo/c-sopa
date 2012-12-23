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
* Inicializa o sistema de arquivos e o disco com os arquivos do sistema hospedeiro.
* Cria a ilusão de persistência do disco.
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param	O sistema de arquivos que será inicializado.
* @param DISCO				*disco_param			O disco que será preenchido com os dados lidos.
*/
void sistemaArquivos_inicializarComArquivosDoHospedeiro(SISTEMA_ARQUIVOS *sistemaArquivos_param, DISCO *disco_param){
	FIFO_inicializar(&sistemaArquivos_param->arquivos, MAXIMO_ARQUIVOS);
	
	ARQUIVO **arquivoLido = (ARQUIVO**) malloc(sizeof(ARQUIVO*));
	DIR *d;
	struct dirent *dir;
	d = opendir(DIRETORIO_DADOS_DISCO);
	char caminhoArquivo[200];
	char mensagem[200];
	char nomeArquivo[200];
	int caractere;
	int palavraInicioDoArquivo = 0;
	int conseguiuLer = 0;

	if(d){
		while((dir = readdir(d)) != NULL){
			if( dir->d_name[0] == 'A'
				&& dir->d_name[1] == 'R'
				&& dir->d_name[2] == 'Q'
				&& dir->d_name[3] == 'U'
				&& dir->d_name[4] == 'I'
				&& dir->d_name[5] == 'V'
				&& dir->d_name[6] == 'O'
				&& dir->d_name[7] == '_'
				&& dir->d_name[strlen(dir->d_name)-1] != '~'){
					memset(caminhoArquivo, '\0', 200);
					strcat(caminhoArquivo, DIRETORIO_DADOS_DISCO);
					strcat(caminhoArquivo, "/");
					strcat(caminhoArquivo, dir->d_name);
					*arquivoLido = (ARQUIVO*) malloc(sizeof(ARQUIVO));

					memset(nomeArquivo, '\0', 200);
					for(caractere=0; caractere<strlen(dir->d_name)-8; caractere++){
						nomeArquivo[caractere] = dir->d_name[caractere+8];
					}
					sprintf(mensagem, "    ");
					tela_escreverNaColuna(&global_tela, mensagem, 4);
					conseguiuLer = arquivo_lerDaMaquinaHospedeira(*arquivoLido, disco_param, nomeArquivo, caminhoArquivo, palavraInicioDoArquivo);
					if(conseguiuLer){
						sprintf(mensagem, "Li '%s'", (*arquivoLido)->nome);
						tela_escreverNaColuna(&global_tela, mensagem, 4);
						FIFO_inserir(&sistemaArquivos_param->arquivos, arquivoLido);
						palavraInicioDoArquivo += arquivo_getTamanhoEmPalavras(*arquivoLido);
						arquivoLido = (ARQUIVO**) malloc(sizeof(ARQUIVO*));
					} else {
						sprintf(mensagem, "Nao consegui ler '%s'", dir->d_name);
						tela_escreverNaColuna(&global_tela, mensagem, 4);
					}
			}
		}
		closedir(d);
	} else {
		tela_escreverNaColuna(&global_tela, "Nao consegui abrir a pasta dos arquivos do disco.", 4);
	}
	tela_escreverNaColuna(&global_tela, "Sistema de arquivos inicializado.", 4);
}

/**
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param	O sistema de arquivos em que a operação será realizada.
* @param char*				nomeProcurado_param		O nome do arquivo que se quer.
* @return ARQUIVO*	O arquivo que tem o nome procurado. Caso não haja, retonará NULL.
*/
ARQUIVO* sistemaArquivos_buscaPorNome(SISTEMA_ARQUIVOS *sistemaArquivos_param, char* nomeProcurado_param){
	int totalArquivos = FIFO_quantidadeElementos(&sistemaArquivos_param->arquivos);
	int arquivoAtual;
	ARQUIVO* arquivo;
	ARQUIVO* arquivoEncontrado = NULL;
	FIFO copiaFila;
	FIFO_inicializar(&copiaFila, totalArquivos);

	for(arquivoAtual=0; arquivoAtual<totalArquivos; arquivoAtual++){
		arquivo = * (ARQUIVO**) FIFO_espiar(&sistemaArquivos_param->arquivos);
		FIFO_inserir(&copiaFila, FIFO_remover(&sistemaArquivos_param->arquivos));
		if(strcmp(arquivo_getNome(arquivo), nomeProcurado_param) == 0){
			arquivoEncontrado = arquivo;
		}
	}

	FIFO_copiar(&sistemaArquivos_param->arquivos, &copiaFila);
	FIFO_destruir(&copiaFila);
	return arquivoEncontrado;
}








