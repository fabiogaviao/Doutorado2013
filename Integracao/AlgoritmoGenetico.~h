//---------------------------------------------------------------------------
#ifndef AlgoritmoGeneticoH
#define AlgoritmoGeneticoH
//---------------------------------------------------------------------------
#include "ParametroGenetico.h"

class AlgoritmoGenetico
{
private:
//--------------------------------------------------------
   InstanciaProblema       ip;
   int                     tmb; // taxa de mutação de bits:
                                // (qtde de bits a mudar / filho)
   int                     *pSelite; // Selite -> conjunto restrito
                           // das melhores colunas que cobrem
                           // as m linhas de Aij olhando apenas as 5
                           // primeiras colunas de cada linha de Aij
   int                     TamanhoSelite;
   int                     kcmc; // 'k' 'c'olunas
                           // de 'm'enor 'c'usto em alfa-i
                           // = 5 no paper (eqv. a k).
   double                  mf, mc, mg; // coef. para calculo
                           // da taxa de mutacao
   size_t                  t; // no. filhos não duplicados
                              // gerados até o momento!
   bool                    duplicidade; // para marcar
                           // se ha ou nao duplicidade
   void zerarColunaRedundante(bitset<NUMERO_COLUNAS> &, vector<int> &);
   int fitnessDeUmDosIndividuosNaGeracao(int);
   ResultadoTorneio  selecionarPais(void);
   int               contarBitsDesiguais(ResultadoTorneio);
   void              combinar(int, int, int, bitset<NUMERO_COLUNAS> &);
   void              calcularSElite(void);
   void              inicializarWi(bitset<NUMERO_COLUNAS> &, vector<int> &);
   void              inicializarS(bitset<NUMERO_COLUNAS> &, vector<int> &);
   void              inicializarU(vector<int> &, vector<int> &);
   void              adicionarColuna(vector<int> &,
                        vector<int> &, bitset<NUMERO_COLUNAS>&,
                        vector<int> &);
   void              removerColunasRedundantes(
                        vector<int> &, vector<int> &,
                        bitset<NUMERO_COLUNAS> &);
   void              diminuirWi(vector<int> &Owi,
                     int Acoluna);
   void              atualizarU(vector<int> &, int);
   double            contarQtasCobrira(vector<int> &, int Acoluna);
   void              ordenarS(vector<int> &);
//-------------------------------------------------------------------
// Depois da retomada

   // Semente para geração 'não pseudo_ramdomica'!
   time_t Semente; // incializado no construtor padrão

   vector<bitset<NUMERO_COLUNAS> >  UmaGeracao;

   int   N; // tamanho da população inicial sugerida
            // no artigo do Beasley (pag. 399, 1o. paragrafo)
   /*********************************************/
   void atualizarWi(vector<int> &OWi, int Acoluna)
   /**********************************************
    * Objetivos: incrementar wi onde ocorrer 1
    * na coluna pAij[][Acoluna].
    *********************************************/
   {
      for(int i = 0; i < ip.nlinhas; ++i)
         if(ip.pAij[i][Acoluna] == 1)
            ++OWi[i];
   } /* fim do atualizarWi() */
   /**************************/

   /***************************************************************/
   void atualizarWiPeloIndividuo(
       bitset<NUMERO_COLUNAS> &Oindividuo,
       vector<int> &OWi)
   /****************************************************************
    * Objetivos: incrementar wi nas colunas de OIndividuo
    * diferentes de zero.
    ***************************************************************/
   {
      for(size_t j = 0; j < NUMERO_COLUNAS; ++j)
         if( Oindividuo.test(j) )
            for(int i = 0; i < ip.nlinhas; ++i)
               if(ip.pAij[i][j])
                  ++OWi[i];
   } /* fim do atualizarWiPeloIndividuo() */
   /***************************************/

   void eliminarColunaRedundanteNaPI(bitset<NUMERO_COLUNAS> &,
                                       vector<int> &);

   /**************************************************************/
   int calcularFitnessIndividuo(bitset<NUMERO_COLUNAS> &OIndividuo)
   /***************************************************************
    * Objetivos:
    **************************************************************/
   {
      int FitnessDoIndividuo = 0;
      for(size_t j = 0; j < OIndividuo.size(); ++j)
         if(OIndividuo.test((size_t)j))
            FitnessDoIndividuo += ip.pFO[j];
      assert(FitnessDoIndividuo);
      return FitnessDoIndividuo;
   }

   /***************************************************************/
   void obterMapaGeracaoAtual(multimap<int, int> &MultiMapDaGeracao)
   /****************************************************************
    * Objetivos: insere na primeira coluna o valor do fitness do
    * individuo e na segunda coluna o índice do mesmo na popula
    * ção atual.
    * Automaticamente o valor do fitness já é ordenado em ordem
    * crescente.
    ***************************************************************/
   {
      int OFitnessDoIndividuo, OIndiceDoIndividuo;
      for(int i = 0; i < N; ++i) // N é sempre o tamanho da geração atual
      {
         OFitnessDoIndividuo = calcularFitnessIndividuo(
                        UmaGeracao[i]);
         OIndiceDoIndividuo = i;
         MultiMapDaGeracao.insert(pair<int, int>(OFitnessDoIndividuo,
                                                   OIndiceDoIndividuo));
      }
   }

