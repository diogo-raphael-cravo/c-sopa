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
* @param GERENCIADOR_DISCO	*gerenciado_param	O gerenciador que será inicializado.
* @param DISCO 				*disco_param		O disco gerenciado.
*/
void gerenciadorDisco_inicializar(GERENCIADOR_DISCO *gerenciador_param, DISCO *disco_param){
	gerenciador_param->discoGerenciado = disco_param;
	gerenciador_param->proximaOperacaoKernel = NULL;
	gerenciador_param->proximaOperacaoKernelConsultada = 1;
	FIFO_inicializar(&gerenciador_param->filaOperacoesDisco, TAMANHO_DISCO_PALAVRAS);
	FIFO_inicializar(&gerenciador_param->filaOperacoesKernel, TAMANHO_DISCO_PALAVRAS);
}

/**
* @param GERENCIADOR_DISCO		*gerenciador_param			O gerenciador que irá fazer o agendamento.
* @param OPERACAO_DISCO*		operacaoDisco_param			A operação do disco que irá esperar para executar.
* @param OPERACAO_KERNEL*		operacaoKernel_param		A operação do kernel 
* ATENÇÃO: se o disco estiver livre, o mandará executar a próxima direto.
*/
void gerenciadorDisco_agendar(GERENCIADOR_DISCO *gerenciador_param, OPERACAO_DISCO* operacaoDisco_param, 
		OPERACAO_KERNEL* operacaoKernel_param){
	FIFO_inserir(&gerenciador_param->filaOperacoesDisco, operacaoDisco_param);
	FIFO_inserir(&gerenciador_param->filaOperacoesKernel, operacaoKernel_param);
	if(!disco_estahOcupado(gerenciador_param->discoGerenciado)){
		gerenciadorDisco_executarProxima(gerenciador_param);
	}
	tela_escreverNaColuna(&global_tela, "Acesso a disco agendado.", 3);
}

/**
* @param	GERENCIADOR_DISCO		*gerenciador_param	O gerenciador que irá mandar 
*														e que contém o disco que realizará a operação.
* ATENÇÃO: não permitirá execução duas vezes seguidas sem que a próxima operação do kernel seja consultada com 
* gerenciadorDisco_proximaOperacaoKernel!
*/
void gerenciadorDisco_executarProxima(GERENCIADOR_DISCO *gerenciador_param){
	if(!FIFO_vazia(&gerenciador_param->filaOperacoesDisco) && !FIFO_vazia(&gerenciador_param->filaOperacoesKernel)
			&& !disco_estahOcupado(gerenciador_param->discoGerenciado)
			&& gerenciador_param->proximaOperacaoKernelConsultada == 1){
		OPERACAO_DISCO* proximaOperacaoDisco = (OPERACAO_DISCO*) FIFO_remover(&gerenciador_param->filaOperacoesDisco);
		gerenciador_param->proximaOperacaoKernel = (OPERACAO_KERNEL*) FIFO_remover(&gerenciador_param->filaOperacoesKernel);

		gerenciador_param->proximaOperacaoKernelConsultada = 0;

		if(proximaOperacaoDisco->tipoOperacao == TIPO_OPERACAO_LEITURA_DISCO){
			OPERACAO_LEITURA_DISCO* operacaoExecutada = (OPERACAO_LEITURA_DISCO*) proximaOperacaoDisco->operacao;
			disco_lerEndereco(
				gerenciador_param->discoGerenciado, 
				operacaoExecutada->endereco);
		} else if(proximaOperacaoDisco->tipoOperacao == TIPO_OPERACAO_ESCRITA_DISCO){
			OPERACAO_ESCRITA_DISCO* operacaoExecutada = (OPERACAO_ESCRITA_DISCO*) proximaOperacaoDisco->operacao;
			disco_escreverEndereco(
				gerenciador_param->discoGerenciado, 
				operacaoExecutada->endereco, 
				operacaoExecutada->dados, 
				operacaoExecutada->tamanhoDados);
		} else if(proximaOperacaoDisco->tipoOperacao == TIPO_OPERACAO_CARGA_DMA_DISCO){
			OPERACAO_CARGA_DMA_DISCO* operacaoExecutada = (OPERACAO_CARGA_DMA_DISCO*) proximaOperacaoDisco->operacao;
			disco_transferirParaMemoria(
				gerenciador_param->discoGerenciado, 
				operacaoExecutada->enderecoMemoria, 
				operacaoExecutada->enderecoDisco, 
				operacaoExecutada->quantidadePalavras);
		} else {
			tela_imprimirTelaAzulDaMorte(&global_tela, "Bah, cara. Deu um problema na gerenciadorDisco_executarProxima. No faco ideia do que pode ser.");
		}

		free(proximaOperacaoDisco->operacao);
		free(proximaOperacaoDisco);
	} else {
		gerenciador_param->proximaOperacaoKernel = NULL;
	}
}

