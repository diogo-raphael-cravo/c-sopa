#include "../include/Tela.h"
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

/**
* Variáveis globais acessíveis somente neste arquivo.
*/
//A quantidade de colunas desta tela. 
int quantidadeColunas;
//A última linha em que houve escrita.
int ultimaLinhaEscrita;
//A quantidade total de linhas em que se pode escrever. Medida em caracteres.
#define CARACTERES_POR_LINHA_PARA_ESCRITA 50
//A quantidade total de colunas em que se pode escrever. Medida em caracteres.
#define CARACTERES_POR_COLUNA_PARA_ESCRITA 50
//Os nomes das colunas da tela.
char** nomesColunas;


//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------
/**
* Move o cursor para o início da linha e coluna. Enviar 0,0 mandará para a linha de entrada.
* @param int	linha_param	A linha, da tela, não de caracteres, em que o cursor irá ficar.
* @param int	coluna_param	A coluna, da tela, não de caracteres, em que o cursor irá ficar.
*/
void moverCursor(int linha_param, int coluna_param){
	move(linha_param,coluna_param*CARACTERES_POR_COLUNA_PARA_ESCRITA/MAXIMO_COLUNAS);	
}


//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/**
* Inicializa a tela.
*/
void inicializarTela(void){
	initscr();   /*Esta função  inicializa a ncurses. Para todos os programas  
                   devemos sempre inicializar a ncurses e depois finalizar, como 
                  veremos adiante. */

	start_color(); //Esta função torna possível o uso das cores

	//Abaixo estamos definindo os pares de cores que serão utilizados no programa
	//init_pair(1,COLOR_WHITE,COLOR_BLUE); //Texto(Branco) | Fundo(Azul)
	//init_pair(2,COLOR_BLUE,COLOR_WHITE); //Texto(Azul) | Fundo(Branco)
	//init_pair(3,COLOR_RED,COLOR_WHITE);  //Texto(Vermelho) | Fundo(Branco)

	/*bkgd(COLOR_PAIR(1));*/  /*Aqui nós definiremos que a cor de fundo do nosso 
                                      programa será azul e a cor dos textos será branca.*/

	/*attron(COLOR_PAIR(3));*/ /*Estamos alterando o par de cores para 3 em vez 
                                          de utilizar o par de cor por omissão(1).*/

	//move(2,1);  //Aqui estamos movendo o cursor para a linha 2 coluna 1.
	//printw("Olá mundo!!!");  //Imprimimos um texto na tela na posição acima.
	/*attroff(COLOR_PAIR(3));*/  /*Estamos alterando o par com a cor por omissão,
                             ou seja, retornando para o par de cor 1. */
	//attron(COLOR_PAIR(2));  
	//move(3,1);
	/*printw("Qualquer tecla para sair.");*/ /*Imprimimos um texto na tela na
                                                        posição acima. */
	//attroff(COLOR_PAIR(2));
	//refresh();    //Atualiza a tela
	ultimaLinhaEscrita = 2;
	quantidadeColunas = 0;
	nomesColunas = (char**) malloc(MAXIMO_COLUNAS * sizeof(char**));
}

/**
* Termina o uso da tela.
*/
void fecharTela(void){
	endwin(); /*Sempre que finalizarmos um programa com a biblioteca curses, 
                     devemos executar este comando.*/
}

/**
* Espera que o usuário digita um caractere e o retorna.
*/
char esperarCaractere(void){
	return getch();      //Fica esperando que o usuário aperte alguma tecla.
}

/**
* Adiciona uma coluna a esta tela com o nome dado.
* @param char*	nomeColuna_param	O nome que ficará no topo da coluna.
*/
void adicionarColuna(char* nomeColuna_param){
	quantidadeColunas++;
	nomesColunas[quantidadeColunas-1] = nomeColuna_param;
	moverCursor(2,quantidadeColunas);
	printw(nomeColuna_param);
}

/**
* Escreve o texto na linha dada.
* @param char*	texto_param	O texto que será escrito. Como há um espaço limitado para cada linha,
*				o texto pode ser enrolado.
* @param int	coluna_param	Coluna em que o texto será escrito. Começando em 1.
*/
void escreverNaColuna(char* texto_param, int coluna_param){
	ultimaLinhaEscrita++;
	moverCursor(ultimaLinhaEscrita,coluna_param);			
	printw(texto_param);
}







