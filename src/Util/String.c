#include "../../include/DadosComuns.h"

/**
* Variáveis globais acessíveis somente neste arquivo.
*/

//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------
/**
* @param char* 		texto_param			O texto cujo número de substrings será contado.
* @param char*		separador_param		String que delimita subtextos.
* @return int	Quantidade de substrings com o separador.
*/
int privada_quantidadeSubtextos(char* texto_param, char* separador_param){
	int quantidade=0;
	char *copia = (char*) malloc((strlen(texto_param)+1)*sizeof(char));
	strcpy(copia, texto_param);
	if(strtok(copia, separador_param) != NULL){ quantidade++; }
	while(strtok(NULL, separador_param) != NULL){ quantidade++; }
	free(copia);
	return quantidade;
}

/**
* @param char*		string_param		String que será invertida.
*/
void privada_inverter(char* string_param){
	int caracteresInvertidos=0;
	char caractereBuffer;
	for(; caracteresInvertidos<strlen(string_param)%2; caracteresInvertidos++){
		caractereBuffer = string_param[caracteresInvertidos];
		string_param[caracteresInvertidos] = string_param[strlen(string_param)-caracteresInvertidos];
		string_param[strlen(string_param)-caracteresInvertidos] = caractereBuffer;
	}
}

/**
* @param char*			string_param		String em que será feita a busca.
* @param char*			separador_param		O separador utilizado.
* @return char*		String após primeira ocorrência do separador.
*/
char* privada_getSubstringApos(char* string_param, char* separador_param){
	char* substringApos = NULL;
	int i=0;
	while(string_param[i] != '\0'){
		if(substringApos==NULL && string_param[i]==separador_param[0]){
			substringApos = &string_param[i];
		}
		i++;
	}
	return substringApos;
}

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

/**
* @param char*	texto_param				Um texto terminado por '\0'.
* @param char*	separador_param			String que delimita subtextos.
* @param int	ordemSubtexto_param		Qual subtexto será retornado. Começa em 0. Aceita valores negativos.
* @return char*	O subtexto na ordem ordemSubtexto_param.
*/
char* string_pegarSubtexto(char* texto_param, char* separador_param, int ordemSubtexto_param){
	char* subtexto = NULL;
	char* subtextoAtual;
	char* copiaTexto = (char*) malloc((strlen(texto_param)+1)*sizeof(char));
	int i=0;
	int ordem = ordemSubtexto_param;
	int inverter=0;

	if(ordemSubtexto_param < 0){
		privada_inverter(copiaTexto);
		ordem *= -1;
		inverter=1;
	}
tela_escreverNaColuna(&global_tela, "Pegar subtexto de:", 3);
tela_escreverNaColuna(&global_tela, texto_param, 3);
	strcpy(copiaTexto, texto_param);
	subtextoAtual = privada_getSubstringApos(copiaTexto, separador_param);
	while(subtextoAtual != NULL){
tela_escreverNaColuna(&global_tela, "Subtexto Atual:", 3);
tela_escreverNaColuna(&global_tela, subtextoAtual, 3);
		if(i == ordem){
			subtexto = (char*) malloc((strlen(subtextoAtual)+1)*sizeof(char));
			strcpy(subtexto, subtextoAtual);
tela_escreverNaColuna(&global_tela, "Subtexto:", 3);
tela_escreverNaColuna(&global_tela, subtexto, 3);
		}
		i++;
		subtextoAtual = privada_getSubstringApos(subtextoAtual, separador_param);
	}
	free(copiaTexto);
	free(subtextoAtual);

	if(inverter && subtexto != NULL){
		privada_inverter(subtexto);
	}

	return subtexto;
}

/**
* @param char*	texto_param				Um texto terminado por '\0'.
* @param char*	separador_param			String que delimita subtextos.
* @param int	subtextosAntes_param	Quantos subtextos haverão antes.
* @param int	subtextosDepois_param	Quantos subtextos haverão depois.
* @return char*	O subtexto na ordem ordemSubtexto_param.
*/
char* string_pegarSubtextoEntreSubtextos(char* texto_param, char* separador_param, int subtextosAntes_param, int subtextosDepois_param){
	char* subtexto = (char*) malloc((strlen(texto_param)+1)*sizeof(char));
	int i=0;
	int ordemSubtextoInicial = subtextosAntes_param;
	int ordemSubtextoFinal =  privada_quantidadeSubtextos(texto_param, separador_param)-subtextosDepois_param-1;

	memset(subtexto, '\0', (strlen(texto_param)+1));
	for(; i<privada_quantidadeSubtextos(texto_param, separador_param); i++){
		if(ordemSubtextoInicial <= i && i <= ordemSubtextoFinal
				&& string_pegarSubtexto(texto_param, separador_param, i) != NULL){
			strcat(subtexto, string_pegarSubtexto(texto_param, separador_param, i));
			strcat(subtexto, separador_param);
		}
	}

	return subtexto;
}

/**
* Converte um char* para int, diferenciando números de letras.
* @param char*	texto_param	O texto que será convertido.
* @return int	 Se char* contiver um caractere, retornará seu valor ASCII. Caso contrário, o interpretará como um número
*				 e retornará o int correspondente ao número.
*/
int string_paraInt(char* texto_param){
	int valorInt;
	if(strlen(texto_param) == 1){
		valorInt = (int) texto_param[0];
	} else {
		valorInt = atoi(texto_param);
	}
	return valorInt;
}



