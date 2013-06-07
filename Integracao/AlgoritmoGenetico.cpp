//---------------------------------------------------------------------------

#pragma hdrstop

#include "AlgoritmoGenetico.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

/*****************************************/
int AlgoritmoGenetico::calcularMi(void)
/*****************************************
 * Data de cria��o: 27-mar-2013 �s 14:31h
 * Objetivo: Antes este valor de Mi era
 * fixo e tinha um valor para cada conjunto
 * de scp de beasley que se estava testando.
 * Mi = media do n�mero de repeti��es de
 * colunas na popula��o inicial.
 *****************************************/
{
   Mi = (double)2.0;
   return Mi;
} /* Fim do calcularMi() */
/*************************/

/*****************************************************/
void AlgoritmoGenetico::calcularDensidadeMatCob(void)
/******************************************************
 * Data de cria��o: 26-mar-2013 �s 15:00h
 * Objetivo: calcular a densidade da matriz de cobertura
 * do SCP gerado a partir do conjunto F.
 ******************************************************/
 {
   int    Contagem = 0;

      for(int i = 0; i < ip.nlinhas; ++i)
         for(int j = 0; j < ip.ncolunas; ++j)
            if(ip.pAij[i][j] == 1)
               ++Contagem;

      DensMatCob = ((double)(Contagem))/((double)ip.nlinhas *
                                             (double)ip.ncolunas);
      // OU PARA O NOSSO CASO ESPEC�FICO:
      // DensMatCob = ((double)ndr)/((double)ip.nlinhas);

 } /* Fim do calcularDensidadeMatCob() */
 /**************************************/

/***************************************************************************/
void AlgoritmoGenetico::criarPopulacaoInicial(InstanciaProblema UmaInstancia)
/****************************************************************************
 * Objetivos: Calcular o tamanho da popula��o inicial
 * e criar os individuos desta popula��o.
 ****************************************************************************/
{
   ip = UmaInstancia; // Problema gerado a partir de F:
                      // Par�metros: fo, restri��es, nlinhas, ncolunas.
   calcularDensidadeMatCob();
   // Quantidade de indiv�duos da popula��o inicial para garantir
   // cobrir no m�nimo todas as linhas de F
   // N = floor(DensMatCob*((double)ip.ncolunas) * calcularMi());
   // Qualifica��o 03 de maio de 2013
   //N = ceil( (ip.ncolunas/((double)1.0/DensMatCob)) * calcularMi() );
   N = ceil(calcularMi() * ip.ncolunas * DensMatCob * 0.1);
   if(N <= 4)
      N = 5;
   bitset<NUMERO_COLUNAS> NovoIndividuo;
   NovoIndividuo.reset(); // Zera o novo individuo!

   vector<int> AlfaIk;
   vector<int> Wi; // Vetor de "nlinhas" de comprimento.
                   // Guarda qtas vezes cada linha de Aij
                   // � coberta pela popula��o inicial
   int ValorSorteado = 0;
   try
   {
      // Gera popula��o inicial de N indiv�duos
      for(int p = 0; p < N; ++p)
      {
         // Para cada linha de pAij
         for(int i = 0; i < ip.nlinhas; ++i)
         {
            // Sorteia uma coluna que cobre e a adiciona no NOvoIndividuo
            // tornando assim viavel o novo indiv�duo
            for(int j = 0; j < ip.ncolunas; ++j)
               if(ip.pAij[i][j] == 1)
                  AlfaIk.push_back(j);
            // Sorteia uma coluna de AlfaIk.
            // Qualifica��o 03 de maio de 2013: tem que ver se size � 1.
            // Se for tem que seleciona-la sem fazer sorteio.
            if(AlfaIk.size() == 1)
               ValorSorteado = AlfaIk.at(0);
            else
               ValorSorteado = AlfaIk.at( (size_t)(rand()%AlfaIk.size()) );
            // seta o bit desta coluna no NovoIndividuo
            NovoIndividuo.set((size_t)ValorSorteado);
            AlfaIk.clear(); // zerar antes de fazer proxima linha!
         } // pr�xima linha de Aij
         // Inserir NovoIndividuo em UmaGeracao (atributo da classe).
         Wi.assign((size_t) ip.nlinhas, 0); // cria e zera o Wi
         atualizarWiPeloIndividuo(NovoIndividuo, Wi);
         eliminarColunaRedundanteNaPI(NovoIndividuo, Wi);
//         imprime_individuo(NovoIndividuo);
         UmaGeracao.push_back(NovoIndividuo);
         NovoIndividuo.reset(); // Zera bits do individuo atual p/ novo
         Wi.clear();
         Wi.assign((size_t) ip.nlinhas, 0); // zerar vetor de cobertura
                                            // para o novo individuo
      } // retorna ao primeiro for para fazer novo individuo (at� 'N')
//      imprimirGeracaoAtual();
   }
   catch(const exception& e)
   {
      cerr << "Excecao ocorrida no \'criarPopulacaoInicial()\': "
                     << e.what() << endl;
   }
   // limpa a memoria:
   if(!AlfaIk.empty())
      AlfaIk.clear();
   if(!Wi.empty())
      Wi.clear();
} /* fim criarPopulacaoInicial() */
/***********************************/

