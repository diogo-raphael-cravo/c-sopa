#include "../include/Tela.h"
#include <stdio.h>


int main(void)
{
	tela_inicializar();
	tela_adicionarColuna("PRIMEIRA COLUNA");
	tela_adicionarColuna("SEGUNDA COLUNA");
	tela_adicionarColuna("TERCEIRA COLUNA");
	tela_adicionarColuna("QUARTA COLUNA");
	tela_adicionarColuna("QUINTA COLUNA");
	tela_escreverNaColuna("Estah eh a mensagem de coluna um e ela eh muito grande. Tao grande que vai ter que ser enrolada em novas linhas.",1);
	tela_escreverNaColuna("Estah eh a mensagem de coluna um e ela eh muito grande. Tao grande que vai ter que ser enrolada em novas linhas.",2);
	tela_escreverNaColuna("ccccccc",2);
	tela_escreverNaColuna("ddddddd",1);
	tela_escreverNaColuna("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee",3);
	tela_escreverNaColuna("quartaquartaquartaquartaquartaquartaquartaquartaquartaquarta",4);
	tela_run();
	tela_fechar();
	return 0;
}

