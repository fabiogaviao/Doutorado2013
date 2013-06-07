//---------------------------------------------------------------------------

#pragma hdrstop

#include "LeitorSCP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
/*********************************************************/
void LeitorSCP::lerProblemaScp(void)
/*************************************************************
 * Revis�o: 23-mar-2013 - Alterado para usar o nome do problema
 *          que j� est� dentro da classe ao inv�s de recebe-
 *          lo do chamador.
 ************************************************************/
{
   ifstream *pIfs = new ifstream(PROBLEMA, ios::in);
   assert(pIfs);
   // Carrega para a memoria em tempo de execu��o
   pIfs->rdbuf();
   // L� o n�mero de linhas primeiro e o n�mero de colunas em seguida
   (*pIfs) >> nlinhas >> ncolunas;
   // L� os coeficientes da fun��o objetivo
   lerFO(pIfs);
   // L� os coeficientes da matriz de incid�ncia
   lerAij(pIfs);
   // Acabado servi�o com arquivo de entrada fecha-o
   // e deleta o fluxo de entrada utilizado.
   pIfs->close();
   if(pIfs)
      delete pIfs;
} /* Fim do m�todo LeitorSCP() */
/*******************************/

/********************************************/
void  LeitorSCP::lerFO(ifstream *pIfs)
/********************************************
 * Objetivos: Ler os coeficientes da fun��o
 * objetivo do problema no arquivo beasley.txt
 * e transfere para a �rea privada (pFO).
 *******************************************/
{
   // instancia vetor de inteiros para armazenar
   // valores dos coeficientes da funcao objetivo
   pFO = new int[ncolunas];
   assert(pFO);
   for(int j = 0; j < ncolunas; ++j)
      (*pIfs) >> pFO[j];
//   assert(pFO[999] == TAMANHO_PI_SUGERIDO); // Debug para ver se leu o ultimo coeficiente
} /* Fim do lerFO() */
/********************/

/********************************************/
void LeitorSCP::lerAij(ifstream *pIfs)
/**************************************************
 * Objetivos: le os coeficientes da matriz de
 * incidencia (cobertura)
 * N�mero de linhas e de colunas j� lidos antes
 * Coeficientes da matriz de incidencia: pAij
 **************************************************/
{
   // instancia a matriz de cobertura com 0s
   try
   {
      pAij = new int * [nlinhas];
      for(int i = 0; i < nlinhas; ++i)
      {
         pAij[i] = new int [ncolunas];
         for(int j = 0; j < ncolunas; ++j)
            pAij[i][j] = 0;
      }
   }
   catch(bad_alloc)
   {
      cerr << "Nao pode alocar espaco para a Aij!";
      exit(-1);
   }

   // Le os coeficientes e p�e em pAij
   int QtdeDeCobertura, // qtas colunas est�o cobrindo a linha 'i'
       ColunaDoUm,      // a coluna que cobre
       i;               // �ndice de i que deve ir de 0 at� nlinhas
                        // pois todas as linhas tem que estar coberta
                        // ao menos uma vez sen�o o problema n�o tem
                        // solu��o!
   i = 0;
   while(!pIfs->eof())
   {
      if(pIfs->eof())
         break;
      (*pIfs) >> QtdeDeCobertura;
      for(int j = 0; j < QtdeDeCobertura; ++j)
      {
         (*pIfs) >> ColunaDoUm;
         pAij[i][ColunaDoUm - 1] = 1;
      }
      ++i;
   }
} /* fim do lerAij() */
/*********************/

/*********************************************************************/
void LeitorSCP::mostrarSolucao(InstanciaProblema ip, bitset<NUMERO_COLUNAS> &ASolucao)
/**********************************************************************
 * Objetivos: mostrar as colunas da solu��o final
 * e o valor da fun��o objetivo correspondente.
 *********************************************************************/
{
//   system("pause");
   cout << "-----------------------------\
-------------------------------" << endl;

   // Imprimir as colunas da solucao
   for(int j = 0, i = 0; j < NUMERO_COLUNAS; ++j)
      if(ASolucao.test(j))
      {
         if( !(i % 14))
            cout << endl;
         cout << j << " ";
         ++i;
      }

   // Imprimir o valor da FO
   cout << endl << endl;
   int   total = 0;
   for( int j = 0; j < NUMERO_COLUNAS; ++j)
      if(ASolucao.test(j))
         total += pFO[j];
   cout << "FO = " << total << endl;

   // Gravar em arquivo
   string Frase = "";
   Frase += ".\\Dados\\Otimizacao\\P10N6\\SolucaoGA";
//   Frase += "P" + ip.nlinhas;
//   Frase += "N" + ip.ncolunas;
   Frase += ".txt";
   ofstream *pOfs = new ofstream(Frase.c_str(),ios::out | ios::trunc);
   assert(pOfs);

   (*pOfs) << "Solucao do problema:" << endl;
   (*pOfs) << "FO = " << total << endl;
   // Imprimir as colunas da solucao
   for(int j = 0, i = 0; j < NUMERO_COLUNAS; ++j)
      if(ASolucao.test(j))
      {
         if( !(i % 14))
            (*pOfs) << endl;
         (*pOfs) << j << " ";
         ++i;
      }
   (*pOfs) << endl;
   pOfs->close();
   if(pOfs)
      delete pOfs;

   system("pause");
} /* fim do mostrarSolucao() */
/*****************************/