/************************************************************/
void AlgoritmoGenetico::eliminarColunaRedundanteNaPI(
        bitset<NUMERO_COLUNAS> &UmIndividuo, vector<int> &OWi)
/*************************************************************
 * Objetivos: Se para uma coluna do individuo todos
 * os wi's s�o maiores ou iguais a 2, a mesma �
 * redundante e pode ser apagada da solu��o final.
 ************************************************************/
{
/*
   vector<int> T; // T conjunto auxiliar ref. pag. 398, item 4.1
                  // leter 'iii'
   for(size_t j = 0; j < UmIndividuo.count(); ++j)
      if(UmIndividuo.test(j))
         T.push_back(j);
   int ColunaSelecionada = -1;

   int ValorSorteado = rand()%(T.size());
   T
*/

   bool Redundante = true; // parte do pressuposto de que a coluna
                           // j � redundante!
   // Para cada bit do indiv�duo UmIndividuo (colunas)
   try
   {
      for(int j = 0; j < ip.ncolunas; ++j)
         // Verificar se o mesmo � falso ou verdadeiro.
         if(UmIndividuo.test((size_t)j))
         {// Sendo verdadeiro:
            // Percorrer todas linhas da matriz de cobertura
            // referentes a esta coluna 'j'
            for(int i = 0; i < ip.nlinhas; ++i)
            {  // Se a linha percorrida for coberta pela coluna 'j'
               if( ip.pAij[i][j] == 1 )
                  // verificar se o OWi[i] � >= 2
                  if(OWi[i] >= 2)
                     // Se sim, a coluna 'j' pode ser redundante.
                     // Para isto devem ser verificadas as demais
                     // linhas desta coluna
                     continue;
                  // Caso contr�rio: wi[i] � no m�ximo 1 e ao
                  // tirar a coluna (subtrair 1) ela deixar� de
                  // cobrir a linha 'i'. Em outras palavras, a
                  // coluna faz realmente falta (n�o � redundante)
                  // Marcar Redundante false!
                  else
                  {
                     Redundante = false;
                     break; // n�o precisa continuar at� a ultima linha
                  }
            }
            // Acabou de percorrer todas linhas de uma coluna 'j' ou
            // descobriu antecipadamente que a coluna 'j' n�o � Redundante
            if(Redundante == true)
            {
               // Decrementar o OWi
               for(int i = 0; i < ip.nlinhas; ++i)
                  if( ip.pAij[i][j] == 1 )
                     --OWi[i];
               // tirar a coluna 'j' da solu��o
               UmIndividuo.reset( (size_t)j);
            }
            // come�a tudo outra vez para o proximo bit (j) em UmIndividuo
            Redundante = true;
         } // fim do 'for ... if'  que percorre os bits de UmIndividuo
   }
   catch(exception& e)
   {
      cerr << "Excecao ocorrida no \'eliminarColunaRedundanteNaPI()\': " <<
               e.what() << endl;
   }
}/* fim do eliminarColunaRedundanteNaPI() */
/******************************************/

/*********************************************/
void AlgoritmoGenetico::zerarColunaRedundante(
      bitset<NUMERO_COLUNAS> &bs, vector<int>  &Owi )
/*********************************************
 * Objetivos: Zera no "bs" o bit correspondente
 * a uma coluna que nao faz falta na solucao!
 * De outra forma: se separar as linhas que uma
 * coluna do indiv�duo cobre e, para cada linha,
 * ao subtrair 1 nas respectivas posi��es de Owi
 * n�o der 0 em nenhuma a coluna � redundante e
 * pode ser removida da solu��o final.
 ********************************************/
{
   bool flag = true;
   // Para cada bit dos NUMERO_COLUNAS bits do indiv�vuo bs (colunas)
   try
   {
      for(int j = 0; j < NUMERO_COLUNAS; ++j)
         // Verificar se o mesmo � falso ou verdadeiro.
         // Sendo verdadeiro
         if(bs.test((size_t)j))
         {
            // Percorrer todas linhas da matriz de cobertura
            // referentes a esta coluna 'j'
            for(int i = 0; i < ip.nlinhas; ++i)
               // Se a linha for coberta pela coluna 'j'
               if( ip.pAij[i][j] == 1 )
                  // verificar se o Owi[i] � >= 2
                  if(Owi[i] >= 2)
                     // Se sim, coluna 'j' pode ser redundante
                     // verificar as demais coberturas
                     continue;
                  // Caso contr�rio: wi[i] � no m�ximo 1 e ao
                  // tirar a coluna (subtrair 1) ela deixar� de
                  // cobrir a linha 'i'. Em outras palavras, a
                  // coluna faz realmente falta.
                  // Marcar flag false!
                  else
                     flag = false;
            // No final, ap�s percorrer todas as linhas
            // Se flag � true
            if(flag == true)
            {
               // decrementar o wi
               for(int i = 0; i < ip.nlinhas; ++i)
                  if( ip.pAij[i][j] == 1 )
                     --Owi[i];
               // tirar a coluna 'j' da solu��o
               bs.reset( (size_t)j);
            }
            // come�a tudo outra vez para o proximo 'j' em bs
            flag = true;
         } // fim do 'if' que testa todos os bits do inv�duo
   }
   catch(exception& e)
   {
      cerr << "Excecao ocorrida no \'zerarColunaRedundante()\' : " <<
                  e.what() << endl;
   }
} /* fim zerarColunaRedundante() */
/**********************************/