/**
* @param 	GERENCIADOR_DISCO		*gerenciador_param	O gerenciador que mandou executar uma operação 
* @return OPERACAO_KERNEL*	A operação do kernel que estava relacionada com a última operação do disco
*							que foi mandada executar. Retornará NULL, se não houver.
*/
OPERACAO_KERNEL* gerenciadorDisco_proximaOperacaoKernel(GERENCIADOR_DISCO *gerenciador_param){
	gerenciador_param->proximaOperacaoKernelConsultada = 1;
	return gerenciador_param->proximaOperacaoKernel;
}

	/**
	* Operações do KERNEL
	**/
/**
* @param DESCRITOR_PROCESSO**	descritorProcesso_param		O processo que será criado.
* @return OPERACAO_KERNEL*	A operação criada.
*/
OPERACAO_KERNEL* gerenciadorDisco_criarOperacaoKernelCriacaoProcesso(DESCRITOR_PROCESSO **descritorProcesso_param){
	OPERACAO_KERNEL* operacaoNova = (OPERACAO_KERNEL*) malloc(sizeof(OPERACAO_KERNEL));
	OPERACAO_CRIACAO_PROCESSO_KERNEL* operacaoGuardadaNova = (OPERACAO_CRIACAO_PROCESSO_KERNEL*) malloc(sizeof(OPERACAO_CRIACAO_PROCESSO_KERNEL));

	operacaoGuardadaNova->processoCriado = descritorProcesso_param;

	operacaoNova->operacao = operacaoGuardadaNova;
	operacaoNova->tipoOperacao = TIPO_OPERACAO_AGENDAVEL_CRIACAO_PROCESSO_KERNEL;

	return operacaoNova;
}

/**
* @param DESCRITOR_PROCESSO**	descritorProcesso_param		O processo que será criado.
* @param TIPO_OPERACAO_DISCO	tipoOperacao_param			O tipo da operação que o processo requisitou.
* @return OPERACAO_KERNEL*	A operação criada.
*/
OPERACAO_KERNEL* gerenciadorDisco_criarOperacaoKernelFeitaPorProcesso(DESCRITOR_PROCESSO** descritorProcesso_param, 
		TIPO_OPERACAO_DISCO tipoOperacao_param){
	OPERACAO_KERNEL* operacaoNova = (OPERACAO_KERNEL*) malloc(sizeof(OPERACAO_KERNEL));
	OPERACAO_FEITA_POR_PROCESSO_KERNEL* operacaoGuardadaNova = (OPERACAO_FEITA_POR_PROCESSO_KERNEL*) malloc(sizeof(OPERACAO_FEITA_POR_PROCESSO_KERNEL));

	operacaoGuardadaNova->processoQuePediu = descritorProcesso_param;
	operacaoGuardadaNova->tipoOperacaoFeita = tipoOperacao_param;

	operacaoNova->operacao = operacaoGuardadaNova;
	operacaoNova->tipoOperacao = TIPO_OPERACAO_AGENDAVEL_FEITA_POR_PROCESSO_KERNEL;

	return operacaoNova;
}

	/**
	* Operações do DISCO
	**/
