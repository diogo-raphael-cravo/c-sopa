#include "../include/DadosComuns.h"

/**
* Variáveis globais acessíveis somente neste arquivo.
*/



//---------------------------------------------------------------------
//			FUNÇÕES PRIVADAS DESTE ARQUIVO						
//---------------------------------------------------------------------
/**
* @param PROCESSADOR	*processador_param			O processador que irá ler.
* @return int	O tamanho da stack do processo rodando no processador, isto é, a posição "que está em PC=-1", por assim dizer.
*/
int privada_getTamanhoStackProcessoRodando(PROCESSADOR *processador_param){
	PALAVRA tamanhoStack;
	int base;
	MMU_sincronizado_getBase(&global_MMU, &base);
	MMU_sincronizado_lerFisico(&global_MMU, base-1, &tamanhoStack);
	return tamanhoStack;
}

/**
* Busca uma instrução na memória, a próxima a ser executada.
* @param PROCESSADOR	*processador_param			O processador que está realizando a busca.
* @param MMU			*MMU_param					A memória em que a busca é feita.
* @param int			*houveAcessoIlegal_param	Indica se houve acesso ilegal à memoria.
* @return PALAVRA	A instrução encontrada.
*/
PALAVRA privada_buscaInstrucao(PROCESSADOR *processador_param, MMU *MMU_param, int *houveAcessoIlegal_param){
	PALAVRA palavraLida;
	*houveAcessoIlegal_param = MMU_sincronizado_lerLogico(MMU_param, contexto_getPC(&processador_param->contextoProcessador), &palavraLida, 0);
	if(*houveAcessoIlegal_param){
		kernel_rodar(&global_kernel, INTERRUPCAO_SEGMENTACAO_MEMORIA);
	}
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

/**********************************************************
* Seguem implementações das instruções do processador.
* Seus parâmetros são os parâmetros das instruções.
* As funções esperam posições de memória e registradores válidos, elas não farão checagem dos parâmetros!
* Para todas:
* @param PROCESSADOR 	*processador_param			O processador que realizará a instrução.
* @param char*			mensagemInstrucao_param		Variável em que a mensagem que explica a instrução será colocada.
**********************************************************/

void privada_JPA(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int enderecoDestino_param){
	sprintf(mensagemInstrucao_param, "JPA %d", enderecoDestino_param);
	contexto_setPC(&processador_param->contextoProcessador, enderecoDestino_param);
}
void privada_INT(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int interrupcao_param){
	sprintf(mensagemInstrucao_param, "INT %d", interrupcao_param);
	kernel_rodar(&global_kernel, interrupcao_param);
}
void privada_LM(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int registradorDestino_param, int enderecoMemoria_param){
	sprintf(mensagemInstrucao_param, "LM %d %d", registradorDestino_param, enderecoMemoria_param);

	PALAVRA palavraGravada;
	int invadiuMemoria = MMU_sincronizado_lerLogico(&global_MMU, processador_param->IR.conteudo[3], &palavraGravada, 0);
	if(invadiuMemoria){
		kernel_rodar(&global_kernel, INTERRUPCAO_SEGMENTACAO_MEMORIA);
	} else {
		contexto_setRegistradorPalavra(&processador_param->contextoProcessador, palavraGravada, registradorDestino_param);
	}
}
void privada_LC(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int registradorDestino_param, PALAVRA constante_param){
	sprintf(mensagemInstrucao_param, "LC %d %d", registradorDestino_param, constante_param);
	contexto_setRegistradorPalavra(&processador_param->contextoProcessador, constante_param, registradorDestino_param);
}
void privada_WM(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int qualRegistrador_param, PALAVRA enderecoDestino_param){
	sprintf(mensagemInstrucao_param, "WM %d %d", qualRegistrador_param, enderecoDestino_param);
	PALAVRA conteudoRegistrador = registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, qualRegistrador_param));
	int invadiuMemoria = MMU_sincronizado_escreverLogico(&global_MMU, enderecoDestino_param, conteudoRegistrador, 0);
	if(invadiuMemoria){
		kernel_rodar(&global_kernel, INTERRUPCAO_SEGMENTACAO_MEMORIA);
	}
}
void privada_SU(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int registradorDestino_param, int registradorOrigem_param){
	int resultadoOperacao = 
		registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registradorDestino_param))
		- registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registradorOrigem_param));
	sprintf(mensagemInstrucao_param, "SU %d %d", registradorDestino_param, registradorOrigem_param);
	contexto_setRegistradorPalavra(&processador_param->contextoProcessador, resultadoOperacao, registradorDestino_param);
}
void privada_AD(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int registradorDestino_param, int registradorOrigem_param){
	int resultadoOperacao = 
		registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registradorDestino_param))
		+ registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registradorOrigem_param));
	sprintf(mensagemInstrucao_param, "AD %d %d", registradorDestino_param, registradorOrigem_param);
	contexto_setRegistradorPalavra(&processador_param->contextoProcessador, resultadoOperacao, registradorDestino_param);
}
void privada_DEC(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int registrador_param){
	sprintf(mensagemInstrucao_param, "DEC %d", registrador_param);
	registrador_somar(contexto_getRegistrador(&processador_param->contextoProcessador, registrador_param), -1);
}
void privada_INC(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int registrador_param){
	sprintf(mensagemInstrucao_param, "INC %d", registrador_param);
	registrador_somar(contexto_getRegistrador(&processador_param->contextoProcessador, registrador_param), 1);
}
void privada_CP(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int registrador1_param, int registrador2_param){
	PALAVRA conteudo1 = registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registrador1_param));
	PALAVRA conteudo2 = registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registrador2_param));
	sprintf(mensagemInstrucao_param, "CP %d %d", registrador1_param, registrador2_param);
	processador_param->Z = (conteudo1==0			 ? 1 : 0);
	processador_param->E = (conteudo1==conteudo2	 ? 1 : 0);
	processador_param->L = (conteudo1<conteudo2		 ? 1 : 0);
}
void privada_JPZ(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int enderecoDestino_param){
	sprintf(mensagemInstrucao_param, "JPZ %d", enderecoDestino_param);
	if(processador_param->Z == 1){
		contexto_setPC(&processador_param->contextoProcessador, enderecoDestino_param);
	}
}
void privada_JPE(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int enderecoDestino_param){
	sprintf(mensagemInstrucao_param, "JPE %d", enderecoDestino_param);
	if(processador_param->E == 1){
		contexto_setPC(&processador_param->contextoProcessador, enderecoDestino_param);
	}
}
void privada_JPL(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int enderecoDestino_param){
	sprintf(mensagemInstrucao_param, "JPL %d", enderecoDestino_param);
	if(processador_param->L == 1){
		contexto_setPC(&processador_param->contextoProcessador, enderecoDestino_param);
	}
}
void privada_NOP(PROCESSADOR *processador_param, char* mensagemInstrucao_param){
	sprintf(mensagemInstrucao_param, "NOP");
}
void privada_LDM(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int registradorDestino_param, int enderecoMemoria_param){
	PALAVRA palavraParaGravar;
	int invadiuMemoria = MMU_sincronizado_lerLogico(&global_MMU, enderecoMemoria_param, &palavraParaGravar, 0);
	if(invadiuMemoria){
		kernel_rodar(&global_kernel, INTERRUPCAO_SEGMENTACAO_MEMORIA);
	} else {
		sprintf(mensagemInstrucao_param, "LDM %d %d %d %d %d", registradorDestino_param, 
			(((enderecoMemoria_param & 0xFF000000)/256)/256)/256,
			((enderecoMemoria_param & 0x00FF0000)/256)/256,
			(enderecoMemoria_param & 0x0000FF00)/256,
			enderecoMemoria_param & 0x000000FF);
		registrador_carregarPalavra(
			contexto_getRegistrador(&processador_param->contextoProcessador, registradorDestino_param),
			palavraParaGravar);
	}
}
void privada_LDC(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int registradorDestino_param, PALAVRA constante_param){
	sprintf(mensagemInstrucao_param, "LDC %d %d %d %d %d", registradorDestino_param, 
		(((constante_param & 0xFF000000)/256)/256)/256,
		((constante_param & 0x00FF0000)/256)/256,
		(constante_param & 0x0000FF00)/256,
		constante_param & 0x000000FF);
	registrador_carregarPalavra(
		contexto_getRegistrador(&processador_param->contextoProcessador, registradorDestino_param),
		constante_param);
}
void privada_WRM(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int registradorOrigem_param, int enderecoDestino_param){
	sprintf(mensagemInstrucao_param, "WRM %d %d %d %d %d", registradorOrigem_param, 
		(((enderecoDestino_param & 0xFF000000)/256)/256)/256,
		((enderecoDestino_param & 0x00FF0000)/256)/256,
		(enderecoDestino_param & 0x0000FF00)/256,
		enderecoDestino_param & 0x000000FF);

	PALAVRA conteudoOrigem = registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registradorOrigem_param));
	int invadiuMemoria = MMU_sincronizado_escreverLogico(&global_MMU, enderecoDestino_param, conteudoOrigem, 0);
	if(invadiuMemoria){
		kernel_rodar(&global_kernel, INTERRUPCAO_SEGMENTACAO_MEMORIA);
	}
}
void privada_LI(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int registradorDestino_param, int registradorOrigem_param){
	sprintf(mensagemInstrucao_param, "LI %d %d", registradorDestino_param, registradorOrigem_param);

	PALAVRA conteudoDestino = registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registradorOrigem_param));
	int enderecoDestino;
	int invadiuMemoria = MMU_sincronizado_lerLogico(&global_MMU, conteudoDestino, &enderecoDestino, 0);
	if(invadiuMemoria){
		kernel_rodar(&global_kernel, INTERRUPCAO_SEGMENTACAO_MEMORIA);
	} else {
		PALAVRA conteudoOrigem;
		invadiuMemoria = MMU_sincronizado_lerLogico(&global_MMU, enderecoDestino, &conteudoOrigem, 0);
		if(invadiuMemoria){
			kernel_rodar(&global_kernel, INTERRUPCAO_SEGMENTACAO_MEMORIA);
		} else {
			registrador_carregarPalavra(
				contexto_getRegistrador(&processador_param->contextoProcessador, registradorDestino_param),
				conteudoOrigem);
		}
	}
}
void privada_WI(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int registradorOrigem_param, int registradorDestino_param){
	sprintf(mensagemInstrucao_param, "WI %d %d", registradorOrigem_param, registradorDestino_param);

	PALAVRA conteudoOrigem = registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registradorOrigem_param));
	PALAVRA conteudoDestino = registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registradorDestino_param));
	int invadiuMemoria = MMU_sincronizado_escreverLogico(&global_MMU, conteudoDestino, conteudoOrigem, 0);
	if(invadiuMemoria){
		kernel_rodar(&global_kernel, INTERRUPCAO_SEGMENTACAO_MEMORIA);
	}
}
void privada_JCA(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int enderecoDestino_param){
	sprintf(mensagemInstrucao_param, "JCA %d %d %d %d", 
		(((enderecoDestino_param & 0xFF000000)/256)/256)/256,
		((enderecoDestino_param & 0x00FF0000)/256)/256,
		(enderecoDestino_param & 0x0000FF00)/256,
		enderecoDestino_param & 0x000000FF);

	contexto_setPC(&processador_param->contextoProcessador, enderecoDestino_param);
}
void privada_JCZ(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int enderecoDestino_param){
	sprintf(mensagemInstrucao_param, "JCZ %d %d %d %d", 
		(((enderecoDestino_param & 0xFF000000)/256)/256)/256,
		((enderecoDestino_param & 0x00FF0000)/256)/256,
		(enderecoDestino_param & 0x0000FF00)/256,
		enderecoDestino_param & 0x000000FF);

	if(processador_param->Z == 1){
		contexto_setPC(&processador_param->contextoProcessador, enderecoDestino_param);
	}
	contexto_setPC(&processador_param->contextoProcessador, contexto_getPC(&processador_param->contextoProcessador)+1);
}
void privada_JCE(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int enderecoDestino_param){
	sprintf(mensagemInstrucao_param, "JCE %d %d %d %d", 
		(((enderecoDestino_param & 0xFF000000)/256)/256)/256,
		((enderecoDestino_param & 0x00FF0000)/256)/256,
		(enderecoDestino_param & 0x0000FF00)/256,
		enderecoDestino_param & 0x000000FF);

	if(processador_param->E == 1){
		contexto_setPC(&processador_param->contextoProcessador, enderecoDestino_param);
	}
	contexto_setPC(&processador_param->contextoProcessador, contexto_getPC(&processador_param->contextoProcessador)+1);
}
void privada_JCL(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int enderecoDestino_param){
	sprintf(mensagemInstrucao_param, "JCL %d %d %d %d", 
		(((enderecoDestino_param & 0xFF000000)/256)/256)/256,
		((enderecoDestino_param & 0x00FF0000)/256)/256,
		(enderecoDestino_param & 0x0000FF00)/256,
		enderecoDestino_param & 0x000000FF);

	if(processador_param->L == 1){
		contexto_setPC(&processador_param->contextoProcessador, enderecoDestino_param);
	}
	contexto_setPC(&processador_param->contextoProcessador, contexto_getPC(&processador_param->contextoProcessador)+1);
}
void privada_JRA(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int registradorDestino_param){
	sprintf(mensagemInstrucao_param, "JRA %d", registradorDestino_param);

	PALAVRA conteudoDestino = registrador_lerPalavra(
		contexto_getRegistrador(&processador_param->contextoProcessador, registradorDestino_param));
	contexto_setPC(&processador_param->contextoProcessador, conteudoDestino);
}
void privada_JRZ(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int registradorDestino_param){
	sprintf(mensagemInstrucao_param, "JRZ %d", registradorDestino_param);

	if(processador_param->Z == 1){
		PALAVRA conteudoDestino = registrador_lerPalavra(
			contexto_getRegistrador(&processador_param->contextoProcessador, registradorDestino_param));
		contexto_setPC(&processador_param->contextoProcessador, conteudoDestino);
	}
}
void privada_JRE(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int registradorDestino_param){
	sprintf(mensagemInstrucao_param, "JRE %d", registradorDestino_param);

	if(processador_param->E == 1){
		PALAVRA conteudoDestino = registrador_lerPalavra(
			contexto_getRegistrador(&processador_param->contextoProcessador, registradorDestino_param));
		contexto_setPC(&processador_param->contextoProcessador, conteudoDestino);
	}
}
void privada_JRL(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int registradorDestino_param){
	sprintf(mensagemInstrucao_param, "JRL %d", registradorDestino_param);

	if(processador_param->L == 1){
		PALAVRA conteudoDestino = registrador_lerPalavra(
			contexto_getRegistrador(&processador_param->contextoProcessador, registradorDestino_param));
		contexto_setPC(&processador_param->contextoProcessador, conteudoDestino);
	}
}
void privada_CALL(PROCESSADOR *processador_param, char* mensagemInstrucao_param, PALAVRA enderecoDestino_param){
	sprintf(mensagemInstrucao_param, "CALL %d", enderecoDestino_param);

	registrador_somar(contexto_getRegistrador(&processador_param->contextoProcessador, REGISTRADOR_STACK_POINTER), 1);
	int posicaoApontadaStack = 
		registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, REGISTRADOR_STACK_POINTER));
	int tamanhoStack = privada_getTamanhoStackProcessoRodando(processador_param);

	if(tamanhoStack < posicaoApontadaStack){
		kernel_rodar(&global_kernel, INTERRUPCAO_ESTOURO_PILHA);
	} else {
		int base;
		MMU_sincronizado_getBase(&global_MMU, &base);
		MMU_sincronizado_escreverFisico(&global_MMU, base-(posicaoApontadaStack+1), contexto_getPC(&processador_param->contextoProcessador)+1);
		contexto_setPC(&processador_param->contextoProcessador, enderecoDestino_param);
	}
}
void privada_RETC(PROCESSADOR *processador_param, char* mensagemInstrucao_param){
	sprintf(mensagemInstrucao_param, "RETC");

	int posicaoApontadaStack = 
		registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, REGISTRADOR_STACK_POINTER));

	if(posicaoApontadaStack < 1){
		kernel_rodar(&global_kernel, INTERRUPCAO_ESTOURO_PILHA);
	} else {
		int base;
		int enderecoRetorno;
		MMU_sincronizado_getBase(&global_MMU, &base);
		MMU_sincronizado_lerFisico(&global_MMU, base-(posicaoApontadaStack+1), &enderecoRetorno);
		registrador_somar(contexto_getRegistrador(&processador_param->contextoProcessador, REGISTRADOR_STACK_POINTER), -1);
		contexto_setPC(&processador_param->contextoProcessador, enderecoRetorno);
	}
}
void privada_PUS(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int registradorOrigem_param){
	sprintf(mensagemInstrucao_param, "PUS %d", registradorOrigem_param);

	registrador_somar(contexto_getRegistrador(&processador_param->contextoProcessador, REGISTRADOR_STACK_POINTER), 1);
	int posicaoApontadaStack = 
		registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, REGISTRADOR_STACK_POINTER));
	int tamanhoStack = privada_getTamanhoStackProcessoRodando(processador_param);

	if(tamanhoStack < posicaoApontadaStack){
		kernel_rodar(&global_kernel, INTERRUPCAO_ESTOURO_PILHA);
	} else {
		int base;
		int conteudoGravado = 
			registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, registradorOrigem_param));
		MMU_sincronizado_getBase(&global_MMU, &base);
		MMU_sincronizado_escreverFisico(&global_MMU, base-(posicaoApontadaStack+1), conteudoGravado);
	}
}
void privada_POP(PROCESSADOR *processador_param, char* mensagemInstrucao_param, int registradorDestino_param){
	sprintf(mensagemInstrucao_param, "POP %d", registradorDestino_param);

	int posicaoApontadaStack = 
		registrador_lerPalavra(contexto_getRegistrador(&processador_param->contextoProcessador, REGISTRADOR_STACK_POINTER));

	if(posicaoApontadaStack < 1){
		kernel_rodar(&global_kernel, INTERRUPCAO_ESTOURO_PILHA);
	} else {
		int base;
		int conteudoGravado;
		MMU_sincronizado_getBase(&global_MMU, &base);
		MMU_sincronizado_lerFisico(&global_MMU, base-(posicaoApontadaStack+1), &conteudoGravado);
		registrador_carregarPalavra(
			contexto_getRegistrador(&processador_param->contextoProcessador, registradorDestino_param),
			conteudoGravado);
		registrador_somar(contexto_getRegistrador(&processador_param->contextoProcessador, REGISTRADOR_STACK_POINTER), -1);
	}
}


