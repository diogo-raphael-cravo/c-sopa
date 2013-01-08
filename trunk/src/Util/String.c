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
	int tamanhoString = strlen(string_param);
	int caractereBuffer;
	char copiaInvertida[tamanhoString+1];

	memset(copiaInvertida, '\0', tamanhoString);
	for(caractereBuffer=0; caractereBuffer<=tamanhoString && string_param[caractereBuffer]!='\0'; caractereBuffer++){
		copiaInvertida[tamanhoString-caractereBuffer] = string_param[caractereBuffer];
	}
	int caractereAtual;
	for(caractereAtual=0; caractereAtual<=tamanhoString; caractereAtual++){
		copiaInvertida[caractereAtual] = copiaInvertida[caractereAtual+1];
	}
	copiaInvertida[tamanhoString+1] = '\0';
	memset(string_param, '\0', tamanhoString);
	strcpy(string_param, copiaInvertida);
}

/**
* @param char*		texto_param			O texto em que a consulta será feita.
* @param char		separador_param		Caractere utilizado como separador.
* @param int		ordem_param			Quantos caracteres devem ser ignorados.
* @return int	Índice da n-ésima ocorrência do separador na string. STRING_INDICE_INEXISTENTE, se não houver.
*/
int privada_getIndiceSeparador(char* texto_param, char separador_param, int ordem_param){
	int indiceOcorrencia = STRING_INDICE_INEXISTENTE;
	int charAtual=0;
	if(0 < strlen(texto_param)){
		while(charAtual<strlen(texto_param) && texto_param[charAtual] != separador_param){
			charAtual++;
		}
		if(texto_param[charAtual] == separador_param){
			if(ordem_param == 0){
				indiceOcorrencia = charAtual;
			} else {
				if(charAtual+2 < strlen(texto_param)){
					int indice = privada_getIndiceSeparador(&texto_param[charAtual+1], separador_param, ordem_param-1);
					indiceOcorrencia = (indice==STRING_INDICE_INEXISTENTE? STRING_INDICE_INEXISTENTE : charAtual+indice+1);
				}
			}
		}
	}
	return indiceOcorrencia;
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
	char* copiaTexto = (char*) malloc((strlen(texto_param)+1)*sizeof(char));
	int i=0;
	int ordem = ordemSubtexto_param;
	int inverter=0;
	char caractereSeparador = separador_param[0];

	strcpy(copiaTexto, texto_param);
	if(ordemSubtexto_param < 0){
		if(copiaTexto[strlen(copiaTexto)-1] == caractereSeparador){
			copiaTexto[strlen(copiaTexto)-1] = '\0';
		}
		privada_inverter(copiaTexto);
		ordem *= -1;
		ordem -= 1;
		inverter=1;
	}

	int indiceInicio = (ordemSubtexto_param == 0? 0 : privada_getIndiceSeparador(copiaTexto, caractereSeparador, ordem-1)+1);
	int indiceFim = privada_getIndiceSeparador(copiaTexto, caractereSeparador, ordem);
	indiceFim = (indiceFim != STRING_INDICE_INEXISTENTE? indiceFim-1 : strlen(copiaTexto)-1);

	if(indiceInicio != STRING_INDICE_INEXISTENTE){
		subtexto = (char*) malloc((indiceFim-indiceInicio+1)*sizeof(char));
		for(i=indiceInicio; i<=indiceFim; i++){
			subtexto[i-indiceInicio] = copiaTexto[i];
		}
	}
	free(copiaTexto);

	if(inverter && subtexto != NULL){
		privada_inverter(subtexto);
		subtexto[strlen(subtexto)-1] = '\0';
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