int AlgoritmoGenetico::fitnessDeUmDosIndividuosNaGeracao(int individuo)
{
   int z = 0;
   // Para cada bit dos NUMERO_COLUNAS de um indiv�duo
   try
   {
      for(int j = 0; j < NUMERO_COLUNAS; ++j)
         // se for 'true'
         if( (UmaGeracao.at((size_t)individuo)).test((size_t)j) == true)
            z += ip.pFO[j]; // adiciona no custo total
   }
   catch(const exception& e)
   {
      cerr << "Excecao ocorrida no \'fitnessDeUmDosIndividuosNaGeracao()\' : "
                           << e.what() << endl;
   }
   return z;
} /* fim fitnessDeUmDosIndividuosNaGeracao() */
/*********************************************/

ResultadoTorneio AlgoritmoGenetico::selecionarPais(void)
{
   int      ElementosSorteados[4];
   int      ValorDeFitness[4];
   ResultadoTorneio rt;

   size_t   tamanho;
   tamanho = UmaGeracao.size();

   randomize();
   for(int i = 0; i < 4; ++i)
      ElementosSorteados[i] = rand()%tamanho;
   for(int i = 0; i < 4; ++i)
      ValorDeFitness[i] = fitnessDeUmDosIndividuosNaGeracao(ElementosSorteados[i]);

   // Obtem os dois pais com menor fitness (dois mais adaptados)
   int   ValorTmp;
   for(int i = 0; i < 3; ++i)
      for(int j = i + 1; j < 4; ++j)
         if(ValorDeFitness[i] > ValorDeFitness[j])
         {
            ValorTmp = ElementosSorteados[j];
            ElementosSorteados[j] = ElementosSorteados[i];
            ElementosSorteados[i] = ValorTmp;
            ValorTmp = ValorDeFitness[j];
            ValorDeFitness[j] = ValorDeFitness[i];
            ValorDeFitness[i] = ValorTmp;
         }
   // Qualifica��o 04 de maio de 2013
   if(ElementosSorteados[0] != ElementosSorteados[1])
   {
      rt.individuo1 = ElementosSorteados[0]; // 1o. melhor
      rt.individuo2 = ElementosSorteados[1]; // 2o. melhor
   }
   else if(ElementosSorteados[0] != ElementosSorteados[2])
   {
      rt.individuo1 = ElementosSorteados[0];
      rt.individuo2 = ElementosSorteados[2];
   }
   else if(ElementosSorteados[0] != ElementosSorteados[3])
   {
      rt.individuo1 = ElementosSorteados[0];
      rt.individuo2 = ElementosSorteados[3];
   }
   else if(ElementosSorteados[1] != ElementosSorteados[2])
   {
      rt.individuo1 = ElementosSorteados[1];
      rt.individuo2 = ElementosSorteados[2];
   }
   else if(ElementosSorteados[1] != ElementosSorteados[3])
   {
      rt.individuo1 = ElementosSorteados[1];
      rt.individuo2 = ElementosSorteados[3];
   }
   else if(ElementosSorteados[2] != ElementosSorteados[3])
   {
      rt.individuo1 = ElementosSorteados[2];
      rt.individuo2 = ElementosSorteados[3];
   }
   else
   {
//      cerr << "Quatro pais do torneio bin�rio tem o mesmo fitness!" << endl;
//      system("pause");
//      assert(true);
      rt.individuo1 = ElementosSorteados[0];
      rt.individuo2 = ElementosSorteados[1];
   }
   return rt;
   } /* fim do selecionarPais() */
/********************************/

/***********************************************************************/
bitset<NUMERO_COLUNAS> AlgoritmoGenetico::aplicarOperadorCruzamento(void)
/************************************************************************
 * Objetivo: gera novo indiv�duo pelo cruzamento de genes
 * existentes na gera��o atual.
 * Baseia-se no algoritmo de torneio bin�rio
 * Nesse torneio primeiro devem ser selecionados 2 pools
 * de 2 individuos cada. Escolhe os dois melhores
 * individuos dos 4. Gera um filho copiando os
 * genes iguais dos pais e transferindo os genes
 * diferentes respeitando-se a proporcao
 * de fitness de cada um.
 * NOTA: Se tem 80 bits diferentes para transferir
 * e o melhor individuo possui 60% de participacao na composi��o
 * do fitness global, primeiro s�o transferidos 48 bits
 * do individuo mais adaptado e o restante, (32 bits),
 * do individuo menos adapatado.
 ***********************************************/
{
   bitset<NUMERO_COLUNAS> OFilho;
   ResultadoTorneio rt = selecionarPais();
/*
   assert(rt.individuo1);
   assert(rt.individuo2);
*/
   // Contar numero de bits desiguais nos dois
   int bits_desiguais = 0;
   bits_desiguais = contarBitsDesiguais(rt);
   /*
   if(bits_desiguais == 0)
   {
      cerr << "bits desiguais = " << bits_desiguais << endl;
      system("pause");
      assert(bits_desiguais != 0);
   }
   */
   //   assert(bits_desiguais); // Tem que dar diferente de 0 (zero)
   // fusion crossover operator:
   int fp1, fp2;
   double p; // probabilidade
   fp1 = fitnessDeUmDosIndividuosNaGeracao(rt.individuo1);
   fp2 = fitnessDeUmDosIndividuosNaGeracao(rt.individuo2);
/*
   assert(fp1);
   assert(fp2);
*/
   // fp1 � melhor se for menor
   if(fp1 < fp2)
      p = (double)fp2 / (double)(fp1 + fp2); // obtem maior percentagem
   else
      p =(double)fp1 / (double)(fp1 + fp2); // obtem maior percentagem
   int   bits_transferir;
   bits_transferir = (int)ceil(bits_desiguais*p);
   // Antes de fazer a troca de bits � interessante resetar
   // OFilho para que assuma tudo '0' (false).
   OFilho.reset();

   if(fp1 < fp2)
      combinar(rt.individuo1, rt.individuo2, bits_transferir, OFilho);
   else
      combinar(rt.individuo2, rt.individuo1, bits_transferir, OFilho);
   return OFilho;
} /* fim de aplicarOperadorCruzamento() */
/******************************************/

