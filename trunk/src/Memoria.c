#include "../include/DadosComuns.h"
/**
* Variáveis globais acessíveis somente neste arquivo.
*/



//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------
/**
* Limpa completamente a memória fornecida.
* @param MEMORIA	*memoria_param	A memória que será limpa.
*/
void privada_limpar(MEMORIA *memoria_param){
	int particao=0;
	int palavra=0;
	for(; particao<QUANTIDADE_PARTICOES_MEMORIA; particao++){
		for(palavra=0; palavra<QUANTIDADE_PALAVRAS_PARTICAO; palavra++){
			memoria_param->particoes[particao][palavra] = 2*palavra;
		}
	}
}

//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/**
* @param MEMORIA	*memoria_param	A memória que será inicializada.
*/
void memoria_inicializar(MEMORIA *memoria_param){
	sem_init(&memoria_param->mutexAcessoMemoria, 0, 1);
	sem_wait(&memoria_param->mutexAcessoMemoria);
	privada_limpar(memoria_param);
	sem_post(&memoria_param->mutexAcessoMemoria);
}

/**
* @param MEMORIA	*memoria_param			A memória em que a escrita será feita.
* @param int		endereco_param			Endereço da memória em que a escrita será feita.
* @param PALAVRA 	dadosEscritos_param		O que será efetivamente escrito em endereco_param.
*/
void memoria_escrever(MEMORIA *memoria_param, int endereco_param, PALAVRA dadosEscritos_param){
	sem_wait(&memoria_param->mutexAcessoMemoria);
	int particao = 0;
	int palavra = endereco_param;
	memoria_param->particoes[particao][palavra] = dadosEscritos_param;
	sem_post(&memoria_param->mutexAcessoMemoria);
}

/**
* @param MEMORIA	*memoria_param			A memória em que a leitura será feita.
* @param int		endereco_param			Endereço da memória em que a leitura será feita.
* @return PALAVRA	Conteúdo da memória no endereço dado.
*/
PALAVRA memoria_ler(MEMORIA *memoria_param, int endereco_param){
	sem_wait(&memoria_param->mutexAcessoMemoria);
	PALAVRA dadosLidos;
	int particao = 0;
	int palavra = endereco_param;
	dadosLidos = memoria_param->particoes[particao][palavra];
	sem_post(&memoria_param->mutexAcessoMemoria);
	return dadosLidos;
}




