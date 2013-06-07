//---------------------------------------------------------------------------
#ifndef InterfaceUsuarioH
#define InterfaceUsuarioH
//---------------------------------------------------------------------------
#include "ParametroGenetico.h"

class LeitorSCP
{
private:
   int   **pAij;
   int   nlinhas, ncolunas;
   int   *pFO;
   void  lerFO(ifstream *);
   void  lerAij(ifstream *);
public:
   LeitorSCP()
   {
      pAij = NULL;
      nlinhas = 0;
      ncolunas = 0;
      pFO = NULL;
   }
   ~LeitorSCP()
   {
      if(pFO)
         delete [] pFO;
      if(pAij)
      {
         for(int i = 0; i < nlinhas; ++i)
            delete [] pAij[i];
         delete [] pAij;
      }
   }
   void lerProblemaScp(void);

   InstanciaProblema retornarInstanciaProblema(void)
   {
      InstanciaProblema ip;
      ip.pAij = pAij;
      ip.pFO = pFO;
      ip.nlinhas = nlinhas;
      ip.ncolunas = ncolunas;
      return ip;
   }
   void mostrarSolucao(InstanciaProblema, bitset<NUMERO_COLUNAS> &);
};
//---------------------------------------------------------------------------
#endif