int AlgoritmoGenetico::contarBitsDesiguais(ResultadoTorneio rt)
{
   int   contagem = 0;
   try
   {
      for(int j = 0; j < NUMERO_COLUNAS; ++j)
         if((UmaGeracao.at((size_t)rt.individuo1)).test((size_t)j)
                                    !=
            (UmaGeracao.at((size_t)rt.individuo2)).test((size_t)j))
            ++contagem;
   }
   catch(const exception& e)
   {
      cerr << "Excecao ocorrida no \'contarBitsDesiguais()\' : "
                        << e.what() << endl;
   }
   return contagem;
} /* fim do contarBitsDesiguais() */
/**********************************/

/**********************************************/
void AlgoritmoGenetico::combinar(
                           int melhor, int pior,
                           int qtde_transferir,
                           bitset<NUMERO_COLUNAS> &Filho
                           )
/****************************************************
 * Objetivos: Copiar os bits iguais, depois os
 * bits diferentes. Estes dever�o ser tirados
 * do melhor pai primeiro at� esgotar. Depois deveremos
 * copiar os bits restantes do pior pai.
 ***************************************************/
{
   int Acontagem = qtde_transferir;
   // Bits iguais transfere de qualquer pai
   try
   {
      for(int j = 0; j < NUMERO_COLUNAS; ++j)
         if( (UmaGeracao.at((size_t)melhor)).test((size_t)j)
                                    ==
             (UmaGeracao.at((size_t)pior)).test((size_t)j) )
            if((UmaGeracao.at((size_t)melhor)).test((size_t)j))
               Filho.set((size_t)j, true);
            // N�o precisa do else pois Filho foi inicializado
            // � priori com tudo 0.
      // se os dois pais n�o tiverem nenhum bit diferente encerra
      // aqui devolvendo Filho sem mudan�a
      if(Acontagem == 0)
         return;
      // Qdo os bits forem diferentes transferir 1o. do pai melhor
      for(int j = 0; j < NUMERO_COLUNAS; ++j)
         if( (UmaGeracao.at((size_t)melhor)).test((size_t)j)
                                    !=
             (UmaGeracao.at((size_t)pior)).test((size_t)j) )
         {
            if(Acontagem > 0)
            {  // os bits do melhor primeiro:
               Filho.set((size_t)j, (UmaGeracao.at((size_t)melhor)).test((size_t)j) );
               --Acontagem;
            }
            else// depois o do pior:
               Filho.set((size_t)j, (UmaGeracao.at((size_t)pior)).test((size_t)j) );
         }
   }
   catch(const exception& e)
   {
      cerr << "Excecao ocorrida no \'combinar()\' : " << e.what() << endl;
   }
} /* fim do combinar() */
/***********************/

void AlgoritmoGenetico::aplicarOperadorMutacao(bitset<NUMERO_COLUNAS> &Filho)
{
   // calcular a taxa de muta��o para o
   // problema em questao.
   int   pos;
   // Pega os valores fixos de mf, mc e mg da menor inst�ncia!
   mf = MUTATION_COEF[INDICE_PROBLEM_SET][0];
   mc = MUTATION_COEF[INDICE_PROBLEM_SET][1];
   mg = MUTATION_COEF[INDICE_PROBLEM_SET][2];
   // a vari�vel independente nesta equa��o � o t (n�mero de gera��es)
   // tmb = taxa de mudan�a de bits no tempo
   tmb = (int)ceil( mf/( 1.0 + exp(-4.0 * mg * (t - mc)/mf) ) );
   if(tmb <= 0)
   {
      cerr << "tmb <= 0" << endl;
      system("pause");
   }
   //   assert(tmb > 0);
   // Qualifica��o:
   // N�o � chamada a calcularSElite para reduzir o grau de cobrimento
   // do dom�nio do problema e consequentemente ter um n�mero de
   // indiv�duos menores na PI.

   // calcularSElite();

   // Qualifica��o:
   // Mudar "tmb" bits de Filho usando uma estrat�gia para mudar os bits
   // mais � esquerda (mais � direita implica em colunas de alto custo)
   for(int j = 0; j < tmb; ++j)
   {
      // apenas na metade mais � esquerda
      pos = rand()% ((int)(ip.ncolunas/(double)2.0));

      if( Filho.test((size_t)pos) )
         Filho.reset((size_t)pos);
      else
         Filho.set((size_t)pos);
   }
} /* fim do aplicarOperadorMutacao() */
/***************************************/

