#include "../include/Tela.h"
#include <stdio.h>


int main(void)
{
	inicializarTela();
	adicionarColuna("Primeira");
	adicionarColuna("Segunda");
	escreverNaColuna("aaaaaaa",1);
	escreverNaColuna("bbbbbbb",2);
	escreverNaColuna("ccccccc",2);
	escreverNaColuna("ddddddd",1);
	esperarCaractere();
	fecharTela();
	return 0;
}

