#include "c-sopa/include/Util/FIFO.h"
#include <stdio.h>
#include <stdlib.h>


int main(){
	FIFO fifo;

	FIFO_inicializar(&fifo, 10);

	char* primeiro="primeiro";
	char* segundo="segundo";
	char* terceiro="terceiro";

	FIFO_inserir(&fifo, &primeiro);

	printf("%s", (char*)FIFO_espiar(&fifo));

}



