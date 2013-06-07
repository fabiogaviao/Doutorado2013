//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Controle.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
/****************************************************/
void Controle::imprimirContagemDeTempo(void)
/*****************************************************
* Feito em 25 de maio de 2013 às 11:45.
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
* Feita em: 19-mar-2013 às 16:50h
*
* Objetivos: Emprega os objetos LeitorSCP e AlgoritmoGenetico para
* executar os procedimentos de otimização
************************************************************/
{
   LeitorSCP         lscp;
   lscp.lerProblemaScp();
   /*
      Cria a nova Semente para o gerador
      pseudo-aleatorio. Os números são gerados nos seguintes métodos
      criarPopulacaoInicial(), selecionarPais() (chamado no
      aplicarOperadorCruzamento()) e em aplicarOperadorMutacao()
   */
   AlgoritmoGenetico ag;
   ag.criarPopulacaoInicial(lscp.retornarInstanciaProblema());

   bitset<NUMERO_COLUNAS> OFilho; // o indivíduo atual
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
         // Obter o melhor dos individuos na população atual (UmaGeracao)
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
* ativar() Feita em: 11-fev-2013 às 09:50h - acrescentar for-
*          mulários próprios para reportar processamentos
*          internos ao projeto GUI anterior.
*
* Revisão: 21-mar-2013 às 14:38h - reverter o projeto para
*          modo console retirando todos os formulários
*          para concentrar na solução do problema.
************************************************************/
{
   Grafo *pG  = new Grafo();
   Testa(pG, "Nao foi possivel instanciar o Grafo");
   try
   {
//      executarPreprocessamento(pFp2);
      pG->construirGt();
      // contagem até ???
      for(int i = 0; i < 1; ++i)  // era i < 100
      {
         pG->gerarConjuntoF(); //percorre grafo Gt
         pG->gerarSCP_FMT_Beasley();
         pG->gerarSCP_FMT_Xpress();

         // Otimização:
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
   RelatorioDoPreproc += "Inicialização de estrutura para disparar processo...";
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
   Testa(ShellExecuteEx (&execinfo), "Preprocessamento não executado!");
   WaitForSingleObject (execinfo.hProcess, INFINITE) ;
   RelatorioDoPreproc += "Processo disparado";
   RelatorioDoPreproc += "\n\r";
   RelatorioDoPreproc += "Gerados apt.ros e adj.ros";
   cout << RelatorioDoPreproc;

} /* Fim do executarPreprocessamento() ****************/
/******************************************************/

// OBSERVAÇÕES ANTERIORES A 2013
/*
   AlgoritmoGenetico ag; // Aqui se cria a nova Semente para o gerador
       // pseudo-aleatorio. Os números são gerados nos seguintes métodos:
       // criarPopulacaoInicial(), selecionarPais() (chamado no
       // aplicarOperadorCruzamento()) e em aplicarOperadorMutacao()

   ag.criarPopulacaoInicial(i1.retornarInstanciaProblema());
   // CADÊ A AVALIAÇÃO DO NÍVEL DE ADAPTAÇÃO DA POPULAÇÃO
   // INICIAL CRIADA? Resp.: Nenhuma avaliação inicial é sugerida.
   // No entanto ao fazer o cruzamento e se esta solução por acaso
   // for a ótima, é de se esperar pela propria estratégia do algoritmo
   // que nenhum novo indivíduo consiga melhorar a população existente
   // desde o início: a técnica de não permitir duplicar filhos evita
   // que um filho com pior custo adentre a população e se a população
   // inicial já possui o menor custo nenhum cruzamento gerará outro
   // filho para ela. No entanto a estratégia manda continuar e tentar
   // as 100.000 vezes (M). O tempo de computação será o tempo de realizar
   // as operações de cruzamento, mutação, viabilização várias vezes até
   // chegar no filho de número 100.000.
      // Novo indivíduo é unico. Tentar inclui-lo na geração atual,
      // (Beasley&Chu p397 item 3.6) em primeiro lugar.
      // Quer seja ou não incluído, deve ser contado e finalmente
      // verificado se se t < M.
      // SIM: prosseguir. NÃO: parar!
      // O melhor indivíduo é aquele que apresentar melhor fitness
      // na população atual (S)!
 */
