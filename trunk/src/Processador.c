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
* @param MMU			*MMU_param			A memória em que a busca é feita.
* @return PALAVRA	A instrução encontrada.
*/
PALAVRA privada_buscaInstrucao(PROCESSADOR *processador_param, MMU *MMU_param){
	PALAVRA palavraLida;
	MMU_sincronizado_lerLogico(MMU_param, contexto_getPC(&processador_param->contextoProcessador), &palavraLida);
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
	} else if(registrador_contem(&processador_param->IR, 'L', 'M', BYTE_QUALQUER, BYTE_QUALQUER)){
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
	} else if(registrador_contem(&processador_param->IR, 'N', 'O', 'P', BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_NOP;
	} else if(registrador_contem(&processador_param->IR, 'L', 'D', 'M', BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_LOAD_REGISTER_FROM_MEMORY_32;
	} else if(registrador_contem(&processador_param->IR, 'L', 'D', 'C', BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_LOAD_REGISTER_FROM_CONSTANT_32;
	} else if(registrador_contem(&processador_param->IR, 'W', 'R', 'M', BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_WRITE_REGISTER_IN_MEMORY_32;
	} else if(registrador_contem(&processador_param->IR, 'L', 'I', BYTE_QUALQUER, BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_LOAD_REGISTER_INDIRECTLY;
	} else if(registrador_contem(&processador_param->IR, 'W', 'I', BYTE_QUALQUER, BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_WRITE_REGISTER_INDIRECTLY;
	} else if(registrador_contem(&processador_param->IR, 'J', 'C', 'A', BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_ABSOLUTE_JUMP_32;
	} else if(registrador_contem(&processador_param->IR, 'J', 'C', 'Z', BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_JUMP_ON_ZERO_32;
	} else if(registrador_contem(&processador_param->IR, 'J', 'C', 'E', BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_JUMP_ON_EQUAL_32;
	} else if(registrador_contem(&processador_param->IR, 'J', 'C', 'L', BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_JUMP_ON_LESS_32;
	} else if(registrador_contem(&processador_param->IR, 'J', 'R', 'A', BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_ABSOLUTE_JUMP_TO_REGISTER;
	} else if(registrador_contem(&processador_param->IR, 'J', 'R', 'Z', BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_JUMP_ON_ZERO_TO_REGISTER;
	} else if(registrador_contem(&processador_param->IR, 'J', 'R', 'E', BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_JUMP_ON_EQUAL_TO_REGISTER;
	} else if(registrador_contem(&processador_param->IR, 'J', 'R', 'L', BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_JUMP_ON_LESS_TO_REGISTER;
	} else if(registrador_contem(&processador_param->IR, 'C', 'A', 'L', 'L')){
		instrucaoEncontrada = INSTRUCAO_CALL_ROUTINE;
	} else if(registrador_contem(&processador_param->IR, 'R', 'E', 'T', 'C')){
		instrucaoEncontrada = INSTRUCAO_RETURN_FROM_CALL;
	} else if(registrador_contem(&processador_param->IR, 'P', 'U', 'S', BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_PUSH_REGISTER_INTO_STACK;
	} else if(registrador_contem(&processador_param->IR, 'P', 'O', 'P', BYTE_QUALQUER)){
		instrucaoEncontrada = INSTRUCAO_POP_REGISTER_FROM_STACK;
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
	PALAVRA palavraLida, palavraGravada, resultadoOperacao, conteudoOrigem, conteudoDestino;

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
			enderecoDestino = processador_param->IR.conteudo[3];
			sprintf(mensagem, "JPZ %d", enderecoDestino);
			if(processador_param->Z == 1){
				contexto_setPC(&processador_param->contextoProcessador, enderecoDestino);
			}
			break;
		case INSTRUCAO_JUMP_ON_EQUAL:
			enderecoDestino = processador_param->IR.conteudo[3];
			sprintf(mensagem, "JPE %d", enderecoDestino);
			if(processador_param->E == 1){
				contexto_setPC(&processador_param->contextoProcessador, enderecoDestino);
			}
			break;
		case INSTRUCAO_JUMP_ON_LESS:
			enderecoDestino = processador_param->IR.conteudo[3];
			sprintf(mensagem, "JPL %d", enderecoDestino);
			if(processador_param->L == 1){
				contexto_setPC(&processador_param->contextoProcessador, enderecoDestino);
			}
			break;
		case INSTRUCAO_NOP:
			sprintf(mensagem, "NOP");
			break;
		case INSTRUCAO_LOAD_REGISTER_FROM_MEMORY_32:
			registradorDestino = processador_param->IR.conteudo[3];
			MMU_sincronizado_lerLogico(&global_MMU, contexto_getPC(&processador_param->contextoProcessador), &palavraLida);
			MMU_sincronizado_lerLogico(&global_MMU, palavraLida, &palavraGravada);
			
			sprintf(mensagem, "LDM %d %d %d %d %d", registradorDestino, 
				(((palavraLida & 0xFF000000)/256)/256)/256,
				((palavraLida & 0x00FF0000)/256)/256,
				(palavraLida & 0x0000FF00)/256,
				palavraLida & 0x000000FF);

			registrador_carregarPalavra(
				contexto_getRegistrador(&processador_param->contextoProcessador, registradorDestino),
				palavraGravada);
			contexto_setPC(&processador_param->contextoProcessador, contexto_getPC(&processador_param->contextoProcessador)+1);
			break;
		case INSTRUCAO_LOAD_REGISTER_FROM_CONSTANT_32:
			registradorDestino = processador_param->IR.conteudo[3];
			MMU_sincronizado_lerLogico(&global_MMU, contexto_getPC(&processador_param->contextoProcessador), &palavraGravada);

			sprintf(mensagem, "LDC %d %d %d %d %d", registradorDestino, 
					(((palavraGravada & 0xFF000000)/256)/256)/256,
					((palavraGravada & 0x00FF0000)/256)/256,
					(palavraGravada & 0x0000FF00)/256,
					palavraGravada & 0x000000FF);
			registrador_carregarPalavra(
				contexto_getRegistrador(&processador_param->contextoProcessador, registradorDestino),
				palavraGravada);
			contexto_setPC(&processador_param->contextoProcessador, contexto_getPC(&processador_param->contextoProcessador)+1);
			break;
		case INSTRUCAO_WRITE_REGISTER_IN_MEMORY_32:
			registradorOrigem = processador_param->IR.conteudo[3];
			MMU_sincronizado_lerLogico(&global_MMU, contexto_getPC(&processador_param->contextoProcessador), &enderecoDestino);

			sprintf(mensagem, "WRM %d %d %d %d %d", registradorOrigem, 
					(((enderecoDestino & 0xFF000000)/256)/256)/256,
					((enderecoDestino & 0x00FF0000)/256)/256,
					(enderecoDestino & 0x0000FF00)/256,
					enderecoDestino & 0x000000FF);

			conteudoOrigem = registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registradorOrigem));
			MMU_sincronizado_escreverLogico(&global_MMU, enderecoDestino, conteudoOrigem);
			contexto_setPC(&processador_param->contextoProcessador, contexto_getPC(&processador_param->contextoProcessador)+1);
			break;
		case INSTRUCAO_LOAD_REGISTER_INDIRECTLY:
			registradorDestino = processador_param->IR.conteudo[2];
			registradorOrigem = processador_param->IR.conteudo[3];

			sprintf(mensagem, "LI %d %d", registradorDestino, registradorOrigem);

			conteudoDestino = registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registradorOrigem));
			MMU_sincronizado_lerLogico(&global_MMU, conteudoDestino, &enderecoDestino);
			MMU_sincronizado_lerLogico(&global_MMU, enderecoDestino, &conteudoOrigem);
			registrador_carregarPalavra(
				contexto_getRegistrador(&processador_param->contextoProcessador, registradorOrigem),
				conteudoOrigem);
			break;
		case INSTRUCAO_WRITE_REGISTER_INDIRECTLY:
			registradorOrigem = processador_param->IR.conteudo[2];
			registradorDestino = processador_param->IR.conteudo[3];

			sprintf(mensagem, "WI %d %d", registradorOrigem, registradorDestino);

			conteudoOrigem = registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registradorOrigem));
			conteudoDestino = registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registradorDestino));
			MMU_sincronizado_escreverLogico(&global_MMU, conteudoDestino, conteudoOrigem);
			break;
		case INSTRUCAO_ABSOLUTE_JUMP_32:
			MMU_sincronizado_lerLogico(&global_MMU, contexto_getPC(&processador_param->contextoProcessador), &enderecoDestino);

			sprintf(mensagem, "JCA %d %d %d %d", 
				(((enderecoDestino & 0xFF000000)/256)/256)/256,
				((enderecoDestino & 0x00FF0000)/256)/256,
				(enderecoDestino & 0x0000FF00)/256,
				enderecoDestino & 0x000000FF);

			contexto_setPC(&processador_param->contextoProcessador, enderecoDestino);
			break;
		case INSTRUCAO_JUMP_ON_ZERO_32:
			MMU_sincronizado_lerLogico(&global_MMU, contexto_getPC(&processador_param->contextoProcessador), &enderecoDestino);

			sprintf(mensagem, "JCZ %d %d %d %d", 
				(((enderecoDestino & 0xFF000000)/256)/256)/256,
				((enderecoDestino & 0x00FF0000)/256)/256,
				(enderecoDestino & 0x0000FF00)/256,
				enderecoDestino & 0x000000FF);

			if(processador_param->Z == 1){
				contexto_setPC(&processador_param->contextoProcessador, enderecoDestino);
			} else {
				contexto_setPC(&processador_param->contextoProcessador, contexto_getPC(&processador_param->contextoProcessador)+1);
			}
			break;
		case INSTRUCAO_JUMP_ON_EQUAL_32:
			MMU_sincronizado_lerLogico(&global_MMU, contexto_getPC(&processador_param->contextoProcessador), &enderecoDestino);

			sprintf(mensagem, "JCE %d %d %d %d", 
				(((enderecoDestino & 0xFF000000)/256)/256)/256,
				((enderecoDestino & 0x00FF0000)/256)/256,
				(enderecoDestino & 0x0000FF00)/256,
				enderecoDestino & 0x000000FF);

			if(processador_param->E == 1){
				contexto_setPC(&processador_param->contextoProcessador, enderecoDestino);
			} else {
				contexto_setPC(&processador_param->contextoProcessador, contexto_getPC(&processador_param->contextoProcessador)+1);
			}
			break;
		case INSTRUCAO_JUMP_ON_LESS_32:
			MMU_sincronizado_lerLogico(&global_MMU, contexto_getPC(&processador_param->contextoProcessador), &enderecoDestino);

			sprintf(mensagem, "JCL %d %d %d %d", 
				(((enderecoDestino & 0xFF000000)/256)/256)/256,
				((enderecoDestino & 0x00FF0000)/256)/256,
				(enderecoDestino & 0x0000FF00)/256,
				enderecoDestino & 0x000000FF);

			if(processador_param->L == 1){
				contexto_setPC(&processador_param->contextoProcessador, enderecoDestino);
			} else {
				contexto_setPC(&processador_param->contextoProcessador, contexto_getPC(&processador_param->contextoProcessador)+1);
			}
			break;
		case INSTRUCAO_ABSOLUTE_JUMP_TO_REGISTER:
			registradorDestino = processador_param->IR.conteudo[3];

			sprintf(mensagem, "JRA %d", registradorDestino);

			conteudoDestino = registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registradorDestino));
			contexto_setPC(&processador_param->contextoProcessador, conteudoDestino);
			break;
		case INSTRUCAO_JUMP_ON_ZERO_TO_REGISTER:
			registradorDestino = processador_param->IR.conteudo[3];

			sprintf(mensagem, "JRZ %d", registradorDestino);

			if(processador_param->Z == 1){
				conteudoDestino = registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registradorDestino));
				contexto_setPC(&processador_param->contextoProcessador, conteudoDestino);
			}
			break;
		case INSTRUCAO_JUMP_ON_EQUAL_TO_REGISTER:
			registradorDestino = processador_param->IR.conteudo[3];

			sprintf(mensagem, "JRE %d", registradorDestino);

			if(processador_param->E == 1){
				conteudoDestino = registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registradorDestino));
				contexto_setPC(&processador_param->contextoProcessador, conteudoDestino);
			}
			break;
		case INSTRUCAO_JUMP_ON_LESS_TO_REGISTER:
			registradorDestino = processador_param->IR.conteudo[3];

			sprintf(mensagem, "JRL %d", registradorDestino);

			if(processador_param->L == 1){
				conteudoDestino = registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registradorDestino));
				contexto_setPC(&processador_param->contextoProcessador, conteudoDestino);
			}
			break;
		case INSTRUCAO_CALL_ROUTINE:
			sprintf(mensagem, "CALL ainda nao foi implementado.");
			break;
		case INSTRUCAO_RETURN_FROM_CALL:
			sprintf(mensagem, "RETURN ainda nao foi implementado.");
			break;
		case INSTRUCAO_PUSH_REGISTER_INTO_STACK:
			sprintf(mensagem, "PUSH_TO_STACK ainda nao foi implementado.");
			break;
		case INSTRUCAO_POP_REGISTER_FROM_STACK:
			sprintf(mensagem, "POP_FROM_STACK ainda nao foi implementado.");
			break;
		default:
			sprintf(mensagem, "Instrucao desconhecida.");
			kernel_rodar(&global_kernel, INTERRUPCAO_PROCESSADOR);
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

		instrucaoLida = privada_buscaInstrucao(processador_param, &global_MMU);
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