/**
* Agenda a operação, para ser feita depois de todas as que estão agendadas.
* @param int					endereco_param				O endereço da leitura.
* @return OPERACAO_DISCO* 	A operação criada.
*/
OPERACAO_DISCO* gerenciadorDisco_criarOperacaoDiscoLeitura(int endereco_param){
	OPERACAO_DISCO* operacaoNova = (OPERACAO_DISCO*) malloc(sizeof(OPERACAO_DISCO));
	OPERACAO_LEITURA_DISCO* operacaoGuardadaNova = (OPERACAO_LEITURA_DISCO*) malloc(sizeof(OPERACAO_LEITURA_DISCO));

	operacaoGuardadaNova->endereco = endereco_param;

	operacaoNova->operacao = operacaoGuardadaNova;
	operacaoNova->tipoOperacao = TIPO_OPERACAO_LEITURA_DISCO;

	return operacaoNova;
}

/**
* Agenda a operação, para ser feita depois de todas as que estão agendadas.
* @param int					endereco_param				O endereço da escrita.
* @param PALAVRA				*dados_param				Os dados que serão escritos.
* @param int					tamanhoDados_param			A quantidade de palavras que serão escritas.
* @return OPERACAO_DISCO* 	A operação criada.
*/
OPERACAO_DISCO* gerenciadorDisco_criarOperacaoDiscoEscrita(int endereco_param,
		PALAVRA *dados_param, int tamanhoDados_param){
	OPERACAO_DISCO* operacaoNova = (OPERACAO_DISCO*) malloc(sizeof(OPERACAO_DISCO));
	OPERACAO_ESCRITA_DISCO* operacaoGuardadaNova = (OPERACAO_ESCRITA_DISCO*) malloc(sizeof(OPERACAO_ESCRITA_DISCO));

	operacaoGuardadaNova->endereco = endereco_param;
	operacaoGuardadaNova->dados = (PALAVRA*) malloc(tamanhoDados_param*sizeof(PALAVRA));
	int i=0;
	for(; i<tamanhoDados_param; i++){
		operacaoGuardadaNova->dados[i] = dados_param[i];
	}
	operacaoGuardadaNova->tamanhoDados = tamanhoDados_param;

	operacaoNova->operacao = operacaoGuardadaNova;
	operacaoNova->tipoOperacao = TIPO_OPERACAO_ESCRITA_DISCO;

	return operacaoNova;
}

/**
* Agenda a operação, para ser feita depois de todas as que estão agendadas.
* @param int					enderecoMemoria_param		Endereço em que a escrita será iniciada na memória.
* @param int					enderecoDisco_param			O endereço do disco à partir do qual os dados serão transferidos.
* @param int					quantidadePalavras_param	A quantidade de palavras que serão lidas.
* @return OPERACAO_DISCO* 	A operação criada.
*/
OPERACAO_DISCO* gerenciadorDisco_criarOperacaoDiscoCargaDMA(int enderecoMemoria_param,
		int enderecoDisco_param, int quantidadePalavras_param){
	OPERACAO_DISCO* operacaoNova = (OPERACAO_DISCO*) malloc(sizeof(OPERACAO_DISCO));
	OPERACAO_CARGA_DMA_DISCO* operacaoGuardadaNova = (OPERACAO_CARGA_DMA_DISCO*) malloc(sizeof(OPERACAO_CARGA_DMA_DISCO));

	operacaoGuardadaNova->enderecoMemoria = enderecoMemoria_param;
	operacaoGuardadaNova->enderecoDisco = enderecoDisco_param;
	operacaoGuardadaNova->quantidadePalavras = quantidadePalavras_param;

	operacaoNova->operacao = operacaoGuardadaNova;
	operacaoNova->tipoOperacao = TIPO_OPERACAO_CARGA_DMA_DISCO;

	return operacaoNova;
}