/*******************************************/
void AlgoritmoGenetico::calcularSElite(void)
/********************************************
 * Objetivo: Calcular o conjunto SElite que
 * � a uni�o de todos AlfaIk, ou seja, das
 * k colunas de menores custos de cada linha
 *******************************************/
{
   bitset<NUMERO_COLUNAS> bs;
   int   k = 0;
   // Para cada linha de Aij
   for(int i = 0; i < ip.nlinhas; ++i)
      // procura-se as 'kcmc' primeiras colunas com valor igual a 1
      for(int j = 0; j < ip.ncolunas; ++j)
         if(ip.pAij[i][j] == 1)
         {
            // e guarda seus indices em bs
            bs.set((size_t)j, 1);
            ++k;
            if(k == kcmc) // = 5 (cte!)
            {
               k = 0;
               break;
            }
         }
//   imprime_individuo(bs);
   // bs representa a uniao dos indices de todas as 5 primeiras
   // colunas de Aij de menor custo. pSelite e' um vetor
   // que guarda o conjunto desses indices
   TamanhoSelite = (int) bs.count();
   pSelite = new int [TamanhoSelite];
   if(pSelite == NULL)
   {
      cerr << "pSelite == NULL" << endl;
      system("pause");
   }
//   assert(pSelite);
   for(int j = 0; j < TamanhoSelite; ++j)
      pSelite[j] = 0; // Inicializa com 0 o Selite
   // P�e em Selite os valores das colunas encontradas anteriormente
   for(int j = 0, i = 0; j < ip.ncolunas; ++j)
      if(bs.test((size_t)j) == true)
         pSelite[i++] = j;
}/* fim da calcularSElite() */
/*****************************/

/****************************************************************************/
void AlgoritmoGenetico::obterMelhorIndividuo(bitset<NUMERO_COLUNAS> &ASolucao)
/*****************************************************************************
 * Objetivos: Ao inv�s de devolver o �ndice do melhor dos individuos
 * na popula��o atual (UmaGeracao) optei por devolver o indiv�duo
 * selecionado.
 ****************************************************************************/
{
   vector<int> CustoIndividuosGeracaoAtual;
   // Geracao atual == UmaGeracao
   try
   {
      for( size_t i = 0; i < UmaGeracao.size(); ++i)
         CustoIndividuosGeracaoAtual.push_back(calcularFitnessIndividuo(UmaGeracao.at((size_t)i)));
      // selecionar o indice do individuo de menor custo: � o
      // melhor da melhor gera��o. Portanto a solu��o esperada!
      int OMenor = CustoIndividuosGeracaoAtual.at(0);
      int OIndiceDoMelhor = -1;
      for(size_t i = 0; i < CustoIndividuosGeracaoAtual.size(); ++i)
         if(CustoIndividuosGeracaoAtual.at(i) < OMenor)
         {
            OMenor = CustoIndividuosGeracaoAtual[i];
            OIndiceDoMelhor = i;
         }
      if(OIndiceDoMelhor == -1)
      {
         OIndiceDoMelhor = 0;
         /*
         cerr << "O indice do melhor == -1" << endl;
         system("pause");
         */
      }
//      assert(OIndiceDoMelhor);
      ASolucao = UmaGeracao.at((size_t)OIndiceDoMelhor);
//      imprime_individuo(ASolucao);
      verificarCobertura(ASolucao);
   }
   catch(const exception& e)
   {
      cerr << "Excecao ocorrida no \'obterMelhorIndividuo()\' : "
                        << e.what() << endl;
   }
} /*  fim do metodo obterMelhorIndividuo() */
/*******************************************/

/**************************************************************/
bool AlgoritmoGenetico::verificarDuplicidadeNovoIndividuo(
                           bitset<NUMERO_COLUNAS> &NovoIndividuo)
/**************************************************************
 * Objetivo: Checar se existe algum individuo
 * na geracao atual igual ao filho recem gerado
 * (NovoIndividuo). Retorna V ou F e ao mesmo tempo
 * seta a variavel "duplicidade" para true na �rea
 * privada da classe.
 * Essa vari�vel � lida por outro m�todo para ajudar o ativar()
 * no seu gerenciamento.
 **************************************************************/
{
   duplicidade = false;
   for(size_t i = 0; i < UmaGeracao.size(); ++i)
      if( UmaGeracao[i] == NovoIndividuo )
      {
         duplicidade = true;
         break;
      }
   return duplicidade;
} /* fim do verificarDuplicidadeNovoIndividuo() */
/************************************************/

/***************************************************************/
size_t AlgoritmoGenetico::incluirNovoIndividuo(
                     bitset<NUMERO_COLUNAS> &NovoIndividuo)
