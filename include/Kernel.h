/**
* Concentra as funções de manipulação do kernel.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------
struct str_kernel{
	int seila;
};

typedef struct str_kernel KERNEL;

//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* @param KERNEL			*kernel_param		O kernel que irá rodar.
* @param INTERRUPCAO	interrupcao_param	A interrupção que definirá o comportamento do kernel.
*/
void kernel_rodar(KERNEL *kernel_param, INTERRUPCAO interrupcao_param);
