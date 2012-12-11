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
	memoria_sincronizado_ler(memoria_param, processador_param->PC, &palavraLida);
	return palavraLida;
}

/**
* Decodifica uma instrução.
* @param PROCESSADOR	*processador_param	O processador que está realizando a busca.
* @return INSTRUCAO	A instrução decodificada.
*/
INSTRUCAO privada_decodificaInstrucao(PROCESSADOR *processador_param){
	INSTRUCAO instrucaoEncontrada;
	if(registrador_contem(&processador_param->IR, 'J', 'P', 'A', BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_JPA;
	} else 	if(registrador_contem(&processador_param->IR, 'I', 'N', 'T', BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_INT;
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
		processador_param->PC = processador_param->IR.conteudo[3];
	} else if(instrucao_param == INSTRUCAO_INT){
		tela_escreverNaColuna(&global_tela, "Interrupcao de Software.", 1);
		kernel_rodar(&global_kernel, processador_param->IR.conteudo[3]);
	} else {
		tela_escreverNaColuna(&global_tela, "Instrucao desconhecida.", 1);
	}
}



//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/**
* @param PROCESSADOR	*processador_param	O processador que será inicializado.
*/
void processador_inicializar(PROCESSADOR *processador_param){
	processador_param->PC = 0;
	registrador_inicializar(&processador_param->IR);
	registrador_inicializar(&processador_param->registrador);
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
		sprintf(mensagem, "PROCESSADOR: PC=%d IR=%d %d %d %d", 	processador_param->PC,
			processador_param->IR.conteudo[0], processador_param->IR.conteudo[1], processador_param->IR.conteudo[2],
			processador_param->IR.conteudo[3]);
		tela_escreverNaColuna(&global_tela, mensagem, 1);

		instrucaoLida = privada_buscaInstrucao(processador_param, &global_memoria);
		processador_param->PC += 1;
		registrador_carregarPalavra(&processador_param->IR, instrucaoLida);
		instrucaoBuscada = privada_decodificaInstrucao(processador_param);
		privada_executaInstrucao(processador_param, instrucaoBuscada);
		
		
		if(controladorInterrupcoes_get(&global_controladorInterrupcoes) != SEM_INTERRUPCAO){
			kernel_rodar(&global_kernel, controladorInterrupcoes_get(&global_controladorInterrupcoes));
			controladorInterrupcoes_reset(&global_controladorInterrupcoes);
		}
	}
}

/**
* @param PROCESSADOR	*processador_param	O processador no qual a informação será buscada.
* @return int	PC do processador.
*/
int processador_getPC(PROCESSADOR *processador_param){
	return processador_param->PC;
}

/**
* @param PROCESSADOR	*processador_param	O processador no qual a informação será buscada.
* @return REGISTRADOR*	O registrador do processador.
*/
REGISTRADOR* processador_getRegistrador(PROCESSADOR *processador_param){
	return &processador_param->registrador;
}

/**
* @param PROCESSADOR	*processador_param	O processador no qual a operação será realizada.
* @param int				PC_param		O PC que o processador deverá ter.
*/
void processador_setPC(PROCESSADOR *processador_param, int PC_param){
	processador_param->PC = PC_param;
}

/**
* @param PROCESSADOR	*processador_param		O processador no qual a operação será realizada.
* @param REGISTRADOR	*registrador_param		Registrador que contém o valor que conterá o registrador 
*												do processador ao fim da operação.
*/
void processador_setRegistrador(PROCESSADOR *processador_param, REGISTRADOR *registrador_param){
	registrador_copiar(&processador_param->registrador, registrador_param);
}