/**
* @param PROCESSADOR		*processador_param	O processador cujos dados serão impressos.
* @param int				coluna_param	A coluna da tela em que a impressão será feita.
*/
void processador_imprimir(PROCESSADOR *processador_param, int coluna_param){
	char mensagem[200];

	sprintf(mensagem, "*(Z)=");
	tela_escreverNaColuna(&global_tela, mensagem, coluna_param);
	sprintf(mensagem, " =%d", processador_param->Z);
	tela_escreverNaColuna(&global_tela, mensagem, coluna_param);
	sprintf(mensagem, "*(E)=");
	tela_escreverNaColuna(&global_tela, mensagem, coluna_param);
	sprintf(mensagem, " =%d", processador_param->E);
	tela_escreverNaColuna(&global_tela, mensagem, coluna_param);
	sprintf(mensagem, "*(L)=");
	tela_escreverNaColuna(&global_tela, mensagem, coluna_param);
	sprintf(mensagem, " =%d", processador_param->L);
	tela_escreverNaColuna(&global_tela, mensagem, coluna_param);

	sprintf(mensagem, "*(IR)=");
	tela_escreverNaColuna(&global_tela, mensagem, coluna_param);
	registrador_imprimir(&processador_param->IR, coluna_param);

	contexto_imprimirRegistradores(&processador_param->contextoProcessador, coluna_param);
}

