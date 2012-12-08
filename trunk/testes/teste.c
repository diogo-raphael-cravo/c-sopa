#include "../include/Util/FIFO.h"
#include <stdio.h>
#include <stdlib.h>


int main(){
	FIFO fifo;

	FIFO_inicializar(&fifo, 10);

	char* primeiro="primeiro";
	char* segundo="segundo";
	char* terceiro="terceiro";

	FIFO_inserir(&fifo, &primeiro);
	FIFO_inserir(&fifo, &segundo);
	FIFO_inserir(&fifo, &terceiro);

	printf("%s\n", * (char**) (FIFO_remover(&fifo)));
	printf("%s\n", * (char**) (FIFO_remover(&fifo)));
	printf("%s\n", * (char**) (FIFO_remover(&fifo)));

}



