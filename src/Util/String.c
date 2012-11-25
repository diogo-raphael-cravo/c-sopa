#include "../../include/Util/String.h"
#include <stdio.h>
#include <stdlib.h>

/**
* Variáveis globais acessíveis somente neste arquivo.
*/

//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------




//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/**
* @param char*	texto_param				Um texto terminado por '\0'.
* @param int	tamanhoSubtexto_param	Tamanho de cada subtexto.
* @param int	ordemSubtexto_param		Qual subtexto será retornado.
* @return char*	O subtexto de tamanho tamanhoSubtexto_param na ordem ordemSubtexto_param.
* Obs.: Adaptado de http://www.programmingsimplified.com/c/source-code/c-substring
*/
char* string_pegarSubtextoNaOrdem(char* texto_param, int tamanhoSubtexto_param, int ordemSubtexto_param){
	char *ponteiro;
	int c;
	int posicao = ordemSubtexto_param*tamanhoSubtexto_param;
	ponteiro = malloc(tamanhoSubtexto_param+1);
	for(c=0 ; c<posicao-1; c++){
		texto_param++; 
	} 
	for(c=0 ; c<tamanhoSubtexto_param; c++){
		*(ponteiro+c) = *texto_param;      
		texto_param++;   
	} 
	*(ponteiro+c) = '\0';
	return ponteiro;
}
