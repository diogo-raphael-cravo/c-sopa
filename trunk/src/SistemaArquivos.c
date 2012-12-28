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
* @return int	Posição do disco que está livre para receber um arquivo novo (com tamanho TAMANHO_ARQUIVO_RECEM_CRIADO). 
* 				Retornará MEMORIA_ENDERECO_INEXISTENTE, caso não haja posição livre.
*/
int privada_getPosicaoLivreDisco(SISTEMA_ARQUIVOS *sistemaArquivos_param, DISCO *disco_param){
	int posicaoLivre = MEMORIA_ENDERECO_INEXISTENTE;
	int numeroArquivos = FIFO_quantidadeElementos(&sistemaArquivos_param->arquivos);

	int POSICAO=0;
	int TAMANHO=1;
	int segmentos[numeroArquivos*MAXIMO_SEGMENTOS_DESCRITOR_ARQUIVO][2];

	int arquivoAtual=0;
	int segmentoAtual;
	int quantidadeSegmentos;
	int indicePreenchido=0;

	DESCRITOR_ARQUIVO* arquivo;
	for(; arquivoAtual<numeroArquivos; arquivoAtual++){
		arquivo = * (DESCRITOR_ARQUIVO**) FIFO_espiarPosicao(&sistemaArquivos_param->arquivos, arquivoAtual);
		quantidadeSegmentos = descritorArquivo_getQuantidadeSegmentos(arquivo);
		for(segmentoAtual=0; segmentoAtual<quantidadeSegmentos; segmentoAtual++){
			segmentos[indicePreenchido][POSICAO] = arquivo_getEnderecoInicial(descritorArquivo_getSegmento(arquivo, segmentoAtual));
			segmentos[indicePreenchido][TAMANHO] = arquivo_getTamanhoEmPalavras(descritorArquivo_getSegmento(arquivo, segmentoAtual));
			indicePreenchido++;
		}
	}

	//Bubblesort
	int totalIndicesPreenchidos = indicePreenchido;
	int indiceOrdenado=0;
	int indiceAtual;
	int segmentoAuxiliar[2];
	for(; indiceOrdenado<totalIndicesPreenchidos; indiceOrdenado++){ 
		for(indiceAtual=0; indiceAtual<totalIndicesPreenchidos-1; indiceAtual++){
			if(segmentos[indiceAtual+1][POSICAO] < segmentos[indiceAtual][POSICAO]){
				segmentoAuxiliar[POSICAO] = segmentos[indiceAtual][POSICAO];
				segmentoAuxiliar[TAMANHO] = segmentos[indiceAtual][TAMANHO];
				segmentos[indiceAtual][POSICAO] = segmentos[indiceAtual+1][POSICAO];
				segmentos[indiceAtual][TAMANHO] = segmentos[indiceAtual+1][TAMANHO];
				segmentos[indiceAtual+1][POSICAO] = segmentoAuxiliar[POSICAO];
				segmentos[indiceAtual+1][TAMANHO] = segmentoAuxiliar[TAMANHO];
			}
		}
	}

	//Procura pelo primeiro intervalo em que caiba.
	int intervalo=0;
	int tamanhoIntervalo=0;
	int encontrou=0;
	while(intervalo<totalIndicesPreenchidos-1 && !encontrou){
		tamanhoIntervalo = segmentos[intervalo+1][POSICAO] - segmentos[intervalo][POSICAO]+segmentos[intervalo][TAMANHO];
		if(TAMANHO_ARQUIVO_RECEM_CRIADO <= tamanhoIntervalo){
			encontrou = 1;
			posicaoLivre = segmentos[intervalo][POSICAO]+segmentos[intervalo][TAMANHO];
		}
		intervalo++;
	}

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

/**
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param	O sistema de arquivos em que a operação será realizada.
* @param char*				nomeProcurado_param		O nome do arquivo que se quer.
* @return DESCRITOR_ARQUIVO*	O arquivo que tem o nome procurado.
*/
DESCRITOR_ARQUIVO* privada_buscaPorNome(SISTEMA_ARQUIVOS *sistemaArquivos_param, char* nomeProcurado_param){
	int totalArquivos = FIFO_quantidadeElementos(&sistemaArquivos_param->arquivos);
	int arquivoAtual;
	DESCRITOR_ARQUIVO* arquivo;
	DESCRITOR_ARQUIVO* arquivoEncontrado = NULL;

	for(arquivoAtual=0; arquivoAtual<totalArquivos; arquivoAtual++){
		arquivo = * (DESCRITOR_ARQUIVO**) FIFO_espiarPosicao(&sistemaArquivos_param->arquivos, arquivoAtual);
		if(strcmp(descritorArquivo_getNome(arquivo), nomeProcurado_param) == 0){
			arquivoEncontrado = arquivo;
		}
	}

	return arquivoEncontrado;
}

/**
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param	O sistema de arquivos em que a operação será realizada.
* @param int				numeroDescritor_param	Número descritor que o arquivo deve ter.
* @return DESCRITOR_ARQUIVO*	O arquivo que tem o número descritor procurado.
*/
DESCRITOR_ARQUIVO* privada_buscaPorNumeroDescritor(SISTEMA_ARQUIVOS *sistemaArquivos_param, int numeroDescritor_param){
	int totalArquivos = FIFO_quantidadeElementos(&sistemaArquivos_param->arquivos);
	int arquivoAtual;
	DESCRITOR_ARQUIVO* arquivo;
	DESCRITOR_ARQUIVO* arquivoEncontrado = NULL;

	for(arquivoAtual=0; arquivoAtual<totalArquivos; arquivoAtual++){
		arquivo = * (DESCRITOR_ARQUIVO**) FIFO_espiarPosicao(&sistemaArquivos_param->arquivos, arquivoAtual);
		if(descritorArquivo_getNumeroDescritor(arquivo) == numeroDescritor_param){
			arquivoEncontrado = arquivo;
		}
	}

	return arquivoEncontrado;
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
	DESCRITOR_ARQUIVO* arquivoEncontrado = privada_buscaPorNome(sistemaArquivos_param, nomeProcurado_param);
	ARQUIVO* arquivoFisicoEncontrado = NULL;
	if(arquivoEncontrado != NULL && !descritorArquivo_estahFragmentado(arquivoEncontrado)){
		arquivoFisicoEncontrado = descritorArquivo_getSegmento(arquivoEncontrado, 0);
	}
	return arquivoFisicoEncontrado;
}

/**
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param	O sistema de arquivos em que a operação será realizada.
* @param int				numeroDescritor_param	Número descritor do arquivo que se quer.
* @return DESCRITOR_ARQUIVO*	O arquivo que tem o nome procurado. Caso não haja, retonará NULL.
*/
DESCRITOR_ARQUIVO* sistemaArquivos_buscaPorNumeroDescritor(SISTEMA_ARQUIVOS *sistemaArquivos_param, int numeroDescritor_param){
	return privada_buscaPorNumeroDescritor(sistemaArquivos_param, numeroDescritor_param);
}

/**
* @param SISTEMA_ARQUIVOS			*sistemaArquivos_param	O sistema de arquivos em que a operação será realizada.
* @param char*						nomeProcurado_param		O nome do arquivo que se quer.
* @param DESCRITOR_PROCESSO			*processo_param			O processo que terá posse do arquivo aberto.
* @param OPCAO_ABERTURA_ARQUIVO		opcao_param				O que será feito com o arquivo.
* @return int	Número descritor do arquivo (usado por processos do SOPA para operar sobre o arquivo).
*				Caso não tenha sido possível encontrar/abrir, retornará NUMERO_DESCRITOR_ARQUIVO_INEXISTENTE.
*/
int sistemaArquivos_abrirArquivoExistentePara(SISTEMA_ARQUIVOS *sistemaArquivos_param, char* nomeProcurado_param,
		DESCRITOR_PROCESSO *processo_param, OPCAO_ABERTURA_ARQUIVO opcao_param){
	DESCRITOR_ARQUIVO* arquivoEncontrado = privada_buscaPorNome(sistemaArquivos_param, nomeProcurado_param);
	int descritor = NUMERO_DESCRITOR_ARQUIVO_INEXISTENTE;
	if(arquivoEncontrado != NULL && descritorArquivo_getProcessoQueAbriu(arquivoEncontrado) == NULL){
		descritor = descritorArquivo_getNumeroDescritor(arquivoEncontrado);
		descritorArquivo_abrirParaProcesso(arquivoEncontrado, processo_param, opcao_param);
	}
	return descritor;
}

/**
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param	O sistema de arquivos em que a operação será realizada.
* @param int				numeroDescritor_param	Número descritor do arquivo que será fechado.
*/
void sistemaArquivos_fecharArquivo(SISTEMA_ARQUIVOS *sistemaArquivos_param, int numeroDescritor_param){
	DESCRITOR_ARQUIVO* arquivoEncontrado = privada_buscaPorNumeroDescritor(sistemaArquivos_param, numeroDescritor_param);
	if(arquivoEncontrado != NULL){
		descritorArquivo_fechar(arquivoEncontrado);
	}
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

/**
* Fecha os arquivos que ainda estão abertos para o processo passado.
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param		O sistema de arquivos em que a operação será realizada.
* @param DESCRITOR_PROCESSO	*descritorProcesso_param	O processo cujos arquivos abertos serão fechados.
*/
void sistemaArquivos_fecharArquivosAbertosPara(SISTEMA_ARQUIVOS *sistemaArquivos_param, DESCRITOR_PROCESSO *descritorProcesso_param){
	int totalArquivos = FIFO_quantidadeElementos(&sistemaArquivos_param->arquivos);
	int arquivoAtual;
	DESCRITOR_ARQUIVO* arquivo;
	for(arquivoAtual=0; arquivoAtual<totalArquivos; arquivoAtual++){
		arquivo = * (DESCRITOR_ARQUIVO**) FIFO_espiarPosicao(&sistemaArquivos_param->arquivos, arquivoAtual);
		if(descritorArquivo_getProcessoQueAbriu(arquivo) != NULL
			&& descritorProcesso_getPID(descritorArquivo_getProcessoQueAbriu(arquivo)) == descritorProcesso_getPID(descritorProcesso_param)){
			descritorArquivo_fechar(arquivo);
		}
	}
}

/**
* @param SISTEMA_ARQUIVOS	*sistemaArquivos_param			O sistema de arquivos em que a operação será realizada.
* @param DESCRITOR_PROCESSO	*descritorProcesso_param		O processo cuja possa do arquivo será testada.
* @param int				numeroDescritorArquivo_param	O arquivo de cuja posse será testada.
* @return int	Indica se o arquivo está aberto para o processo.
* ATENÇÃO: retornará false (0) se o arquivo não existir.
*/
int sistemaArquivos_arquivoEstahAbertoPara(SISTEMA_ARQUIVOS *sistemaArquivos_param, DESCRITOR_PROCESSO *descritorProcesso_param,
		int numeroDescritorArquivo_param){
	
	DESCRITOR_ARQUIVO *arquivo = privada_buscaPorNumeroDescritor(sistemaArquivos_param, numeroDescritorArquivo_param);
	int estahAbertoParaProcessoDado = 0;

	if(arquivo != NULL
		&& descritorArquivo_getProcessoQueAbriu(arquivo) != NULL
		&& descritorProcesso_getPID(descritorArquivo_getProcessoQueAbriu(arquivo)) == descritorProcesso_getPID(descritorProcesso_param)){
		estahAbertoParaProcessoDado = 1;
	}

	return estahAbertoParaProcessoDado;
}






