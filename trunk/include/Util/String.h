/**
* Concentra as funções de manipulação de String.
*/

#include <string.h>

//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes
#define CARACTERE_TERMINADOR_STRING_SOPA 0


//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* @param char*	texto_param				Um texto terminado por '\0'.
* @param int	tamanhoSubtexto_param	Tamanho de cada subtexto.
* @param int	ordemSubtexto_param		Qual subtexto será retornado.
* @return char*	O subtexto de tamanho tamanhoSubtexto_param na ordem ordemSubtexto_param.
*/
char* string_pegarSubtextoNaOrdem(char* texto_param, int tamanhoSubtexto_param, int ordemSubtexto_param);

/**
* Converte um char* para int, diferenciando números de letras.
* @param char*	texto_param	O texto que será convertido.
* @return int	 Se char* contiver um caractere, retornará seu valor ASCII. Caso contrário, o interpretará como um número
*				 e retornará o int correspondente ao número.
*/
int string_paraInt(char* texto_param);



