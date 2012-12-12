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
	memoria_sincronizado_ler(memoria_param, contexto_getPC(&processador_param->contextoProcessador), &palavraLida);
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
		instrucaoEncontrada = INSTRUCAO_ABSOLUTE_JUMP;
	} else if(registrador_contem(&processador_param->IR, 'I', 'N', 'T', BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_SOFTWARE_INTERRUPT;
	} else if(registrador_contem(&processador_param->IR, 'L', 'D', BYTE_QUALQUER, BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_LOAD_REGISTER_FROM_MEMORY;
	} else if(registrador_contem(&processador_param->IR, 'L', 'C', BYTE_QUALQUER, BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_LOAD_REGISTER_FROM_CONSTANT;
	} else if(registrador_contem(&processador_param->IR, 'W', 'M', BYTE_QUALQUER, BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_WRITE_REGISTER_IN_MEMORY;
	} else if(registrador_contem(&processador_param->IR, 'S', 'U', BYTE_QUALQUER, BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_SUBTRACT_REGISTERS;
	} else if(registrador_contem(&processador_param->IR, 'A', 'D', BYTE_QUALQUER, BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_ADD_REGISTERS;
	} else if(registrador_contem(&processador_param->IR, 'D', 'E', 'C', BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_DECREMENT_REGISTER;
	} else if(registrador_contem(&processador_param->IR, 'I', 'N', 'C', BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_INCREMENT_REGISTER;
	} else if(registrador_contem(&processador_param->IR, 'C', 'P', BYTE_QUALQUER, BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_COMPARE_REGISTERS;
	} else if(registrador_contem(&processador_param->IR, 'J', 'P', 'Z', BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_JUMP_ON_ZERO;
	} else if(registrador_contem(&processador_param->IR, 'J', 'P', 'E', BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_JUMP_ON_EQUAL;
	} else if(registrador_contem(&processador_param->IR, 'J', 'P', 'L', BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_JUMP_ON_LESS;
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
	char mensagem[200];
	int enderecoDestino, qualInterrupcao, qualRegistrador, registradorOrigem, registradorDestino;
	PALAVRA palavraGravada, resultadoOperacao, conteudoOrigem, conteudoDestino, conteudoRegistrador;
	switch(instrucao_param){
		case INSTRUCAO_ABSOLUTE_JUMP:
			enderecoDestino = processador_param->IR.conteudo[3];
			sprintf(mensagem, "JPA %d", enderecoDestino);
			contexto_setPC(&processador_param->contextoProcessador, enderecoDestino);
			break;
		case INSTRUCAO_SOFTWARE_INTERRUPT:
			qualInterrupcao = processador_param->IR.conteudo[3];
			sprintf(mensagem, "INT %d", qualInterrupcao);
			kernel_rodar(&global_kernel, qualInterrupcao);
			break;
		case INSTRUCAO_LOAD_REGISTER_FROM_MEMORY:
			qualRegistrador = processador_param->IR.conteudo[2];
			MMU_sincronizado_lerLogico(&global_MMU, processador_param->IR.conteudo[3], &palavraGravada);
			sprintf(mensagem, "LM %d %d", qualRegistrador, palavraGravada);
			contexto_setRegistradorPalavra(&processador_param->contextoProcessador, palavraGravada, qualRegistrador);
			break;
		case INSTRUCAO_LOAD_REGISTER_FROM_CONSTANT:
			qualRegistrador = processador_param->IR.conteudo[2];
			palavraGravada = processador_param->IR.conteudo[3];
			sprintf(mensagem, "LC %d %d", qualRegistrador, palavraGravada);
			contexto_setRegistradorPalavra(&processador_param->contextoProcessador, palavraGravada, qualRegistrador);
			break;
		case INSTRUCAO_WRITE_REGISTER_IN_MEMORY:
			qualRegistrador = processador_param->IR.conteudo[2];
			enderecoDestino = processador_param->IR.conteudo[3];
			sprintf(mensagem, "WM %d %d", qualRegistrador, enderecoDestino);
			MMU_sincronizado_escreverLogico(&global_MMU, enderecoDestino, 
				registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, qualRegistrador)));
			break;
		case INSTRUCAO_SUBTRACT_REGISTERS:
			registradorDestino = processador_param->IR.conteudo[2];
			registradorOrigem = processador_param->IR.conteudo[3];
			resultadoOperacao = registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registradorDestino))
				- registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registradorOrigem));
			sprintf(mensagem, "SU %d %d", registradorDestino, registradorOrigem);
			contexto_setRegistradorPalavra(&processador_param->contextoProcessador, resultadoOperacao, registradorDestino);
			break;
		case INSTRUCAO_ADD_REGISTERS:
			registradorDestino = processador_param->IR.conteudo[2];
			registradorOrigem = processador_param->IR.conteudo[3];
			resultadoOperacao = registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registradorDestino))
				+ registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registradorOrigem));
			sprintf(mensagem, "AD %d %d", registradorDestino, registradorOrigem);
			contexto_setRegistradorPalavra(&processador_param->contextoProcessador, resultadoOperacao, registradorDestino);
			break;
		case INSTRUCAO_DECREMENT_REGISTER:
			qualRegistrador = processador_param->IR.conteudo[3];
			sprintf(mensagem, "DEC %d", qualRegistrador);
			registrador_somar(contexto_getRegistrador(&processador_param->contextoProcessador, qualRegistrador), -1);
			break;
		case INSTRUCAO_INCREMENT_REGISTER:
			qualRegistrador = processador_param->IR.conteudo[3];
			sprintf(mensagem, "INC %d", qualRegistrador);
			registrador_somar(contexto_getRegistrador(&processador_param->contextoProcessador, qualRegistrador), 1);
			break;
		case INSTRUCAO_COMPARE_REGISTERS:
			registradorOrigem = processador_param->IR.conteudo[2];
			registradorDestino = processador_param->IR.conteudo[3];
			conteudoOrigem = registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registradorOrigem));
			conteudoDestino = registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registradorDestino));
			sprintf(mensagem, "CP %d %d", registradorOrigem, registradorDestino);
			processador_param->Z = (conteudoOrigem==0					 ? 1 : 0);
			processador_param->E = (conteudoOrigem==conteudoDestino		 ? 1 : 0);
			processador_param->L = (conteudoOrigem<conteudoDestino		 ? 1 : 0);
			break;
		case INSTRUCAO_JUMP_ON_ZERO:
			qualRegistrador = processador_param->IR.conteudo[3];
			conteudoRegistrador = registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, qualRegistrador));
			sprintf(mensagem, "JPZ %d", qualRegistrador);
			if(processador_param->Z == 1){
				contexto_setPC(&processador_param->contextoProcessador, conteudoRegistrador);
			}
			break;
		case INSTRUCAO_JUMP_ON_EQUAL:
			qualRegistrador = processador_param->IR.conteudo[3];
			conteudoRegistrador = registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, qualRegistrador));
			sprintf(mensagem, "JPE %d", qualRegistrador);
			if(processador_param->E == 1){
				contexto_setPC(&processador_param->contextoProcessador, conteudoRegistrador);
			}
			break;
		case INSTRUCAO_JUMP_ON_LESS:
			qualRegistrador = processador_param->IR.conteudo[3];
			conteudoRegistrador = registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, qualRegistrador));
			sprintf(mensagem, "JPL %d", qualRegistrador);
			if(processador_param->L == 1){
				contexto_setPC(&processador_param->contextoProcessador, conteudoRegistrador);
			}
			break;
		default:
			sprintf(mensagem, "Instrucao desconhecida.");
	}
	tela_escreverNaColuna(&global_tela, mensagem, 1);
}



