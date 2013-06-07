//---------------------------------------------------------------------------

#pragma hdrstop

//---------------------------------------------------------------------------
#include "Controle.h"

/*****************/
void sem_memoria()
/***************************************************************
 * Objetivo: responder quando o operador global 'new' lan�ar uma
 * excecao por falta de mem�ria no HEAP para ser alocada.
 **************************************************************/
{
   cerr << "Nao consegue alocar espaco do heap!"
        << endl;
   exit(1);
} /* Fim da fun��o de call_back sem_memoria() */
/**********************************************/

#pragma argsused
int main(int argc, char* argv[])
{
   try
   {
      // Antes do programa come�ar a ser executado
      // registra o novo handler (call_back())
      // no ambiente de execu��o do processo.
      set_new_handler(sem_memoria);
      Controle controle;
      controle.ativar();
   }
   catch(const exception& e)
   {
      cerr << "Excessao ocorrida na funcao handler \'sem_memoria()\': "
                  << e.what() << endl;
   }
   return 0;
}
//---------------------------------------------------------------------

