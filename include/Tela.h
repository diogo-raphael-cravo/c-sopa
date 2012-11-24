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
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Número máximo de colunas.
#define MAXIMO_COLUNAS 5


//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* Inicializa a tela.
*/
void tela_inicializar(void);

/**
* Termina o uso da tela.
*/
void tela_fechar(void);

/**
* Espera que o usuário digite uma linha até [ENTER].
* @return int	A linha que o usuário digitou.
*/
char* tela_esperarLinhaUsuario(void);

/**
* Espera input do usuário e digita na tela.
*/
void tela_run(void);

/**
* Adiciona uma coluna a esta tela com o nome dado.
* @param char*	nomeColuna_param	O nome que ficará no topo da coluna.
*/
void tela_adicionarColuna(char* nomeColuna_param);

/**
* Escreve o texto na linha dada.
* @param char*	texto_param	O texto que será escrito. Como há um espaço limitado para cada linha,
*				o texto pode ser enrolado.
* @param int	coluna_param	Coluna em que o texto será escrito. Começando em 1.
*/
void tela_escreverNaColuna(char* texto_param, int coluna_param);
