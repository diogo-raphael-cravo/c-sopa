#include "../include/DadosComuns.h"

/**
* Variáveis globais acessíveis somente neste arquivo.
*/



//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------
/**
* Busca uma instrução na memória, a próxima a ser executada.
* @param PROCESSADOR	*processador_param	O processador que está realizando a busca.
* @param MEMORIA		*memoria_param		A memória em que a busca é feita.
* @return INSTRUCAO	A instrução encontrada.
*/
INSTRUCAO privada_buscaInstrucao(PROCESSADOR *processador_param, MEMORIA *memoria_param){
	INSTRUCAO instrucaoEncontrada;
	PALAVRA palavraLida;
	palavraLida = memoria_ler(memoria_param, processador_param->PC);
	instrucaoEncontrada = (INSTRUCAO) palavraLida;
	return instrucaoEncontrada;
}

/**
* Carrega uma instrução no registrador de instruções.
* @param PROCESSADOR	*processador_param	O processador em que a operação é realizada.
* @param INSTRUCAO		instrucao_param		A instrução que será carregada no IR.
*/
void privada_carregaInstrucaoIR(PROCESSADOR *processador_param, INSTRUCAO instrucao_param){
	processador_param->IR[0] = instrucao_param & 0xFF000000;
	processador_param->IR[1] = instrucao_param & 0x00FF0000;
	processador_param->IR[2] = instrucao_param & 0x0000FF00;
	processador_param->IR[3] = instrucao_param & 0x000000FF;
}

//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/**
* @param PROCESSADOR	*processador_param	O processador que será inicializado.
*/
void processador_inicializar(PROCESSADOR *processador_param){
	processador_param->PC = 0;
	int palavra=0;
	for(; palavra<TAMANHO_INSTRUCAO_PALAVRAS; palavra++){
		processador_param->IR[palavra] = 0;
	}
}

/**
* Inicia thread do processador.
* @param PROCESSADOR	*processador_param O processador que irá rodar.
*/
void processador_rodar(PROCESSADOR *processador_param){
	INSTRUCAO instrucao_buscada;
	char mensagem[100];

	while(1){
		usleep(1000*1000/10);
		instrucao_buscada = privada_buscaInstrucao(processador_param, &global_memoria);
		privada_carregaInstrucaoIR(processador_param, instrucao_buscada);
		
		sprintf(mensagem, "PROCESSADOR: PC=%d IR=%d %d %d %d", 	processador_param->PC,
			processador_param->IR[0], processador_param->IR[1], processador_param->IR[2],
			processador_param->IR[3]);
		tela_escreverNaColuna(&global_tela, mensagem, 1);
		if(processador_param->IR[3]=='L'){
			tela_escreverNaColuna(&global_tela, " Last byte is L : Request for disk", 1);
		} else {
			tela_escreverNaColuna(&global_tela, "???", 1);
		}
		processador_param->PC += 1;
	}
}








