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
* Inicia thread do kernel.
* @param KERNEL	*kernel_param	O kernel que será inicializado.
*/
void kernel_rodar(KERNEL *kernel_param){
	while(1){
		usleep(1000*1000/10);
		tela_escreverNaColuna(&global_tela, "Mensagem do KERNEL.",3);
	}
}
