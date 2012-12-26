#include "../include/DadosComuns.h"

/**
* Variáveis globais acessíveis somente neste arquivo.
*/



//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------

/**
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param	O sistema de arquivos que realizará a busca.
* @param DISCO				*disco_param			O disco em que a busca é feita.
* @return int	Posição do disco que está livre para receber um arquivo novo. 
* 				Retornará MEMORIA_ENDERECO_INEXISTENTE, caso não haja posição livre.
*/
int privada_getPosicaoLivreDisco(SISTEMA_ARQUIVOS *sistemaArquivos_param, DISCO *disco_param){
	int posicaoLivre = MEMORIA_ENDERECO_INEXISTENTE;
	//int arquivoChecado;

		tela_imprimirTelaAzulDaMorte(&global_tela, "privada_getPosicaoLivreDisco de SistemaArquivos nao foi implementada.");

	return posicaoLivre;
}

/**
* Lê o arquivo de inicialização, criando os arquivos que serão posteriormente preenchidos.
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param	O sistema de arquivos que será inicializado.
*/
void privada_lerArquivoInicializacao(SISTEMA_ARQUIVOS *sistemaArquivos_param){
	FIFO_inicializar(&sistemaArquivos_param->arquivos, MAXIMO_ARQUIVOS);
	
	ARQUIVO *segmentoLido = (ARQUIVO*) malloc(sizeof(ARQUIVO));
	DESCRITOR_ARQUIVO **arquivo = (DESCRITOR_ARQUIVO**) malloc(sizeof(DESCRITOR_ARQUIVO**));
	FILE *arquivoSistemaArquivos;
	int posicaoPalavra;
	char linha[200];
	char* byte;
	char* palavraBytes[2];

	arquivoSistemaArquivos = fopen(CAMINHO_ARQUIVO_DESCRITOR_SISTEMA_ARQUIVOS, "r");
	if(arquivoSistemaArquivos != NULL){
		while(fgets(linha, 200, arquivoSistemaArquivos)){
			byte=strtok(linha, " \n");
			posicaoPalavra = 0;
			do{
				if(byte != NULL){
					palavraBytes[posicaoPalavra] = byte;
					posicaoPalavra++;
				}
			}while(byte=strtok(NULL," \n"));

			arquivo = (DESCRITOR_ARQUIVO**) malloc(sizeof(DESCRITOR_ARQUIVO*));
			*arquivo = (DESCRITOR_ARQUIVO*) malloc(sizeof(DESCRITOR_ARQUIVO));
			arquivo_inicializar(segmentoLido, string_paraInt(palavraBytes[1]));
			descritorArquivo_inicializar(*arquivo, palavraBytes[0], sistemaArquivos_param->numeroDescritorArquivoLivre);
			descritorArquivo_adicionarSegmento(*arquivo, segmentoLido);
			sistemaArquivos_param->numeroDescritorArquivoLivre++;

			FIFO_inserir(&sistemaArquivos_param->arquivos, arquivo);
			segmentoLido = (ARQUIVO*) malloc(sizeof(ARQUIVO));
		}
		fclose(arquivoSistemaArquivos);
	} else {
		tela_escreverNaColuna(&global_tela, "Nao consegui abrir o arquivo que descreve o sistema de arquivos.", 4);
	}
	tela_escreverNaColuna(&global_tela, "Sistema de arquivos inicializado.", 4);
}

/**
* Atualiza o arquivo de inicialização com o conteúdo deste sistema de arquivos.
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param	O sistema de arquivos que será salvo.
*/
void privada_atualizarArquivoInicializacao(SISTEMA_ARQUIVOS *sistemaArquivos_param){
	FILE *arquivoSistemaArquivos;
	int arquivoImpresso=0;

	arquivoSistemaArquivos = fopen(CAMINHO_ARQUIVO_DESCRITOR_SISTEMA_ARQUIVOS, "w");
	if(arquivoSistemaArquivos != NULL){
		while(arquivoImpresso < FIFO_quantidadeElementos(&sistemaArquivos_param->arquivos)){
			fprintf(arquivoSistemaArquivos, "%s %d\n", 
				descritorArquivo_getNome(* (DESCRITOR_ARQUIVO**) FIFO_espiarPosicao(&sistemaArquivos_param->arquivos, arquivoImpresso)),
				descritorArquivo_tamanhoEmPalavras(* (DESCRITOR_ARQUIVO**) FIFO_espiarPosicao(&sistemaArquivos_param->arquivos, arquivoImpresso)));
			arquivoImpresso++;
		}
		fclose(arquivoSistemaArquivos);
	} else {
		tela_escreverNaColuna(&global_tela, "Nao consegui abrir o arquivo que descreve o sistema de arquivos.", 4);
	}
}

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
	sistemaArquivos_param->numeroDescritorArquivoLivre = 0;

	privada_lerArquivoInicializacao(sistemaArquivos_param);

	char caminhoArquivo[200];
	char mensagem[200];
	int conseguiuLer = 0;
	int posicaoLida=0;

	while(posicaoLida < FIFO_quantidadeElementos(&sistemaArquivos_param->arquivos)){
		memset(caminhoArquivo, '\0', 200);
		strcat(caminhoArquivo, DIRETORIO_DADOS_DISCO);
		strcat(caminhoArquivo, "/");
		strcat(caminhoArquivo, descritorArquivo_getNome(
			* (DESCRITOR_ARQUIVO**) FIFO_espiarPosicao(&sistemaArquivos_param->arquivos, posicaoLida)));

		sprintf(mensagem, "    ");
		tela_escreverNaColuna(&global_tela, mensagem, 4);
		conseguiuLer = arquivo_lerDaMaquinaHospedeira(
			descritorArquivo_getSegmento(
				* (DESCRITOR_ARQUIVO**) FIFO_espiarPosicao(&sistemaArquivos_param->arquivos, posicaoLida),
				0), 
			disco_param, caminhoArquivo);
		if(conseguiuLer){
			sprintf(mensagem, "Li '%s'", descritorArquivo_getNome(
				* (DESCRITOR_ARQUIVO**) FIFO_espiarPosicao(&sistemaArquivos_param->arquivos, posicaoLida)));
			tela_escreverNaColuna(&global_tela, mensagem, 4);
		} else {
			sprintf(mensagem, "Nao consegui ler '%s'", descritorArquivo_getNome(
				* (DESCRITOR_ARQUIVO**) FIFO_espiarPosicao(&sistemaArquivos_param->arquivos, posicaoLida)));
			tela_escreverNaColuna(&global_tela, mensagem, 4);
		}
		posicaoLida++;
	}
	tela_escreverNaColuna(&global_tela, "Sistema de arquivos inicializado.", 4);
}

