/**
* Concentra as funções de manipulação de memória.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes

struct str_alocacaoMemoria{
	int enderecoInicio; //Início do programa na memória.
	int tamanhoAreaMemoriaPalavras; //Tamanho da área de memória reservada ao programa, em palavras.
};

typedef struct str_alocacaoMemoria ALOCACAO_MEMORIA;

struct str_mapaMemoria{
	int maximoAlocacoes; //Define o máximo de alocações que podem ser feitas neste mapa.
	MMU *memoriaRepresentada; //A memória que o mapa representa.
	FIFO filaMemoriaAlocada; //Representa o que foi alocado de memória, com elementos ALOCACAO_MEMORIA.
};

typedef struct str_mapaMemoria MAPA_ALOCACOES_MEMORIA;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* @param MAPA_ALOCACOES_MEMORIA	*mapa_param				O mapa que será inicializado.
* @param MMU					*MMU_param				A memória que o mapa representa.
* @param int					maximoAlocacoes_param	O número máximo de alocações que podem ser feitas neste mapa.
*/
void mapaAlocacoesMemoria_inicializar(MAPA_ALOCACOES_MEMORIA *mapa_param, MMU *MMU_param, int maximoAlocacoes_param);

/**
* @param MAPA_ALOCACOES_MEMORIA	*mapa_param						O mapa em que a operação será realizada.
* @param int					tamanhoAlocacaoPalavras_param	O tamanho da alocação, em palavras.
* @return int	O endereço inicial do bloco alocado. Caso não seja possível alocar, retornará MEMORIA_ENDERECO_INEXISTENTE.
*/
int mapaAlocacoesMemoria_alocar(MAPA_ALOCACOES_MEMORIA *mapa_param, int tamanhoAlocacaoPalavras_param);

/**
* @param MAPA_ALOCACOES_MEMORIA	*mapa_param				O mapa em que a operação será realizada.
* @param int					enderecoInicial_param	O endereço que inicia o bloco que será liberado.
*/
void mapaAlocacoesMemoria_liberar(MAPA_ALOCACOES_MEMORIA *mapa_param, int enderecoInicial_param);