/**
* Executa uma instrução.
* @param PROCESSADOR	*processador_param	O processador que está executando a instrução.
* @param INSTRUCAO		instrucao_param		A instrução que será executada.
*/
void privada_executaInstrucao(PROCESSADOR *processador_param, INSTRUCAO instrucao_param){
	char mensagem[200];
	int invadiuMemoria=0;
	int enderecoMemoria, enderecoDestino, qualRegistrador, registradorOrigem, registradorDestino;
	PALAVRA palavraLida, palavraGravada;

	switch(instrucao_param){
		case INSTRUCAO_ABSOLUTE_JUMP:
			privada_JPA(processador_param, mensagem, processador_param->IR.conteudo[3]);
			break;
		case INSTRUCAO_SOFTWARE_INTERRUPT:
			privada_INT(processador_param, mensagem, processador_param->IR.conteudo[3]);
			break;
		case INSTRUCAO_LOAD_REGISTER_FROM_MEMORY:
			qualRegistrador = processador_param->IR.conteudo[2];
			enderecoMemoria = processador_param->IR.conteudo[3];
			if(!(0 <= qualRegistrador && qualRegistrador < QUANTIDADE_REGISTRADORES_CONTEXTO)){
				kernel_rodar(&global_kernel, INTERRUPCAO_PROCESSADOR);
			} else {
				privada_LM(processador_param, mensagem, qualRegistrador, enderecoMemoria);
			}
			break;
		case INSTRUCAO_LOAD_REGISTER_FROM_CONSTANT:
			qualRegistrador = processador_param->IR.conteudo[2];
			palavraGravada = processador_param->IR.conteudo[3];
			if(!(0 <= qualRegistrador && qualRegistrador < QUANTIDADE_REGISTRADORES_CONTEXTO)){
				kernel_rodar(&global_kernel, INTERRUPCAO_PROCESSADOR);
			} else {
				privada_LC(processador_param, mensagem, qualRegistrador, palavraGravada);
			}
			break;
		case INSTRUCAO_WRITE_REGISTER_IN_MEMORY:
			qualRegistrador = processador_param->IR.conteudo[2];
			enderecoDestino = processador_param->IR.conteudo[3];
			if(!(0 <= qualRegistrador && qualRegistrador < QUANTIDADE_REGISTRADORES_CONTEXTO)){
				kernel_rodar(&global_kernel, INTERRUPCAO_PROCESSADOR);
			} else {
				privada_WM(processador_param, mensagem, qualRegistrador, enderecoDestino);
			}
			break;
		case INSTRUCAO_SUBTRACT_REGISTERS:
			registradorDestino = processador_param->IR.conteudo[2];
			registradorOrigem = processador_param->IR.conteudo[3];
			if(!((0 <= registradorDestino && registradorDestino < QUANTIDADE_REGISTRADORES_CONTEXTO)
					&& (0 <= registradorOrigem && registradorOrigem < QUANTIDADE_REGISTRADORES_CONTEXTO))){
				kernel_rodar(&global_kernel, INTERRUPCAO_PROCESSADOR);
			} else {
				privada_SU(processador_param, mensagem, registradorDestino, registradorOrigem);
			}
			break;
		case INSTRUCAO_ADD_REGISTERS:
			registradorDestino = processador_param->IR.conteudo[2];
			registradorOrigem = processador_param->IR.conteudo[3];
			if(!((0 <= registradorDestino && registradorDestino < QUANTIDADE_REGISTRADORES_CONTEXTO)
					&& (0 <= registradorOrigem && registradorOrigem < QUANTIDADE_REGISTRADORES_CONTEXTO))){
				kernel_rodar(&global_kernel, INTERRUPCAO_PROCESSADOR);
			} else {
				privada_AD(processador_param, mensagem, registradorDestino, registradorOrigem);
			}
			break;
		case INSTRUCAO_DECREMENT_REGISTER:
			qualRegistrador = processador_param->IR.conteudo[3];
			if(!(0 <= qualRegistrador && qualRegistrador < QUANTIDADE_REGISTRADORES_CONTEXTO)){
				kernel_rodar(&global_kernel, INTERRUPCAO_PROCESSADOR);
			} else {
				privada_DEC(processador_param, mensagem, qualRegistrador);
			}
			break;
		case INSTRUCAO_INCREMENT_REGISTER:
			qualRegistrador = processador_param->IR.conteudo[3];
			if(!(0 <= qualRegistrador && qualRegistrador < QUANTIDADE_REGISTRADORES_CONTEXTO)){
				kernel_rodar(&global_kernel, INTERRUPCAO_PROCESSADOR);
			} else {
				privada_INC(processador_param, mensagem, qualRegistrador);
			}
			break;
		case INSTRUCAO_COMPARE_REGISTERS:
			registradorOrigem = processador_param->IR.conteudo[2];
			registradorDestino = processador_param->IR.conteudo[3];
			if(!((0 <= registradorDestino && registradorDestino < QUANTIDADE_REGISTRADORES_CONTEXTO)
					&& (0 <= registradorOrigem && registradorOrigem < QUANTIDADE_REGISTRADORES_CONTEXTO))){
				kernel_rodar(&global_kernel, INTERRUPCAO_PROCESSADOR);
			} else {
				privada_CP(processador_param, mensagem, registradorOrigem, registradorDestino);
			}
			break;
		case INSTRUCAO_JUMP_ON_ZERO:
			enderecoDestino = processador_param->IR.conteudo[3];
			privada_JPZ(processador_param, mensagem, enderecoDestino);
			break;
		case INSTRUCAO_JUMP_ON_EQUAL:
			enderecoDestino = processador_param->IR.conteudo[3];
			privada_JPE(processador_param, mensagem, enderecoDestino);
			break;
		case INSTRUCAO_JUMP_ON_LESS:
			enderecoDestino = processador_param->IR.conteudo[3];
			privada_JPL(processador_param, mensagem, enderecoDestino);
			break;
		case INSTRUCAO_NOP:
			privada_NOP(processador_param, mensagem);
			break;
		case INSTRUCAO_LOAD_REGISTER_FROM_MEMORY_32:
			registradorDestino = processador_param->IR.conteudo[3];
			invadiuMemoria = MMU_sincronizado_lerLogico(&global_MMU, contexto_getPC(&processador_param->contextoProcessador), &palavraLida, 0);
			if(invadiuMemoria){
				kernel_rodar(&global_kernel, INTERRUPCAO_SEGMENTACAO_MEMORIA);
			} else if(!(0 <= registradorDestino && registradorDestino < QUANTIDADE_REGISTRADORES_CONTEXTO)){
				kernel_rodar(&global_kernel, INTERRUPCAO_PROCESSADOR);
			} else {
				privada_LDM(processador_param, mensagem, registradorDestino, palavraLida);
			}
			contexto_setPC(&processador_param->contextoProcessador, contexto_getPC(&processador_param->contextoProcessador)+1);
			break;
		case INSTRUCAO_LOAD_REGISTER_FROM_CONSTANT_32:
			registradorDestino = processador_param->IR.conteudo[3];
			invadiuMemoria = MMU_sincronizado_lerLogico(&global_MMU, contexto_getPC(&processador_param->contextoProcessador), &palavraGravada, 0);
			if(invadiuMemoria){
				kernel_rodar(&global_kernel, INTERRUPCAO_SEGMENTACAO_MEMORIA);
			} else if(!(0 <= registradorDestino && registradorDestino < QUANTIDADE_REGISTRADORES_CONTEXTO)){
				kernel_rodar(&global_kernel, INTERRUPCAO_PROCESSADOR);
			} else {
				privada_LDC(processador_param, mensagem, registradorDestino, palavraGravada);
			}
			contexto_setPC(&processador_param->contextoProcessador, contexto_getPC(&processador_param->contextoProcessador)+1);
			break;
		case INSTRUCAO_WRITE_REGISTER_IN_MEMORY_32:
			registradorOrigem = processador_param->IR.conteudo[3];
			invadiuMemoria = MMU_sincronizado_lerLogico(&global_MMU, contexto_getPC(&processador_param->contextoProcessador), &enderecoDestino, 0);
			if(invadiuMemoria){
				kernel_rodar(&global_kernel, INTERRUPCAO_SEGMENTACAO_MEMORIA);
			} else if(!(0 <= registradorOrigem && registradorOrigem < QUANTIDADE_REGISTRADORES_CONTEXTO)){
				kernel_rodar(&global_kernel, INTERRUPCAO_PROCESSADOR);
			} else {
				privada_WRM(processador_param, mensagem, registradorOrigem, enderecoDestino);
			}
			contexto_setPC(&processador_param->contextoProcessador, contexto_getPC(&processador_param->contextoProcessador)+1);
			break;
		case INSTRUCAO_LOAD_REGISTER_INDIRECTLY:
			registradorDestino = processador_param->IR.conteudo[2];
			registradorOrigem = processador_param->IR.conteudo[3];
			if(!((0 <= registradorDestino && registradorDestino < QUANTIDADE_REGISTRADORES_CONTEXTO)
					&& (0 <= registradorOrigem && registradorOrigem < QUANTIDADE_REGISTRADORES_CONTEXTO))){
				kernel_rodar(&global_kernel, INTERRUPCAO_PROCESSADOR);
			} else {
				privada_LI(processador_param, mensagem, registradorDestino, registradorOrigem);
			}
			break;
		case INSTRUCAO_WRITE_REGISTER_INDIRECTLY:
			registradorOrigem = processador_param->IR.conteudo[2];
			registradorDestino = processador_param->IR.conteudo[3];
			if(!((0 <= registradorDestino && registradorDestino < QUANTIDADE_REGISTRADORES_CONTEXTO)
					&& (0 <= registradorOrigem && registradorOrigem < QUANTIDADE_REGISTRADORES_CONTEXTO))){
				kernel_rodar(&global_kernel, INTERRUPCAO_PROCESSADOR);
			} else {
				privada_WI(processador_param, mensagem, registradorOrigem, registradorDestino);
			}
			break;
		case INSTRUCAO_ABSOLUTE_JUMP_32:
			invadiuMemoria = MMU_sincronizado_lerLogico(&global_MMU, contexto_getPC(&processador_param->contextoProcessador), &enderecoDestino, 0);
			if(invadiuMemoria){
				kernel_rodar(&global_kernel, INTERRUPCAO_SEGMENTACAO_MEMORIA);
			} else {
				privada_JCA(processador_param, mensagem, enderecoDestino);
			}
			break;
		case INSTRUCAO_JUMP_ON_ZERO_32:
			invadiuMemoria = MMU_sincronizado_lerLogico(&global_MMU, contexto_getPC(&processador_param->contextoProcessador), &enderecoDestino, 0);
			if(invadiuMemoria){
				kernel_rodar(&global_kernel, INTERRUPCAO_SEGMENTACAO_MEMORIA);
			} else {
				privada_JCZ(processador_param, mensagem, enderecoDestino);
			}
			break;
		case INSTRUCAO_JUMP_ON_EQUAL_32:
			invadiuMemoria = MMU_sincronizado_lerLogico(&global_MMU, contexto_getPC(&processador_param->contextoProcessador), &enderecoDestino, 0);
			if(invadiuMemoria){
				kernel_rodar(&global_kernel, INTERRUPCAO_SEGMENTACAO_MEMORIA);
			} else {
				privada_JCE(processador_param, mensagem, enderecoDestino);
			}
			break;
		case INSTRUCAO_JUMP_ON_LESS_32:
			invadiuMemoria = MMU_sincronizado_lerLogico(&global_MMU, contexto_getPC(&processador_param->contextoProcessador), &enderecoDestino, 0);
			if(invadiuMemoria){
				kernel_rodar(&global_kernel, INTERRUPCAO_SEGMENTACAO_MEMORIA);
			} else {
				privada_JCL(processador_param, mensagem, enderecoDestino);
			}
			break;
		case INSTRUCAO_ABSOLUTE_JUMP_TO_REGISTER:
			registradorDestino = processador_param->IR.conteudo[3];
			if(!(0 <= registradorDestino && registradorDestino < QUANTIDADE_REGISTRADORES_CONTEXTO)){
				kernel_rodar(&global_kernel, INTERRUPCAO_PROCESSADOR);
			} else {
				privada_JRA(processador_param, mensagem, registradorDestino);
			}
			break;
		case INSTRUCAO_JUMP_ON_ZERO_TO_REGISTER:
			registradorDestino = processador_param->IR.conteudo[3];
			if(!(0 <= registradorDestino && registradorDestino < QUANTIDADE_REGISTRADORES_CONTEXTO)){
				kernel_rodar(&global_kernel, INTERRUPCAO_PROCESSADOR);
			} else {
				privada_JRZ(processador_param, mensagem, registradorDestino);
			}
			break;
		case INSTRUCAO_JUMP_ON_EQUAL_TO_REGISTER:
			registradorDestino = processador_param->IR.conteudo[3];
			if(!(0 <= registradorDestino && registradorDestino < QUANTIDADE_REGISTRADORES_CONTEXTO)){
				kernel_rodar(&global_kernel, INTERRUPCAO_PROCESSADOR);
			} else {
				privada_JRE(processador_param, mensagem, registradorDestino);
			}
			break;
		case INSTRUCAO_JUMP_ON_LESS_TO_REGISTER:
			registradorDestino = processador_param->IR.conteudo[3];
			if(!(0 <= registradorDestino && registradorDestino < QUANTIDADE_REGISTRADORES_CONTEXTO)){
				kernel_rodar(&global_kernel, INTERRUPCAO_PROCESSADOR);
			} else {
				privada_JRL(processador_param, mensagem, registradorDestino);
			}
			break;
		case INSTRUCAO_CALL_ROUTINE:
			invadiuMemoria = MMU_sincronizado_lerLogico(&global_MMU, contexto_getPC(&processador_param->contextoProcessador), 
				&enderecoDestino, 0);
			if(invadiuMemoria){
				kernel_rodar(&global_kernel, INTERRUPCAO_SEGMENTACAO_MEMORIA);
			} else {
				privada_CALL(processador_param, mensagem, enderecoDestino);
			}
			break;
		case INSTRUCAO_RETURN_FROM_CALL:
			privada_RETC(processador_param, mensagem);
			break;
		case INSTRUCAO_PUSH_REGISTER_INTO_STACK:
			registradorOrigem = processador_param->IR.conteudo[3];
			if(!(0 <= registradorOrigem && registradorOrigem < QUANTIDADE_REGISTRADORES_CONTEXTO)){
				kernel_rodar(&global_kernel, INTERRUPCAO_PROCESSADOR);
			} else {
				privada_PUS(processador_param, mensagem, registradorOrigem);
			}
			break;
		case INSTRUCAO_POP_REGISTER_FROM_STACK:
			registradorDestino = processador_param->IR.conteudo[3];
			if(!(0 <= registradorDestino && registradorDestino < QUANTIDADE_REGISTRADORES_CONTEXTO)){
				kernel_rodar(&global_kernel, INTERRUPCAO_PROCESSADOR);
			} else {
				privada_POP(processador_param, mensagem, registradorDestino);
			}
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
	int houveAcessoIlegal;

	while(1){
		usleep(1000*1000/10);
		sprintf(mensagem, "PROCESSADOR: PC=%d IR=%d %d %d %d", 	contexto_getPC(&processador_param->contextoProcessador),
			processador_param->IR.conteudo[0], processador_param->IR.conteudo[1], processador_param->IR.conteudo[2],
			processador_param->IR.conteudo[3]);
		tela_escreverNaColuna(&global_tela, mensagem, 1);

		houveAcessoIlegal=0;
		instrucaoLida = privada_buscaInstrucao(processador_param, &global_MMU, &houveAcessoIlegal);
		if(!houveAcessoIlegal){
			contexto_setPC(&processador_param->contextoProcessador, contexto_getPC(&processador_param->contextoProcessador)+1);
			registrador_carregarPalavra(&processador_param->IR, instrucaoLida);
			instrucaoBuscada = privada_decodificaInstrucao(processador_param);
			privada_executaInstrucao(processador_param, instrucaoBuscada);
		}

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