/***********************************************************************
 * Objetivo: Quem e' escolhido para sair e' o 1o. individuo
 * que apresentar uma adaptacao pior do que
 * a do individuo que entra. No caso de cobertura
 * nao unicost seria o primeiro individuo
 * que apresenta z maior do que z do novo individuo.
 * Quando n�o tiver um pior interrompe com assert()
 * Algoritmo:
 *    Calcular fitness do novo indiv�duo
 *    Obter vetor de fitness referente a gera��o atual
 *    Achar na gera��o atual um individuo com fitness pior
 *    do que o fitness do novo individuo
 *    Substituir o individuo com fitness pior pelo novo
 *       individuo
 * Alternativa: caso n�o tenha pior individuo na gera��o atual
 * simplesmente n�o inclui este individuo e continua o
 * processo de cruzamento x muta��o x viabiliza��o x check de duplica��o.
 ***********************************************************************/
{
   // Calcular o fitness do novo indiv�duo (fni)
   int FitnessNovoIndividuo = calcularFitnessIndividuo(NovoIndividuo);

   // Obter matriz de duas colunas contendo na primeira coluna
   // o valor de fitness referente a cada individuo da
   // gera��o atual e na segunda coluna a sua posi��o relativa
   // ao in�cio da gera��o atual
   multimap<int, int> MapaDeFitnessDaGeracao;
   obterMapaGeracaoAtual(MapaDeFitnessDaGeracao);

   // Achar no mapa o �ndice do primeiro individuo que tem fitness
   // maior do que o fitness do novo indiv�duo.
   int IndiceDoPrimeiroSuperior = 0;

   // Qualifica��o
   if(acharPrimeiroIndividuoPiorNaGeracao(FitnessNovoIndividuo,
                  MapaDeFitnessDaGeracao, &IndiceDoPrimeiroSuperior))
      UmaGeracao[IndiceDoPrimeiroSuperior] = NovoIndividuo;

   // Incrementa a gera��o mesmo que o filho n�o tenha sido incluido na popula��o
   ++t;
   //imprimeNumeroNovosIndividuos();

   return t;
} /* fim do incluirNovoIndividuo() */
/***********************************/

/**********************************************************************/
void AlgoritmoGenetico::aplicarOperadorViabilidade(
                           bitset<NUMERO_COLUNAS> &Ofilho)
/***********************************************************************
 * Objetivo: Alg p397 Beasley&Chu
 **********************************************************************/
{
   vector<int> Wi;// numero de colunas do filho que cobrem cada linha 'i'
   vector<int> S; // cj de colunas na solu��o (no filho)
   S.reserve((size_t) ip.ncolunas);
   vector<int> U; // o cj de linhas n�o cobertas
   try
   {
//      imprime_individuo(Ofilho);
      Wi.assign((size_t)ip.nlinhas, 0); // gera Wi:
      inicializarWi(Ofilho, Wi); // numero de colunas do filho que
                                 // cobrem cada linha de Aij
      inicializarS(Ofilho, S); // cada elemento de S guarda a
                               // coluna da solu��o (Ofilho)
      inicializarU(Wi, U); // cada elemento de U � uma linha n�o
                           // coberta pelo filho
      // parte (a) e (b) da letra (iii) do Alg p397
      // de Beasley e Chu
      adicionarColuna(U, S, Ofilho, Wi);
      // faz uma pequena otimiza��o local
      removerColunasRedundantes(S, Wi, Ofilho);
      if(!Wi.empty())
      {
         Wi.swap(vector<int>() );
         Wi.clear();
      }
      if(!S.empty())
         S.clear();
      if(!U.empty())
      {
         U.swap(vector<int> () );
         U.clear();
      }
   }
   catch(const exception& e)
   {
      cerr << "Excecao ocorrida no \'aplicarOperadorViabilidade()\': "
                     << e.what() << endl;
   }
} /* Fim de aplicarOperadorViabilidade() */
/*******************************************/

/***************************************************************/
void AlgoritmoGenetico::viabilizarIndividuoParaPopulacaoInicial(
                           bitset<NUMERO_COLUNAS> &Ofilho)
/******************************************************************
 * Objetivo: Alg p398, item 4, par�grafo 4.1 letra 'ii' Beasley&Chu
 * Diferen�a para o aplicarOperadorViabilidade � que neste
 * m�todo n�o tem a elimina��o de colunas redundantes. Esta
 * tarefa deve ser feita por quem chamar este m�todo, no caso
 * pela criarPopulacaoInicial(). Para cada linha ele simplesmente
 * adiciona uma coluna que a cobre. Assim a solu��o � sempre
 * vi�vel.Mas h� a possibilidade de achar uma linha que seja
 * coberta por coluna fora de AlfaIk!
 ******************************************************************/
{
   vector<int> Wi;// numero de vezes que cada linha 'i' � coberta pelo filho
   vector<int> S; // cj de colunas na solu��o (no filho)
   S.reserve((size_t) ip.ncolunas);
   vector<int> U; // o cj de linhas n�o cobertas
   try
   {
//      imprime_individuo(Ofilho);
//      verificarCobertura(Ofilho);
      Wi.assign((size_t)ip.nlinhas, 0); // gera Wi:
      inicializarWi(Ofilho, Wi); // numero de colunas do filho que
                                 // cobrem cada linha de Aij
      inicializarS(Ofilho, S); // cada elemento de S guarda a
                               // coluna da solu��o (Ofilho)
      inicializarU(Wi, U); // cada elemento de U � uma linha n�o
                           // coberta pelo filho
      // letras (a) e (b) da parte (iii) do Alg p397
      // de Beasley e Chu � usada aqui ao inv�s de seguir
      // como em Beasley&Chu pag. 398, letras (a) e (b) da parte (ii)
      // devido a uma quest�o de melhorar a popula��o inicial e
      // evitar solu��es n�o vi�veis na PI.
      adicionarColuna(U, S, Ofilho, Wi);
//      imprime_individuo(Ofilho);
//      verificarCobertura(Ofilho);
      if(!Wi.empty())
      {
         Wi.swap(vector<int>() );
         Wi.clear();
      }
      if(!S.empty())
         S.clear();
      if(!U.empty())
      {
         U.swap(vector<int> () );
         U.clear();
      }
   }
   catch(const exception& e)
   {
      cerr << "Excecao ocorrida no \'aplicarOperadorViabilidade()\': "
                     << e.what() << endl;
   }
} /* Fim de viabilizarIndividuoParaPopulacaoInicial()*/
/*****************************************************/

