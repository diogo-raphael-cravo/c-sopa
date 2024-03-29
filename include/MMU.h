/**
* Concentra as funções de manipulação de memória.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes


struct str_MMU{
	MEMORIA *memoriaGerenciada; //Memória gerenciada por esta MMU.	
	int base; //Base a ser adicionada para encontrar o endereço físico.
	int limite; //Especificado no endereço lógico.
	sem_t mutexAcessoMMU; //Garante que somente um processo usará a MMU por vez.
};

typedef struct str_MMU MMU;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* Inicializa a MMU.
* @param MMU		*MMU_param					A MMU que será inicializada.
* @param MEMORIA	*memoriaGerenciada_param	A memória que será gerenciada pela MMU.
*/
void MMU_sincronizado_inicializar(MMU *MMU_param, MEMORIA *memoriaGerenciada_param);

/**
* Define o registrador base, que é usado para converter endereços para endereços físicos.
* @param MMU		*MMU_param					A MMU em que a operação será realizada.
* @param int		base_param					O valor base.
*/
void MMU_sincronizado_setBase(MMU *MMU_param, int base_param);

/**
* Define o registrador base, que é usado para converter endereços para endereços físicos.
* @param MMU		*MMU_param					A MMU em que a operação será realizada.
* @param int		*destino_param				Onde será colocado o valor base.
*/
void MMU_sincronizado_getBase(MMU *MMU_param, int *destino_param);

/**
* Define o registrador limite, que é utilizado em endereços lógicos.
* @param MMU		*MMU_param					A MMU em que a operação será realizada.
* @param int		limite_param				O valor do limite.
*/
void MMU_sincronizado_setLimite(MMU *MMU_param, int limite_param);

/**
* Le a memória com endereçamento físico e sem consistência de limites.
* @param MMU		*MMU_param					A MMU em que a operação será realizada.
* @param int		endereco_param				Endereço da leitura.
* @param PALAVRA	*destino_param				Endereço da variável em que o resultado será colocado.
*/
void MMU_sincronizado_lerFisico(MMU *MMU_param, int endereco_param, PALAVRA *destino_param);

/*
* Escreve na memória com endereçamento físico e sem consistência de limites.
* @param MMU		*MMU_param					A MMU em que a operação será realizada.
* @param int		endereco_param				Endereço da escrita.
* @param PALAVRA	palavraEscrita_param		A palavra que será escrita na memória.
*/
void MMU_sincronizado_escreverFisico(MMU *MMU_param, int endereco_param, PALAVRA palavraEscrita_param);

/**
* Le a memória com endereçamento lógico e consistência de limites.
* @param MMU		*MMU_param					A MMU em que a operação será realizada.
* @param int		endereco_param				Endereço da leitura.
* @param PALAVRA	*destino_param				Endereço da variável em que o resultado será colocado.
* @param int		deveGerarInterrupcao_param	Indica se deve cadastrar uma interrupção no controlador de interrupções.
* @return int	Indica se houve invasão da memória.
*/
int MMU_sincronizado_lerLogico(MMU *MMU_param, int endereco_param, PALAVRA *destino_param, int deveGerarInterrupcao_param);

/*
* Escreve na memória com endereçamento lógico e consistência de limites.
* @param MMU		*MMU_param					A MMU em que a operação será realizada.
* @param int		endereco_param				Endereço da escrita.
* @param PALAVRA	palavraEscrita_param		A palavra que será escrita na memória.
* @param int		deveGerarInterrupcao_param	Indica se deve cadastrar uma interrupção no controlador de interrupções.
* @return int	Indica se houve invasão da memória.
*/
int MMU_sincronizado_escreverLogico(MMU *MMU_param, int endereco_param, PALAVRA palavraEscrita_param, int deveGerarInterrupcao_param);

/**
* Indica se a memória tem posições físicas suficientes para gravar um bloco que começa no endereço dado
* e tem o número de palavras dado.
* @param MMU		*MMU_param					A MMU em que a operação será realizada.
* @param int		enderecoInicial_param		O endereço que deverá iniciar o bloco.
* @param int		tamanhoBlocoPalavras_param	O tamanho que o bloco deve ter em palavras.
* @return int	Indica se a memória tem posições físicas suficientes para gravar o bloco. 
* ATENÇÃO: Não considera alocação! Somente EXISTÊNCIA de células físicas!
*/
int MMU_existeEspacoFisicoParaBloco(MMU *MMU_param, int enderecoInicial_param, int tamanhoBlocoPalavras_param);

















