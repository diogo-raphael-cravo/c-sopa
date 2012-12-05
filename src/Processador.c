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
* @return PALAVRA	A instrução encontrada.
*/
PALAVRA privada_buscaInstrucao(PROCESSADOR *processador_param, MEMORIA *memoria_param){
	PALAVRA palavraLida;
	palavraLida = memoria_sincronizado_ler(memoria_param, processador_param->PC);
	return palavraLida;
}

/**
* Decodifica uma instrução.
* @param PROCESSADOR	*processador_param	O processador que está realizando a busca.
* @return INSTRUCAO	A instrução decodificada.
*/
INSTRUCAO privada_decodificaInstrucao(PROCESSADOR *processador_param){
	INSTRUCAO instrucaoEncontrada;
	if(processador_param->IR[0] == 'J' && processador_param->IR[0] == 'P' && processador_param->IR[0] == 'A'){
		instrucaoEncontrada = INSTRUCAO_JPA;
	} else {
		instrucaoEncontrada = INSTRUCAO_INEXISTENTE;
	}
	return instrucaoEncontrada;
}

/**
* Executa uma instrução.
* @param PROCESSADOR	*processador_param	O processador que está executando a instrução.
* @param INSTRUCAO		instrucao_param		A instrução que será executada.
*/
void privada_executaInstrucao(PROCESSADOR *processador_param, INSTRUCAO instrucao_param){
	if(instrucao_param == INSTRUCAO_JPA){
		tela_escreverNaColuna(&global_tela, "Jumping...", 1);
	} else {
		tela_escreverNaColuna(&global_tela, "Instrucao desconhecida.", 1);
	}
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
	PALAVRA instrucaoLida;
	INSTRUCAO instrucaoBuscada;
	char mensagem[100];

	while(1){
		usleep(1000*1000/10);
		instrucaoLida = privada_buscaInstrucao(processador_param, &global_memoria);
		privada_carregaInstrucaoIR(processador_param, instrucaoLida);
		instrucaoBuscada = privada_decodificaInstrucao(processador_param);
		privada_executaInstrucao(processador_param, instrucaoBuscada);
		
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
		if(controladorInterrupcoes_sincronizado_get(&global_controladorInterrupcoes) != SEM_INTERRUPCAO){
			kernel_rodar(&global_kernel, controladorInterrupcoes_sincronizado_get(&global_controladorInterrupcoes));
			controladorInterrupcoes_sincronizado_reset(&global_controladorInterrupcoes);
		}
	}
}