/**********************************************************/
void AlgoritmoGenetico::removerColunasRedundantes(
              vector<int> &OCjS, vector<int> &Owi,
              bitset<NUMERO_COLUNAS> &OFilho)
/**********************************************************
 * Objetivo: Retira uma coluna redundante da
 * solucao come�ando das colunas de alto custo
 * primeiro, devolvendo OCjS, Owi e OFilho atualizados.
 * Coluna redundante e' aquela que ao sair
 * "nao inviabiliza" a solucao.
 * item iv pag. 397 do Beasley algoritmo guloso
 *********************************************************/
{
vector<int>::iterator itI = OCjS.begin(),
                      itF = OCjS.end();
bool     Redundante; // variavel de controle
   // descobre quais colunas da solucao atual OCjS
   // sao redundantes e poe -1 no seu lugar
   try
   {
      while(1)
      {
         Redundante = true;
         --itF; // posiciona itF do final para o in�cio come�ando no ultimo
         for(int i = 0; i < ip.nlinhas; ++i)
            if(ip.pAij[i][*itF] == 1)
            {
               if(Owi.at(i) >= 2)
                  continue;
               else
               {
                  Redundante = false;
                  break;
               }
            }
         // A coluna *itF � redundante?
         if(Redundante)
         {  // SIM:
            OFilho.reset((size_t)(*itF)); // atualiza o filho
            diminuirWi(Owi, *itF);     // depois o Wi
            *itF = -1; // por fim o OCjS
         }
         // N�O, mas chegou no primeiro elemento de OCjS?
         if(itF == itI)
            break; // SIM, terminou de percorrer OCjS
      } // fim while(1)

      // Depois de atualizar Owi, OFilho e OCjS, retira os -1 de S
      itI = OCjS.begin();
      itF = OCjS.end();
      while(itI != OCjS.end())
         if((*itI) == -1)
            itI = OCjS.erase(itI);
         else
            ++itI;
   }
   catch(const exception& e)
   {
      cerr << "Excecao: ocorrida no \'removerColunasRedundantes()\' "
                        << e.what() << endl;
   }
} /* Fim do removerColunasRedundantes()          */
/*************************************************/

void AlgoritmoGenetico::diminuirWi(vector<int> &AtualWi, int AColuna)
{
   for(int i = 0; i < ip.nlinhas; ++i)
      if(ip.pAij[i][AColuna] == 1)
         if(AtualWi[i] >= 2)
            --AtualWi[i];
} /* fim do diminuirWi() */
/*************************/

/**********************************************/
void AlgoritmoGenetico::inicializarWi(
         bitset<NUMERO_COLUNAS> &Ofilho, vector<int> &Owi)
/***********************************************
 * Objetivo: Popular wi em funcao do filho atual
 **********************************************/
{
   try
   {
      for(int j = 0; j < (int)Ofilho.size(); ++j)
         if(Ofilho.test(j))
            for(int i = 0; i < ip.nlinhas; ++i)
               if(ip.pAij[i][j] == 1)
                  ++Owi[i];
   }
   catch(exception& e)
   {
      cerr << "Excecao gerada no \'inicializarWi()\' : "
                  << e.what() << endl;
   }
} /* Fim de inicializarWi() */
/****************************/

/********************************************/
void AlgoritmoGenetico::inicializarS(
         bitset<NUMERO_COLUNAS> &Ofilho, vector<int> &S)
/*********************************************
 * Objetivo: Criar o conjunto S com os indices
 * das colunas que est�o na solu��o.
 ********************************************/
{
   try
   {
      // Inicializa S com os indices dos elementos em Filho  em '1'
      for(size_t j = 0; j < Ofilho.size(); ++j)
         if(Ofilho.test(j))
            S.push_back(j);
   }
   catch(exception& e)
   {
      cerr << "Excecao ocorrida no \'\' : " << e.what() << endl;
   }
} /* Fim de inicializarS() */
/***************************/

/*******************************************/
void AlgoritmoGenetico::inicializarU(
         vector<int> &Owi, vector<int> &CjU)
/********************************************
 * Objetivo: Criar o conjunto U das linhas
 * nao cobertas pelo vetor de solucao atual
 * que s�o os elementos zerados em Owi
 ********************************************/
{
   try
   {
      for(int i = 0; i < (int)Owi.size(); ++i)
         if(Owi.at(i) == 0)
            CjU.push_back(i);
   }
   catch(const exception& e)
   {
      cerr << "Excecao ocorrida no \'inicializarU()\' : "
                     << e.what() << endl;
   }
}/* fim do inicializarU()*/
/*************************/

