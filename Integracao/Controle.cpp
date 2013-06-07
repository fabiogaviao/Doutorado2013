//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Controle.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
/****************************************************/
void Controle::imprimirContagemDeTempo(void)
/*****************************************************
* Feito em 25 de maio de 2013 �s 11:45.
* Objetivo: marcar o tempo de um evento qualquer
* em segundos a partir de 01 de janeiro de 1970.
*****************************************************/
{
   double Horas = 0.0, Minutos = 0.0, Segundos = 0.0;
   double Diferenca = difftime(TempoFinal,TempoInicial);
   Horas = floor(Diferenca/3600.00);
   Diferenca = 3600.00 * (Diferenca/3600.00 - Horas);
   if(Diferenca < 0)
      Diferenca *= -1.0;
   Minutos = floor(Diferenca/60.00);
   Segundos = (Diferenca/60.00 - Minutos) * 60.00;
   cout << endl;
   cout << "Tempo de processamento = " << endl;
   cout << Horas << " horas," << endl;
   cout << Minutos << " minutos," << endl;
   cout << Segundos << " segundos." << endl;
} /* Fim do imprimirContagemDeTempo() */
 /*************************************/

/***********************************************************/
void Controle::executarGenetico(void)
/***********************************************************
* Feita em: 19-mar-2013 �s 16:50h
*
* Objetivos: Emprega os objetos LeitorSCP e AlgoritmoGenetico para
* executar os procedimentos de otimiza��o
************************************************************/
{
   LeitorSCP         lscp;
   lscp.lerProblemaScp();
   /*
      Cria a nova Semente para o gerador
      pseudo-aleatorio. Os n�meros s�o gerados nos seguintes m�todos
      criarPopulacaoInicial(), selecionarPais() (chamado no
      aplicarOperadorCruzamento()) e em aplicarOperadorMutacao()
   */
   AlgoritmoGenetico ag;
   ag.criarPopulacaoInicial(lscp.retornarInstanciaProblema());

   bitset<NUMERO_COLUNAS> OFilho; // o indiv�duo atual
   bitset<NUMERO_COLUNAS> Omi;    // o melhor individuo

   cout << "Processando..." << endl;
   TempoInicial = time(NULL);
   while(1)
   {
      OFilho = ag.aplicarOperadorCruzamento();
      ag.aplicarOperadorMutacao(OFilho);
      ag.aplicarOperadorViabilidade(OFilho);
      if(ag.verificarDuplicidadeNovoIndividuo(OFilho))
         continue;
//      ag.verificarCobertura(OFilho);
//      ag.imprime_individuo(OFilho);
      if(ag.incluirNovoIndividuo(OFilho) < NUM_MAX_GERACOES) // t < M
         continue;
      else
      {
//         ag.imprimirGeracaoAtual();
         // Obter o melhor dos individuos na popula��o atual (UmaGeracao)
         ag.obterMelhorIndividuo(Omi);
         break;
      }
   } // fim do while
   TempoFinal = time(NULL);
   imprimirContagemDeTempo();

   lscp.mostrarSolucao(lscp.retornarInstanciaProblema(),Omi);
}/* Fim do executarGenetico(void) */
/**********************************/

/***********************************************************/
void Controle::ativar(void)
/************************************************************
* ativar() Feita em: 11-fev-2013 �s 09:50h - acrescentar for-
*          mul�rios pr�prios para reportar processamentos
*          internos ao projeto GUI anterior.
*
* Revis�o: 21-mar-2013 �s 14:38h - reverter o projeto para
*          modo console retirando todos os formul�rios
*          para concentrar na solu��o do problema.
************************************************************/
{
   Grafo *pG  = new Grafo();
   Testa(pG, "Nao foi possivel instanciar o Grafo");
   try
   {
//      executarPreprocessamento(pFp2);
      pG->construirGt();
      // contagem at� ???
      for(int i = 0; i < 1; ++i)  // era i < 100
      {
         pG->gerarConjuntoF(); //percorre grafo Gt
         pG->gerarSCP_FMT_Beasley();
         pG->gerarSCP_FMT_Xpress();

         // Otimiza��o:
         executarGenetico();
      }
   }
   __finally
   {
      if(pG)
         delete pG;
   }
} /* Fim do ativar() */
/*********************/

/*******************************************************************/
void Controle::executarPreprocessamento()
/*******************************************************************/
{
   SHELLEXECUTEINFO execinfo ;
   string RelatorioDoPreproc = "";
   RelatorioDoPreproc += "Inicializa��o de estrutura para disparar processo...";
   RelatorioDoPreproc += "\r\n";
   cout << RelatorioDoPreproc;
   memset (&execinfo, 0, sizeof (execinfo)) ;
   execinfo.cbSize = sizeof (execinfo) ;
   execinfo.lpVerb = "open" ;
   execinfo.lpFile = ".\\ProjetoANDA.exe";
   execinfo.lpParameters = NULL;
   execinfo.fMask = SEE_MASK_NOCLOSEPROCESS ;
   execinfo.nShow = SW_SHOWDEFAULT ;

   // Run the program.
   Testa(ShellExecuteEx (&execinfo), "Preprocessamento n�o executado!");
   WaitForSingleObject (execinfo.hProcess, INFINITE) ;
   RelatorioDoPreproc += "Processo disparado";
   RelatorioDoPreproc += "\n\r";
   RelatorioDoPreproc += "Gerados apt.ros e adj.ros";
   cout << RelatorioDoPreproc;

} /* Fim do executarPreprocessamento() ****************/
/******************************************************/

// OBSERVA��ES ANTERIORES A 2013
/*
   AlgoritmoGenetico ag; // Aqui se cria a nova Semente para o gerador
       // pseudo-aleatorio. Os n�meros s�o gerados nos seguintes m�todos:
       // criarPopulacaoInicial(), selecionarPais() (chamado no
       // aplicarOperadorCruzamento()) e em aplicarOperadorMutacao()

   ag.criarPopulacaoInicial(i1.retornarInstanciaProblema());
   // CAD� A AVALIA��O DO N�VEL DE ADAPTA��O DA POPULA��O
   // INICIAL CRIADA? Resp.: Nenhuma avalia��o inicial � sugerida.
   // No entanto ao fazer o cruzamento e se esta solu��o por acaso
   // for a �tima, � de se esperar pela propria estrat�gia do algoritmo
   // que nenhum novo indiv�duo consiga melhorar a popula��o existente
   // desde o in�cio: a t�cnica de n�o permitir duplicar filhos evita
   // que um filho com pior custo adentre a popula��o e se a popula��o
   // inicial j� possui o menor custo nenhum cruzamento gerar� outro
   // filho para ela. No entanto a estrat�gia manda continuar e tentar
   // as 100.000 vezes (M). O tempo de computa��o ser� o tempo de realizar
   // as opera��es de cruzamento, muta��o, viabiliza��o v�rias vezes at�
   // chegar no filho de n�mero 100.000.
      // Novo indiv�duo � unico. Tentar inclui-lo na gera��o atual,
      // (Beasley&Chu p397 item 3.6) em primeiro lugar.
      // Quer seja ou n�o inclu�do, deve ser contado e finalmente
      // verificado se se t < M.
      // SIM: prosseguir. N�O: parar!
      // O melhor indiv�duo � aquele que apresentar melhor fitness
      // na popula��o atual (S)!
 */
