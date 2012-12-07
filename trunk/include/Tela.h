/**
* Concentra as funções de manipulação de tela.
* Todas as colunas têm o mesmo tamanho.
* Exemplo de tela com 4 colunas:
>
NOME COLUNA 1 | NOME COLUNA 2 | NOME COLUNA 3 | NOME COLUNA 4
blablablah    |               |               |
              | blahblahbblAHB|               | 
              |ABAKBLA        |               |
* Notar que a primeira linha é a linha de comando, em que o usuário pode digitar.
* ====================================
*
* 			ATENÇÃO: Não confundir a tela com o console!
*
* ====================================
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Número máximo de colunas.
#define MAXIMO_COLUNAS 5
//Número máximo de linhas salvas.
#define MAXIMO_LINHAS_SALVAS 10000

struct str_tela{
	int quantidadeColunas; 	//A quantidade de colunas desta tela. 
	int ultimaLinhaEscrita; //A última linha em que houve escrita.
	char** nomesColunas; //Os nomes das colunas da tela.
	char* textosColunas[MAXIMO_LINHAS_SALVAS]; //Ordenação de todos os textos que já foram escritos
		// na tela com tela_escreverNaColuna, ordenados linha a linha.
	int colunasTextosColunas[MAXIMO_LINHAS_SALVAS]; //Colunas em que foram escritos os textos em textosColunas.
	int totalTextosColunas; //O número de textos em textosColunas.
	int ultimoTextoColunaExibido; //Índice em textosColunas do primeiro texto que está sendo exibido.
	int abertaParaImpressoes; //Indica se elementos podem imprimir na tela.
};

typedef struct str_tela TELA;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* Inicializa a tela.
* @param TELA	*tela_param A tela em que a operação será realizada.
*/
void tela_inicializar(TELA *tela_param);

/**
* Termina o uso da tela.
* @param TELA	*tela_param A tela em que a operação será realizada.
*/
void tela_fechar(TELA *tela_param);

/**
* Espera que o usuário digite uma linha até [ENTER].
* @param TELA	*tela_param A tela em que a operação será realizada.
* @return int	A linha que o usuário digitou.
*/
char* tela_esperarLinhaUsuario(TELA *tela_param);

/**
* Espera input do usuário e digita na tela.
* @param TELA	*tela_param A tela em que a operação será realizada.
*/
void tela_rodar(TELA *tela_param);

/**
* Espera que o usuário digite uma linha até [ENTER].
* @param TELA	*tela_param A tela em que a operação será realizada.
*/
char* tela_esperarLinhaUsuario(TELA *tela_param);

/**
* Adiciona uma coluna a esta tela com o nome dado.
* @param TELA	*tela_param A tela em que a operação será realizada.
* @param char*	nomeColuna_param	O nome que ficará no topo da coluna.
*/
void tela_adicionarColuna(TELA *tela_param, char* nomeColuna_param);

/**
* Escreve o texto na linha dada.
* @param TELA	*tela_param A tela em que a operação será realizada.
* @param char*	texto_param	O texto que será escrito. Como há um espaço limitado para cada linha,
*				o texto pode ser enrolado.
* @param int	coluna_param	Coluna em que o texto será escrito. Começando em 1.
* Obs.: Para maior liberdade, utilize com sprintf (buffer, "%d plus %d is %d", a, b, a+b).
*/
void tela_escreverNaColuna(TELA *tela_param, char* texto_param, int coluna_param);

/**
* Joga a tela para cima ou para baixo, mostrando textos escondidos.
* @param TELA	*tela_param 			A tela em que a operação será realizada.
* @param int	quantidadeLinhas_param 	A quantidade de linhas que se deseja ler.
*										0 < quantidadeLinhas_param esconderá textos acima e mostrará textos abaixo.
*										quantidadeLinhas_param < 0 esconderá textos abaixo e mostrará textos acimo.
*										quantidadeLinhas_param == 0 não fará nada.
*/
void tela_rolar(TELA *tela_param, int quantidadeLinhas_param);

/**
* Muda para a tela azul da morte!
* Depois de imprimir, espera um caractere do usuário. Quando ele digitar um caractere, restaura a tela 
* ao seu estado anterior à impressão da tela azul.
* @param TELA	*tela_param 			A tela em que a operação será realizada.
* @param char	*descricaoErro_param	A descrição do erro que será exibida.
************************************************************************************
* OBS.: Esta função poderia ser fatorada, MAS é um easter egg e deve ficar restrita.
************************************************************************************
*/
void tela_imprimirTelaAzulDaMorte(TELA *tela_param, char *descricaoErro_param);