//---------------------------------------------------------------------
//			FUNÇÕES PÚBLICAS DO HEADER						
//---------------------------------------------------------------------
/**
* @param PROCESSADOR	*processador_param	O processador que será inicializado.
*/
void processador_inicializar(PROCESSADOR *processador_param){
	processador_param->Z = 0;
	processador_param->E = 0;
	processador_param->L = 0;
	registrador_inicializar(&processador_param->IR);
	contexto_inicializar(&processador_param->contextoProcessador);
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
		sprintf(mensagem, "PROCESSADOR: PC=%d IR=%d %d %d %d", 	contexto_getPC(&processador_param->contextoProcessador),
			processador_param->IR.conteudo[0], processador_param->IR.conteudo[1], processador_param->IR.conteudo[2],
			processador_param->IR.conteudo[3]);
		tela_escreverNaColuna(&global_tela, mensagem, 1);

		instrucaoLida = privada_buscaInstrucao(processador_param, &global_memoria);
		contexto_setPC(&processador_param->contextoProcessador, contexto_getPC(&processador_param->contextoProcessador)+1);
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
* @param PROCESSADOR		*processador_param	O processador cujo contexto será restaurado.
* @param CONTEXTO			*contexto_param		O contexto que o processador terá.
*/
void processador_setContexto(PROCESSADOR *processador_param, CONTEXTO *contexto_param){
	contexto_copiar(&processador_param->contextoProcessador, contexto_param);
}

/**
* @param PROCESSADOR		*processador_param	O processador cujo contexto será retornado.
* @return CONTEXTO*	O contexto do processador.
*/
CONTEXTO* processador_getContexto(PROCESSADOR *processador_param){
	return &processador_param->contextoProcessador;
}