   /*******************************************************************/
   bool acharPrimeiroIndividuoPiorNaGeracao(int FitnessNovoIndividuo,
                  multimap<int, int> &MultMapDaGeracao, int *OEscolhido)
   /********************************************************************
    * Objetivos: pega no mapa o indice do individuo que tem um
    * fitness pior (maior) do que o fitness do novo individuo e o
    * salva em Opior setando retorno para true através do flag.
    *******************************************************************/
   {
      multimap<int, int>::iterator it;
      bool flag = false;
      for(it = MultMapDaGeracao.begin(); it != MultMapDaGeracao.end();++it)
         if(it->first > FitnessNovoIndividuo)
         {
            *OEscolhido = it->second;
            flag = true;
            break;
         }
      return flag;
   } /* Fim do acharPrimeiroIndividuoPiorNaGeracao() */
   /**************************************************/

   void viabilizarIndividuoParaPopulacaoInicial(bitset<NUMERO_COLUNAS> &);

   // Qualificação
   void                    calcularDensidadeMatCob(void);
   int                     calcularMi(void);
   double                  DensMatCob; // densidade da matcob
   double                  Mi;
//------------------------------------------------------------------------
public:
   AlgoritmoGenetico()
   {
   //--------------------------------------------------------------

      // Qualificação
      DensMatCob = (double)0.0;
      Mi         = (double)0.0;

      tmb = 0; // nao ha mutacao de bits
      TamanhoSelite = 0;
      pSelite = NULL;
      kcmc = K_COLS_MENOR_CUSTO; // kcmc = 5 (e' o k = 5 do paper)
      mf = MUTATION_COEF[INDICE_PROBLEM_SET][0];
      mc = MUTATION_COEF[INDICE_PROBLEM_SET][1];
      mg = MUTATION_COEF[INDICE_PROBLEM_SET][2];
      t = 0; // inicio contagem filhos gerados
      duplicidade = false;

      // depois da retomada
      ip.ncolunas = 0;
      ip.nlinhas = 0;
      ip.pFO = NULL;
      ip.pAij = NULL;
      N = 100; // sugerido por Beasley para todos os problemas
               // independente de tamanho e densidade da matriz
               // de cobertura!
      srand((unsigned) time(&Semente)); // Gera nova semente apenas
               // uma vez por execução e quando a instância do GA
               // é criada e inicializada no ativar()
   }
   ~AlgoritmoGenetico()
   {
      if(pSelite)
         delete [] pSelite;
   }
   bitset<NUMERO_COLUNAS> aplicarOperadorCruzamento(void);
   void aplicarOperadorMutacao(bitset<NUMERO_COLUNAS> &);
   void aplicarOperadorViabilidade(bitset<NUMERO_COLUNAS> &);
   bool verificarDuplicidadeNovoIndividuo(bitset<NUMERO_COLUNAS> &);
   size_t incluirNovoIndividuo(bitset<NUMERO_COLUNAS> &);
   void obterMelhorIndividuo(bitset<NUMERO_COLUNAS> &);

   void imprime_individuo(bitset<NUMERO_COLUNAS> Oindividuo)
   {
      cout << "Individuo:" << endl;
      cout << Oindividuo << endl << endl;
      for(int j = 0; j < NUMERO_COLUNAS; ++j)
         if(Oindividuo.test((size_t)j) == true)
            cout << j + 1 << " ";
      cout << endl << endl;
//      system("PAUSE");
   } // fim do imprime_individuo()

   // depois da retomada
   void criarPopulacaoInicial(InstanciaProblema );

   void imprimeNumeroNovosIndividuos(void)
   {
      cerr << "t = " << t << endl;
   }

   void verificarCobertura(bitset<NUMERO_COLUNAS> UmFilho)
   {
      int  FO = 0;
      size_t FilhoTamanho = UmFilho.size();
      vector<int> Wi; // vetor para guardar qtas vezes cada linha é
                      // coberta na solução atual
      Wi.assign((size_t) ip.nlinhas, 0);

      for(size_t j = 0; j < FilhoTamanho; ++j)
      {
         if(UmFilho.test(j)) // se for verdadeiro é uma coluna da solução
         {
            FO += ip.pFO[j];
            for(int i = 0; i < ip.nlinhas; ++i)
               if(ip.pAij[i][j] == 1) // cobre linha 'i'?
                  // SIM:
                  ++Wi[i];
         }
      }

      cerr << endl << "================================" << endl;
      cerr << "O que a solucao cobre:" << endl;
      for(int i = 0; i < ip.nlinhas; ++i)
      {
         if( (i%20) == 0)
            cerr << endl;
         cerr << Wi.at(i) << ' ';
      }

      cerr << endl << "FO = " << FO << endl;
//      system("PAUSE");
   } // fim do verificarCobertura()

   void imprimirGeracaoAtual(void);

}; // fim da classe AlgoritmoGenetico
//---------------------------------------------------------------------------
#endif
