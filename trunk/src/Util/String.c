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
*/
char* string_pegarSubtextoNaOrdem(char* texto_param, int tamanhoSubtexto_param, int ordemSubtexto_param){
	/*int i=0;
	int quantidadeSubtextosEmTexto = strlen(texto_param)/tamanhoSubtexto_param
		+ (0 < strlen(texto_param)%tamanhoSubtexto_param ? 1 : 0);
	char* subtexto = (char*)malloc(quantidadeSubtextosEmTexto*sizeof(char*));
	for(i=tamanhoSubtexto_param*ordemSubtexto_param; 
		i<tamanhoSubtexto_param*(ordemSubtexto_param+1) && texto_param[i]!='\0'; 
		i++){
		subtexto[i] = texto_param[i];
	}
	subtexto[i] = '\0';
	return subtexto;*/
	return "abc";
}
