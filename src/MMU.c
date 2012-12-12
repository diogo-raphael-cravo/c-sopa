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
* Inicializa a MMU.
* @param MMU		*MMU_param					A MMU que será inicializada.
* @param MEMORIA	*memoriaGerenciada_param	A memória que será gerenciada pela MMU.
*/
void MMU_sincronizado_inicializar(MMU *MMU_param, MEMORIA *memoriaGerenciada_param){
	sem_init(&MMU_param->mutexAcessoMMU, 0, 1);
	sem_wait(&MMU_param->mutexAcessoMMU);

	MMU_param->memoriaGerenciada = memoriaGerenciada_param;
	MMU_param->limite = 0;
	MMU_param->base = 0;

	sem_post(&MMU_param->mutexAcessoMMU);
}

/**
* Define o registrador base, que é usado para converter endereços para endereços físicos.
* @param MMU		*MMU_param					A MMU em que a operação será realizada.
* @param int		base_param					O valor base.
*/
void MMU_sincronizado_setBase(MMU *MMU_param, int base_param){
	sem_wait(&MMU_param->mutexAcessoMMU);
	MMU_param->base = base_param;
	sem_post(&MMU_param->mutexAcessoMMU);
}

/**
* Define o registrador limite, que é utilizado em endereços lógicos.
* @param MMU		*MMU_param					A MMU em que a operação será realizada.
* @param int		limite_param				O valor do limite.
*/
void MMU_sincronizado_setLimite(MMU *MMU_param, int limite_param){
	sem_wait(&MMU_param->mutexAcessoMMU);
	MMU_param->limite = limite_param;
	sem_post(&MMU_param->mutexAcessoMMU);
}

/**
* Le a memória com endereçamento físico e sem consistência de limites.
* @param MMU		*MMU_param					A MMU em que a operação será realizada.
* @param int		endereco_param				Endereço da leitura.
* @param PALAVRA	*destino_param				Endereço da variável em que o resultado será colocado.
*/
void MMU_sincronizado_lerFisico(MMU *MMU_param, int endereco_param, PALAVRA *destino_param){
	sem_wait(&MMU_param->mutexAcessoMMU);
	memoria_sincronizado_ler(MMU_param->memoriaGerenciada, endereco_param, destino_param);
	sem_post(&MMU_param->mutexAcessoMMU);
}

/*
* Escreve na memória com endereçamento físico e sem consistência de limites.
* @param MMU		*MMU_param					A MMU em que a operação será realizada.
* @param int		endereco_param				Endereço da escrita.
* @param PALAVRA	palavraEscrita_param		A palavra que será escrita na memória.
*/
void MMU_sincronizado_escreverFisico(MMU *MMU_param, int endereco_param, PALAVRA palavraEscrita_param){
	sem_wait(&MMU_param->mutexAcessoMMU);
	memoria_sincronizado_escreverPalavra(MMU_param->memoriaGerenciada, endereco_param, palavraEscrita_param);
	sem_post(&MMU_param->mutexAcessoMMU);
}

/**
* Le a memória com endereçamento lógico e consistência de limites.
* @param MMU		*MMU_param					A MMU em que a operação será realizada.
* @param int		endereco_param				Endereço da leitura.
* @param PALAVRA	*destino_param				Endereço da variável em que o resultado será colocado.
*/
void MMU_sincronizado_lerLogico(MMU *MMU_param, int endereco_param, PALAVRA *destino_param){
	sem_wait(&MMU_param->mutexAcessoMMU);
	if(MMU_param->limite <= endereco_param){
		controladorInterrupcoes_set(&global_controladorInterrupcoes, INTERRUPCAO_MEMORIA);
		*destino_param = 0;
	} else {
		memoria_sincronizado_ler(MMU_param->memoriaGerenciada, MMU_param->base + endereco_param, destino_param);
	}
	sem_post(&MMU_param->mutexAcessoMMU);
}

/*
* Escreve na memória com endereçamento lógico e consistência de limites.
* @param MMU		*MMU_param					A MMU em que a operação será realizada.
* @param int		endereco_param				Endereço da escrita.
* @param PALAVRA	palavraEscrita_param		A palavra que será escrita na memória.
*/
void MMU_sincronizado_escreverLogico(MMU *MMU_param, int endereco_param, PALAVRA palavraEscrita_param){
	sem_wait(&MMU_param->mutexAcessoMMU);
	if(MMU_param->limite <= endereco_param){
		controladorInterrupcoes_set(&global_controladorInterrupcoes, INTERRUPCAO_MEMORIA);
	} else {
		memoria_sincronizado_escreverPalavra(MMU_param->memoriaGerenciada, MMU_param->base + endereco_param, palavraEscrita_param);
	}
	sem_post(&MMU_param->mutexAcessoMMU);
}




