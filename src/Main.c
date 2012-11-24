#include "../include/Tela.h"
#include <stdio.h>


int main(void)
{
	inicializarTela();
	adicionarColuna("PRIMEIRA COLUNA");
	adicionarColuna("SEGUNDA COLUNA");
	adicionarColuna("TERCEIRA COLUNA");
	adicionarColuna("QUARTA COLUNA");
	adicionarColuna("QUINTA COLUNA");
	escreverNaColuna("Estah eh a mensagem de coluna um e ela eh muito grande. Tao grande que vai ter que ser enrolada em novas linhas.",1);
	escreverNaColuna("Estah eh a mensagem de coluna um e ela eh muito grande. Tao grande que vai ter que ser enrolada em novas linhas.",2);
	escreverNaColuna("ccccccc",2);
	escreverNaColuna("ddddddd",1);
	escreverNaColuna("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee",3);
	escreverNaColuna("quartaquartaquartaquartaquartaquartaquartaquartaquartaquarta",4);
	runTela();
	fecharTela();
	return 0;
}