/**
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param	O sistema de arquivos em que a operação será realizada.
* @param char*				nomeProcurado_param		O nome do arquivo que se quer.
* @return ARQUIVO*	O arquivo que tem o nome procurado. Caso não haja ou esteja fragmentado, retonará NULL.
*/
ARQUIVO* sistemaArquivos_buscaPorNome(SISTEMA_ARQUIVOS *sistemaArquivos_param, char* nomeProcurado_param){
	int totalArquivos = FIFO_quantidadeElementos(&sistemaArquivos_param->arquivos);
	int arquivoAtual;
	DESCRITOR_ARQUIVO* arquivo;
	DESCRITOR_ARQUIVO* arquivoEncontrado = NULL;
	ARQUIVO* arquivoFisicoEncontrado;

	for(arquivoAtual=0; arquivoAtual<totalArquivos; arquivoAtual++){
		arquivo = * (DESCRITOR_ARQUIVO**) FIFO_espiarPosicao(&sistemaArquivos_param->arquivos, arquivoAtual);
		if(strcmp(descritorArquivo_getNome(arquivo), nomeProcurado_param) == 0){
			arquivoEncontrado = arquivo;
		}
	}

	if(arquivoEncontrado != NULL && !descritorArquivo_estahFragmentado(arquivoEncontrado)){
		arquivoFisicoEncontrado = descritorArquivo_getSegmento(arquivoEncontrado, 0);
	} else {
		arquivoFisicoEncontrado = NULL;
	}

	return arquivoFisicoEncontrado;
}

/**
* Atualiza os arquivos na máquina hospedeira.
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param	O sistema de arquivos que será atualizado.
*/
void sistemaArquivos_atualizarNaMaquinaHospedeira(SISTEMA_ARQUIVOS *sistemaArquivos_param){
	privada_atualizarArquivoInicializacao(sistemaArquivos_param);

	char caminhoArquivo[200];
	int arquivoImpresso;
	for(arquivoImpresso=0; arquivoImpresso<FIFO_quantidadeElementos(&sistemaArquivos_param->arquivos); arquivoImpresso++){
		memset(caminhoArquivo, '\0', 200);
		strcat(caminhoArquivo, DIRETORIO_DADOS_DISCO);
		strcat(caminhoArquivo, "/");
		strcat(caminhoArquivo, descritorArquivo_getNome(* (DESCRITOR_ARQUIVO**) FIFO_espiarPosicao(&sistemaArquivos_param->arquivos, arquivoImpresso)));

		descritorArquivo_atualizarNaMaquinaHospedeira(
			* (DESCRITOR_ARQUIVO**) FIFO_espiarPosicao(&sistemaArquivos_param->arquivos, arquivoImpresso),
			 caminhoArquivo);
	}
}

/**
* Cria um arquivo novo.
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param	O sistema de arquivos no qual o arquivo será criado.
* @param char*				nome_param				Nome do arquivo, amigável ao usuário.
* @param DISCO				*disco_param			Disco em que o arquivo será salvo.
* @return int	Número descritor do arquivo (usado por processos do SOPA para operar sobre o arquivo).
*				Caso não tenha sido possível criar, retornará NUMERO_DESCRITOR_ARQUIVO_INEXISTENTE.
*/
int sistemaArquivos_criarArquivo(SISTEMA_ARQUIVOS *sistemaArquivos_param, char* nome_param, DISCO *disco_param){
	ARQUIVO *segmento = (ARQUIVO*) malloc(sizeof(ARQUIVO));
	DESCRITOR_ARQUIVO **arquivo = (DESCRITOR_ARQUIVO**) malloc(sizeof(DESCRITOR_ARQUIVO*));
	int posicaoLivreDisco = privada_getPosicaoLivreDisco(sistemaArquivos_param, disco_param);
	int descritorArquivoCriado = NUMERO_DESCRITOR_ARQUIVO_INEXISTENTE;

	if(posicaoLivreDisco != MEMORIA_ENDERECO_INEXISTENTE){
		*arquivo = (DESCRITOR_ARQUIVO*) malloc(sizeof(DESCRITOR_ARQUIVO));
		descritorArquivoCriado = sistemaArquivos_param->numeroDescritorArquivoLivre;
		arquivo_inicializar(segmento, posicaoLivreDisco);
		descritorArquivo_inicializar(*arquivo, nome_param, descritorArquivoCriado);
		descritorArquivo_adicionarSegmento(*arquivo, segmento);
		sistemaArquivos_param->numeroDescritorArquivoLivre++;
		FIFO_inserir(&sistemaArquivos_param->arquivos, arquivo);
	} else {
		free(arquivo);
		free(segmento);
	}

	return descritorArquivoCriado;
}









