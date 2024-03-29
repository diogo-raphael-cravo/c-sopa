#include "../include/DadosComuns.h"
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
* Inicializa o registrador.
* @param REGISTRADOR	*registrador_param	O registrador em que será feita a operação.
*/
void registrador_inicializar(REGISTRADOR *registrador_param){
	int byte=0;
	for(; byte<TAMANHO_REGISTRADOR_BYTES; byte++){
		registrador_param->conteudo[byte] = 0;
	}
}

/**
* Carrega no registrador os bytes.
* @param REGISTRADOR	*registrador_param	O registrador em que será feita a escrita.
* @param BYTE			byte0_param			O byte que ficará na posição 0 da palavra, que conterá o bit mais significativo da palavra.
* @param BYTE			byte1_param			O byte que ficará na posição 1 da palavra.
* @param BYTE			byte2_param			O byte que ficará na posição 2 da palavra.
* @param BYTE			byte3_param			O byte que ficará na posição 3 da palavra.
*/
void registrador_carregarBytes(REGISTRADOR *registrador_param, BYTE byte0_param, BYTE byte1_param, BYTE byte2_param, BYTE byte3_param){
	registrador_param->conteudo[0] = byte0_param;
	registrador_param->conteudo[1] = byte1_param;
	registrador_param->conteudo[2] = byte2_param;
	registrador_param->conteudo[3] = byte3_param;
}

/**
* Carrega no registrador os bytes.
* @param REGISTRADOR	*registrador_param	O registrador em que será feita a escrita.
* @param PALAVRA		palavra_param		A palavra que será carregada no registrador.
*/
void registrador_carregarPalavra(REGISTRADOR *registrador_param, PALAVRA palavra_param){
	registrador_param->conteudo[0] = (((palavra_param & 0xFF000000)/256)/256)/256;
	registrador_param->conteudo[1] = ((palavra_param & 0x00FF0000)/256)/256;
	registrador_param->conteudo[2] = (palavra_param & 0x0000FF00)/256;
	registrador_param->conteudo[3] = palavra_param & 0x000000FF;
}

/**
* @param REGISTRADOR						*registrador_param	O registrador que será lido.
* @return BYTE*	O conteúdo do registrador em bytes. O índice 0 contém os bits mais significativos.
*/
BYTE* registrador_lerBytes(REGISTRADOR *registrador_param){
	int byte;
	REGISTRADOR *registradorCopia = (REGISTRADOR*) malloc(sizeof(REGISTRADOR));
	for(byte=0; byte<TAMANHO_REGISTRADOR_BYTES; byte++){
		registradorCopia->conteudo[byte] = registrador_param->conteudo[byte];
	}
	return registradorCopia->conteudo;
}

/**
* @param REGISTRADOR	*registrador_param	O registrador que será lido.
* @return PALAVRA	A palavra que estava guardada no registrador.
*/
PALAVRA registrador_lerPalavra(REGISTRADOR *registrador_param){
	PALAVRA palavraFinal = 0;
	palavraFinal = palavraFinal | (registrador_param->conteudo[0]*256*256*256 & 0xFF000000);
	palavraFinal = palavraFinal | (registrador_param->conteudo[1]*256*256 & 0x00FF0000);
	palavraFinal = palavraFinal | (registrador_param->conteudo[2]*256 & 0x0000FF00);
	palavraFinal = palavraFinal | (registrador_param->conteudo[3] & 0x000000FF);
	return palavraFinal;
}

/**
* Compara o conteúdo do registrador com os valores passados.
* @param REGISTRADOR	*registrador_param	Registrador cujo conteúdo será testado.
* @param BYTE			byte0_param			O byte que ficará na posição 0 da palavra, que conterá o bit mais significativo da palavra.
* @param BYTE			byte1_param			O byte que ficará na posição 1 da palavra.
* @param BYTE			byte2_param			O byte que ficará na posição 2 da palavra.
* @param BYTE			byte3_param			O byte que ficará na posição 3 da palavra.
* @return int	Indica se o registrador contém os bytes passados.
* ATENÇÃO: BYTE_QUALQUER é interpretado como wildcard, isto é, significa que o byte será ignorado para o resultado final.
*		exemplo: registrador_contem('*', 'A', 'Y', 'P'); Considerará somente os bytes 1, 2 e 3.
*/
int registrador_contem(REGISTRADOR *registrador_param, BYTE byte0_param, BYTE byte1_param, BYTE byte2_param, BYTE byte3_param){
	int contem=1;
	if(byte0_param != BYTE_QUALQUER && registrador_param->conteudo[0] != byte0_param){ contem = 0; }
	if(byte1_param != BYTE_QUALQUER && registrador_param->conteudo[1] != byte1_param){ contem = 0; }
	if(byte2_param != BYTE_QUALQUER && registrador_param->conteudo[2] != byte2_param){ contem = 0; }
	if(byte3_param != BYTE_QUALQUER && registrador_param->conteudo[3] != byte3_param){ contem = 0; }
	return contem;
}

/**
* Copia conteúdo de outro registrador.
* @param REGISTRADOR	*registradorDestino_param	O destino da cópia.
* @param REGISTRADOR	*registradorOrigem_param	A origem da cópia.
*/
void registrador_copiar(REGISTRADOR *registradorDestino_param, REGISTRADOR *registradorOrigem_param){
	int byte=0;
	for(; byte<TAMANHO_REGISTRADOR_BYTES; byte++){
		registradorDestino_param->conteudo[byte] = registradorOrigem_param->conteudo[byte];
	}
}

/**
* @param REGISTRADOR	*registrador_param	Registrador cujo conteúdo será alterado.
* @param int			parcela_param	A quantidade que deve ser somada ao valor do registrador.
*/
void registrador_somar(REGISTRADOR *registrador_param, int parcela_param){
	registrador_param->conteudo[3] = registrador_param->conteudo[3]+parcela_param;

	if(256 <= registrador_param->conteudo[3]){
		registrador_param->conteudo[3] -= 256;
		registrador_param->conteudo[2] += 1;
	}
	if(256 <= registrador_param->conteudo[2]){
		registrador_param->conteudo[2] = 0;
		registrador_param->conteudo[1] += 1;
	}
	if(256 <= registrador_param->conteudo[1]){
		registrador_param->conteudo[1] = 0;
		registrador_param->conteudo[0] += 1;
	}
	if(256 <= registrador_param->conteudo[0]){
		registrador_param->conteudo[0] = 0;
	}
}

/**
* Imprime o registrador na coluna fornecida da tela.
* @param REGISTRADOR	*registrador_param	O registrador que será impresso.
* @param int			coluna_param		A coluna em que o registrador será impresso.
*/
void registrador_imprimir(REGISTRADOR *registrador_param, int coluna_param){
	char mensagem[200];
	sprintf(mensagem, " =%d", registrador_lerPalavra(registrador_param));
	tela_escreverNaColuna(&global_tela, mensagem, coluna_param);
	sprintf(mensagem, " =%d %d %d %d", registrador_param->conteudo[0],
		registrador_param->conteudo[1], registrador_param->conteudo[2], registrador_param->conteudo[3]);
	tela_escreverNaColuna(&global_tela, mensagem, coluna_param);
}