/******************************************************/
void AlgoritmoGenetico::adicionarColuna(
         vector<int> &OCjU, vector<int> &OCjS,
         bitset<NUMERO_COLUNAS> &Filho, vector<int> &wi)
/*******************************************************
 * Objetivo: Adicionar colunas necess�rias para
 * tornar a Solu��o vi�vel.
 ******************************************************/
{
   double OMenor = (double)10000.0;
   int    MelhorCol = -1;
   try
   {
      // Se OCjU.size() for ZERO sai sem adicionar coluna
      for(int i = 0; i < (int) OCjU.size(); ++i)
      {
         if(OCjU.at(i) == -1)
            continue;
         for(int j = 0; j < ip.ncolunas; ++j)
            if(ip.pAij[OCjU.at(i)][j] == 1)
               if(( ((double)ip.pFO[j])/contarQtasCobrira(OCjU, j) ) < OMenor)
               {
                  OMenor = ((double)ip.pFO[j])/contarQtasCobrira(OCjU, j);
                  MelhorCol = j;
               }
         if(MelhorCol == -1) // por seguran�a, pois uma coluna ao menos
         {                    // sempre dever� cobrir!
            cerr << "O conjunto F � invi�vel" << endl;
            system("pause");
         }
         OCjS.push_back(MelhorCol); // Adicionar nova coluna no final de S
         Filho.set((size_t)MelhorCol, 1); // Poe bit 1 da nova coluna no
                                          // lugar certo do Filho
         atualizarWi(wi, MelhorCol); // Atualiza Wi
         atualizarU(OCjU, MelhorCol); // Atualiza U (p�e -1 em U)

         MelhorCol = -1;
         OMenor = (double)10000.0;
      } // proximo elemento do U, at� todos terem sido analisados
      if(OCjU.size())
         ordenarS(OCjS);
   }
   catch(const exception& e)
   {
      cerr << "Excecao ocorrida no \'adicionarColuna()\' : "
                     << e.what() << endl;
   }
} /* fim do metodo adicionarColuna() */
/*************************************/

/**************************************************/
void AlgoritmoGenetico::atualizarU(
                  vector<int> &OCjU, int NovaColuna)
/***************************************************
* Objetivo: P�e -1 nos elementos de U conforme
* vao sendo cobertos pelas novas colunas
* adicionadas a S.
* Aten��o: � o U := U - Beta jota
****************************************************/
{
   try
   {
/*
      vector<int>::iterator it;
      for(it = OCjU.begin(); it != OCjU.end(); )
         if(ip.pAij[*it][NovaColuna] == 1)
            OCjU.erase(it);
         else
            ++it;
*/
      for(size_t i = 0; i < OCjU.size(); ++i)
         if(OCjU.at(i) == -1)
            continue;
         else if(ip.pAij[OCjU[i]][NovaColuna] == 1)
            OCjU[i] = -1;
   }
   catch(exception& e)
   {
      cerr << "Excecao ocorrida no \'atualizarU()\' : "
                     << e.what() << endl;
   }
} /* Fim do atualizarU() */
/*************************/

void  AlgoritmoGenetico::ordenarS(
                  vector<int> &UmVector)
{
   int NumQq;
   int MaxI = UmVector.size() - 1;
   int MaxJ = UmVector.size();
   try
   {
      for(int i = 0; i < MaxI; ++i)
          for(int j = i + 1; j < MaxJ; ++j)
            if(UmVector[i] > UmVector[j])
            {
               NumQq = UmVector[i];
               UmVector[i] = UmVector[j];
               UmVector[j] = NumQq;
            }
   }
   catch(exception& e)
   {
      cerr << "Excecao ocorrida no \'ordenarS()\' : " << e.what() << endl;
   }
} /* Fim do ordenarS() */
/***********************/

/**********************************************/
double AlgoritmoGenetico::contarQtasCobrira(
               vector<int> &OCjU, int UmaColuna)
/***********************************************
 * Objetivo: contar quantas linhas desco-
 * bertas esta nova coluna 'UmaColuna' cobrir�
 * se fizer parte da solu��o.
 * Aten�ao: � o |U intersec��o com Beta jota|
 * (a) de 'iii' a pag. 397
 **********************************************/
{
   int Contagem = 0;
   try
   {  // Entrou neste m�todo � porque UmaColuna cobre
      // ao menos uma vez a linha sem cobertura
      // e o OCjU.size() � sempre maior do que ZERO.
      for(size_t i = 0; i < OCjU.size(); ++i)
         // Qdo cobre
         if(OCjU.at(i) == -1)
            continue;
         else if(ip.pAij[OCjU.at(i)][UmaColuna])
            ++Contagem; // Incrementa
   }
   catch(const exception& e)
   {
      cerr << "Excecao ocorrida no \'contarQtasCobrira()\' : "
                     << e.what() << endl;
   }

   return (double)Contagem;
} /* fim do metodo contarQtasCobrira() */
/**************************************/

void AlgoritmoGenetico::imprimirGeracaoAtual(void)
{
   for(int i = 0; i < N; ++i)
   {
      imprime_individuo(UmaGeracao.at((size_t) i));
      verificarCobertura(UmaGeracao.at((size_t) i));
   }
} // fim do imprimirGeracaoAtual()
/*********************************/
