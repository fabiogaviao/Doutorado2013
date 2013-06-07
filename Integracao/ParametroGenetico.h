#ifndef ParametroGeneticoH
#define ParametroGeneticoH
//------------------------------------------------------------------------
#include <iostream>
using namespace std;
#include <fstream>
#include <cstdlib>
#include <new>
#include <exception.h>
#include <vector>
#include <bitset>
#include <map>
#include <stdlib>
#include <math>
#include <time>
#include <string>
#include <assert>

const char  PROBLEMA[] = ".\\Dados\\Construtiva\\Beasley\\beasleyP20N6.txt";

typedef struct INSTANCIA_PROBLEMA
{
   int   **pAij;
   int   *pFO;
   int   nlinhas;
   int   ncolunas;
} InstanciaProblema;

typedef struct RESULTADO_TORNEIO
{
   int   individuo1;
   int   individuo2;
} ResultadoTorneio;

const size_t   NUM_MAX_GERACOES = 100; //M:100000
            // Beasley&Chu, p400, 2o. paragrafo
const int   K_COLS_MENOR_CUSTO = 5;
// O índice de cada set:
const int      INDICE_PROBLEM_SET = 0; // set 4: indice 0
                                       // set 5: indice 1
                                       // set 6: indice 2
                                       // set A: indice 3
                                       // set B: indice 4
                                       // set C: indice 5
                                       // set D: indice 6
                                       // set E: indice 7
                                       // Os sets F, G e H precisam ser
                                       // carregados
const double      MUTATION_COEF[8][3] =
{
   //mf:    mc:   mg:   Problem set
      10,   200,  1.3,  // 4  menor instância
      10,   200,  0.6,  // 5
      10,   200,  2.0,  // 6
      10,   200,  2.0,  // A
      10,   300,  1.0,  // B
      10,   200,  2.0,  // C
      10,   200,  2.0,  // D
      10,   350,  1.1   // E
};
//-----------------------------------------------------------------------
// depois da retomada
const int      NUMERO_COLUNAS = 3246; // Tamanho do cromossomo em 'bits'
const double   NColProbRestrito = 361; // Este valor não é fornecido e sim
                  // calculado ao executar o programa. No artigo
                  // esse valor é mostrado para cada set de dados.
                  // Porém tem como calcular e ver se bate com o dele! Mas
                  // não é necessário para o algoritmo funcionar.
                  // Ele é usado no calculo do N o qual é fornecido
                  // como sugestão igual para todos   os testes
                  // de 100 (N = 100 p/ todos os sets)
//------------------------------------------------------------------------
#endif
