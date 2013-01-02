/**
* Concentra as funções de controle da execução do SOPA e da Má-quina,
* isto é, permite parar-los, continuar sua execução e executa-los passo-a-passo.
*/
//---------------------------------------------------------------------
//			DADOS						
//---------------------------------------------------------------------

//Constantes



//---------------------------------------------------------------------
//			FUNÇÕES						
//---------------------------------------------------------------------
/**
* Executa inicialização necessárias.
*/
void controladorSOPA_inicializar(void);

/**
* Requisita a posse do controle do recurso. 
* Bloqueia a thread até que seja possível ceder-lhe a posse.
*/
void controladorSOPA_requisitarPosse(void);

/**
* Libera a posse do controle do recurso. 
* Desbloquia a thread que há mais tempo espera pela posse.
* @param TIPO_RECURSO_DISPUTADO		*recurso_param		O recurso que será disputado.
*/
void controladorSOPA_liberarPosse(void);








