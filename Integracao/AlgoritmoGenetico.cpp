//---------------------------------------------------------------------------

#pragma hdrstop

#include "AlgoritmoGenetico.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

/*****************************************/
int AlgoritmoGenetico::calcularMi(void)
/*****************************************
 * Data de criação: 27-mar-2013 às 14:31h
 * Objetivo: Antes este valor de Mi era
 * fixo e tinha um valor para cada conjunto
 * de scp de beasley que se estava testando.
 * Mi = media do número de repetições de
 * colunas na população inicial.
 *****************************************/
{
   Mi = (double)2.0;
   return Mi;
} /* Fim do calcularMi() */
/*************************/

/*****************************************************/
void AlgoritmoGenetico::calcularDensidadeMatCob(void)
/******************************************************
 * Data de criação: 26-mar-2013 às 15:00h
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
      // OU PARA O NOSSO CASO ESPECÍFICO:
      // DensMatCob = ((double)ndr)/((double)ip.nlinhas);

 } /* Fim do calcularDensidadeMatCob() */
 /**************************************/

/***************************************************************************/
void AlgoritmoGenetico::criarPopulacaoInicial(InstanciaProblema UmaInstancia)
/****************************************************************************
 * Objetivos: Calcular o tamanho da população inicial
 * e criar os individuos desta população.
 ****************************************************************************/
{
   ip = UmaInstancia; // Problema gerado a partir de F:
                      // Parâmetros: fo, restrições, nlinhas, ncolunas.
   calcularDensidadeMatCob();
   // Quantidade de indivíduos da população inicial para garantir
   // cobrir no mínimo todas as linhas de F
   // N = floor(DensMatCob*((double)ip.ncolunas) * calcularMi());
   // Qualificação 03 de maio de 2013
   //N = ceil( (ip.ncolunas/((double)1.0/DensMatCob)) * calcularMi() );
   N = ceil(calcularMi() * ip.ncolunas * DensMatCob * 0.1);
   if(N <= 4)
      N = 5;
   bitset<NUMERO_COLUNAS> NovoIndividuo;
   NovoIndividuo.reset(); // Zera o novo individuo!

   vector<int> AlfaIk;
   vector<int> Wi; // Vetor de "nlinhas" de comprimento.
                   // Guarda qtas vezes cada linha de Aij
                   // é coberta pela população inicial
   int ValorSorteado = 0;
   try
   {
      // Gera população inicial de N indivíduos
      for(int p = 0; p < N; ++p)
      {
         // Para cada linha de pAij
         for(int i = 0; i < ip.nlinhas; ++i)
         {
            // Sorteia uma coluna que cobre e a adiciona no NOvoIndividuo
            // tornando assim viavel o novo indivíduo
            for(int j = 0; j < ip.ncolunas; ++j)
               if(ip.pAij[i][j] == 1)
                  AlfaIk.push_back(j);
            // Sorteia uma coluna de AlfaIk.
            // Qualificação 03 de maio de 2013: tem que ver se size é 1.
            // Se for tem que seleciona-la sem fazer sorteio.
            if(AlfaIk.size() == 1)
               ValorSorteado = AlfaIk.at(0);
            else
               ValorSorteado = AlfaIk.at( (size_t)(rand()%AlfaIk.size()) );
            // seta o bit desta coluna no NovoIndividuo
            NovoIndividuo.set((size_t)ValorSorteado);
            AlfaIk.clear(); // zerar antes de fazer proxima linha!
         } // próxima linha de Aij
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
      } // retorna ao primeiro for para fazer novo individuo (até 'N')
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
 * os wi's são maiores ou iguais a 2, a mesma é
 * redundante e pode ser apagada da solução final.
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
                           // j é redundante!
   // Para cada bit do indivíduo UmIndividuo (colunas)
   try
   {
      for(int j = 0; j < ip.ncolunas; ++j)
         // Verificar se o mesmo é falso ou verdadeiro.
         if(UmIndividuo.test((size_t)j))
         {// Sendo verdadeiro:
            // Percorrer todas linhas da matriz de cobertura
            // referentes a esta coluna 'j'
            for(int i = 0; i < ip.nlinhas; ++i)
            {  // Se a linha percorrida for coberta pela coluna 'j'
               if( ip.pAij[i][j] == 1 )
                  // verificar se o OWi[i] é >= 2
                  if(OWi[i] >= 2)
                     // Se sim, a coluna 'j' pode ser redundante.
                     // Para isto devem ser verificadas as demais
                     // linhas desta coluna
                     continue;
                  // Caso contrário: wi[i] é no máximo 1 e ao
                  // tirar a coluna (subtrair 1) ela deixará de
                  // cobrir a linha 'i'. Em outras palavras, a
                  // coluna faz realmente falta (não é redundante)
                  // Marcar Redundante false!
                  else
                  {
                     Redundante = false;
                     break; // não precisa continuar até a ultima linha
                  }
            }
            // Acabou de percorrer todas linhas de uma coluna 'j' ou
            // descobriu antecipadamente que a coluna 'j' não é Redundante
            if(Redundante == true)
            {
               // Decrementar o OWi
               for(int i = 0; i < ip.nlinhas; ++i)
                  if( ip.pAij[i][j] == 1 )
                     --OWi[i];
               // tirar a coluna 'j' da solução
               UmIndividuo.reset( (size_t)j);
            }
            // começa tudo outra vez para o proximo bit (j) em UmIndividuo
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
 * coluna do indivíduo cobre e, para cada linha,
 * ao subtrair 1 nas respectivas posições de Owi
 * não der 0 em nenhuma a coluna é redundante e
 * pode ser removida da solução final.
 ********************************************/
{
   bool flag = true;
   // Para cada bit dos NUMERO_COLUNAS bits do indivívuo bs (colunas)
   try
   {
      for(int j = 0; j < NUMERO_COLUNAS; ++j)
         // Verificar se o mesmo é falso ou verdadeiro.
         // Sendo verdadeiro
         if(bs.test((size_t)j))
         {
            // Percorrer todas linhas da matriz de cobertura
            // referentes a esta coluna 'j'
            for(int i = 0; i < ip.nlinhas; ++i)
               // Se a linha for coberta pela coluna 'j'
               if( ip.pAij[i][j] == 1 )
                  // verificar se o Owi[i] é >= 2
                  if(Owi[i] >= 2)
                     // Se sim, coluna 'j' pode ser redundante
                     // verificar as demais coberturas
                     continue;
                  // Caso contrário: wi[i] é no máximo 1 e ao
                  // tirar a coluna (subtrair 1) ela deixará de
                  // cobrir a linha 'i'. Em outras palavras, a
                  // coluna faz realmente falta.
                  // Marcar flag false!
                  else
                     flag = false;
            // No final, após percorrer todas as linhas
            // Se flag é true
            if(flag == true)
            {
               // decrementar o wi
               for(int i = 0; i < ip.nlinhas; ++i)
                  if( ip.pAij[i][j] == 1 )
                     --Owi[i];
               // tirar a coluna 'j' da solução
               bs.reset( (size_t)j);
            }
            // começa tudo outra vez para o proximo 'j' em bs
            flag = true;
         } // fim do 'if' que testa todos os bits do invíduo
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
   // Para cada bit dos NUMERO_COLUNAS de um indivíduo
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
   // Qualificação 04 de maio de 2013
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
//      cerr << "Quatro pais do torneio binário tem o mesmo fitness!" << endl;
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
 * Objetivo: gera novo indivíduo pelo cruzamento de genes
 * existentes na geração atual.
 * Baseia-se no algoritmo de torneio binário
 * Nesse torneio primeiro devem ser selecionados 2 pools
 * de 2 individuos cada. Escolhe os dois melhores
 * individuos dos 4. Gera um filho copiando os
 * genes iguais dos pais e transferindo os genes
 * diferentes respeitando-se a proporcao
 * de fitness de cada um.
 * NOTA: Se tem 80 bits diferentes para transferir
 * e o melhor individuo possui 60% de participacao na composição
 * do fitness global, primeiro são transferidos 48 bits
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
   // fp1 é melhor se for menor
   if(fp1 < fp2)
      p = (double)fp2 / (double)(fp1 + fp2); // obtem maior percentagem
   else
      p =(double)fp1 / (double)(fp1 + fp2); // obtem maior percentagem
   int   bits_transferir;
   bits_transferir = (int)ceil(bits_desiguais*p);
   // Antes de fazer a troca de bits é interessante resetar
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
 * bits diferentes. Estes deverão ser tirados
 * do melhor pai primeiro até esgotar. Depois deveremos
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
            // Não precisa do else pois Filho foi inicializado
            // à priori com tudo 0.
      // se os dois pais não tiverem nenhum bit diferente encerra
      // aqui devolvendo Filho sem mudança
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
   // calcular a taxa de mutação para o
   // problema em questao.
   int   pos;
   // Pega os valores fixos de mf, mc e mg da menor instância!
   mf = MUTATION_COEF[INDICE_PROBLEM_SET][0];
   mc = MUTATION_COEF[INDICE_PROBLEM_SET][1];
   mg = MUTATION_COEF[INDICE_PROBLEM_SET][2];
   // a variável independente nesta equação é o t (número de gerações)
   // tmb = taxa de mudança de bits no tempo
   tmb = (int)ceil( mf/( 1.0 + exp(-4.0 * mg * (t - mc)/mf) ) );
   if(tmb <= 0)
   {
      cerr << "tmb <= 0" << endl;
      system("pause");
   }
   //   assert(tmb > 0);
   // Qualificação:
   // Não é chamada a calcularSElite para reduzir o grau de cobrimento
   // do domínio do problema e consequentemente ter um número de
   // indivíduos menores na PI.

   // calcularSElite();

   // Qualificação:
   // Mudar "tmb" bits de Filho usando uma estratégia para mudar os bits
   // mais à esquerda (mais à direita implica em colunas de alto custo)
   for(int j = 0; j < tmb; ++j)
   {
      // apenas na metade mais à esquerda
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
 * é a união de todos AlfaIk, ou seja, das
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
   // Põe em Selite os valores das colunas encontradas anteriormente
   for(int j = 0, i = 0; j < ip.ncolunas; ++j)
      if(bs.test((size_t)j) == true)
         pSelite[i++] = j;
}/* fim da calcularSElite() */
/*****************************/

/****************************************************************************/
void AlgoritmoGenetico::obterMelhorIndividuo(bitset<NUMERO_COLUNAS> &ASolucao)
/*****************************************************************************
 * Objetivos: Ao invés de devolver o índice do melhor dos individuos
 * na população atual (UmaGeracao) optei por devolver o indivíduo
 * selecionado.
 ****************************************************************************/
{
   vector<int> CustoIndividuosGeracaoAtual;
   // Geracao atual == UmaGeracao
   try
   {
      for( size_t i = 0; i < UmaGeracao.size(); ++i)
         CustoIndividuosGeracaoAtual.push_back(calcularFitnessIndividuo(UmaGeracao.at((size_t)i)));
      // selecionar o indice do individuo de menor custo: é o
      // melhor da melhor geração. Portanto a solução esperada!
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
 * seta a variavel "duplicidade" para true na área
 * privada da classe.
 * Essa variável é lida por outro método para ajudar o ativar()
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
 * Quando não tiver um pior interrompe com assert()
 * Algoritmo:
 *    Calcular fitness do novo indivíduo
 *    Obter vetor de fitness referente a geração atual
 *    Achar na geração atual um individuo com fitness pior
 *    do que o fitness do novo individuo
 *    Substituir o individuo com fitness pior pelo novo
 *       individuo
 * Alternativa: caso não tenha pior individuo na geração atual
 * simplesmente não inclui este individuo e continua o
 * processo de cruzamento x mutação x viabilização x check de duplicação.
 ***********************************************************************/
{
   // Calcular o fitness do novo indivíduo (fni)
   int FitnessNovoIndividuo = calcularFitnessIndividuo(NovoIndividuo);

   // Obter matriz de duas colunas contendo na primeira coluna
   // o valor de fitness referente a cada individuo da
   // geração atual e na segunda coluna a sua posição relativa
   // ao início da geração atual
   multimap<int, int> MapaDeFitnessDaGeracao;
   obterMapaGeracaoAtual(MapaDeFitnessDaGeracao);

   // Achar no mapa o índice do primeiro individuo que tem fitness
   // maior do que o fitness do novo indivíduo.
   int IndiceDoPrimeiroSuperior = 0;

   // Qualificação
   if(acharPrimeiroIndividuoPiorNaGeracao(FitnessNovoIndividuo,
                  MapaDeFitnessDaGeracao, &IndiceDoPrimeiroSuperior))
      UmaGeracao[IndiceDoPrimeiroSuperior] = NovoIndividuo;

   // Incrementa a geração mesmo que o filho não tenha sido incluido na população
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
   vector<int> S; // cj de colunas na solução (no filho)
   S.reserve((size_t) ip.ncolunas);
   vector<int> U; // o cj de linhas não cobertas
   try
   {
//      imprime_individuo(Ofilho);
      Wi.assign((size_t)ip.nlinhas, 0); // gera Wi:
      inicializarWi(Ofilho, Wi); // numero de colunas do filho que
                                 // cobrem cada linha de Aij
      inicializarS(Ofilho, S); // cada elemento de S guarda a
                               // coluna da solução (Ofilho)
      inicializarU(Wi, U); // cada elemento de U é uma linha não
                           // coberta pelo filho
      // parte (a) e (b) da letra (iii) do Alg p397
      // de Beasley e Chu
      adicionarColuna(U, S, Ofilho, Wi);
      // faz uma pequena otimização local
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
 * Objetivo: Alg p398, item 4, parágrafo 4.1 letra 'ii' Beasley&Chu
 * Diferença para o aplicarOperadorViabilidade é que neste
 * método não tem a eliminação de colunas redundantes. Esta
 * tarefa deve ser feita por quem chamar este método, no caso
 * pela criarPopulacaoInicial(). Para cada linha ele simplesmente
 * adiciona uma coluna que a cobre. Assim a solução é sempre
 * viável.Mas há a possibilidade de achar uma linha que seja
 * coberta por coluna fora de AlfaIk!
 ******************************************************************/
{
   vector<int> Wi;// numero de vezes que cada linha 'i' é coberta pelo filho
   vector<int> S; // cj de colunas na solução (no filho)
   S.reserve((size_t) ip.ncolunas);
   vector<int> U; // o cj de linhas não cobertas
   try
   {
//      imprime_individuo(Ofilho);
//      verificarCobertura(Ofilho);
      Wi.assign((size_t)ip.nlinhas, 0); // gera Wi:
      inicializarWi(Ofilho, Wi); // numero de colunas do filho que
                                 // cobrem cada linha de Aij
      inicializarS(Ofilho, S); // cada elemento de S guarda a
                               // coluna da solução (Ofilho)
      inicializarU(Wi, U); // cada elemento de U é uma linha não
                           // coberta pelo filho
      // letras (a) e (b) da parte (iii) do Alg p397
      // de Beasley e Chu é usada aqui ao invés de seguir
      // como em Beasley&Chu pag. 398, letras (a) e (b) da parte (ii)
      // devido a uma questão de melhorar a população inicial e
      // evitar soluções não viáveis na PI.
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
 * solucao começando das colunas de alto custo
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
         --itF; // posiciona itF do final para o início começando no ultimo
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
         // A coluna *itF é redundante?
         if(Redundante)
         {  // SIM:
            OFilho.reset((size_t)(*itF)); // atualiza o filho
            diminuirWi(Owi, *itF);     // depois o Wi
            *itF = -1; // por fim o OCjS
         }
         // NÃO, mas chegou no primeiro elemento de OCjS?
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
 * das colunas que estão na solução.
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
 * que são os elementos zerados em Owi
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
 * Objetivo: Adicionar colunas necessárias para
 * tornar a Solução viável.
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
         if(MelhorCol == -1) // por segurança, pois uma coluna ao menos
         {                    // sempre deverá cobrir!
            cerr << "O conjunto F é inviável" << endl;
            system("pause");
         }
         OCjS.push_back(MelhorCol); // Adicionar nova coluna no final de S
         Filho.set((size_t)MelhorCol, 1); // Poe bit 1 da nova coluna no
                                          // lugar certo do Filho
         atualizarWi(wi, MelhorCol); // Atualiza Wi
         atualizarU(OCjU, MelhorCol); // Atualiza U (põe -1 em U)

         MelhorCol = -1;
         OMenor = (double)10000.0;
      } // proximo elemento do U, até todos terem sido analisados
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
* Objetivo: Põe -1 nos elementos de U conforme
* vao sendo cobertos pelas novas colunas
* adicionadas a S.
* Atenção: é o U := U - Beta jota
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
 * bertas esta nova coluna 'UmaColuna' cobrirá
 * se fizer parte da solução.
 * Atençao: é o |U intersecção com Beta jota|
 * (a) de 'iii' a pag. 397
 **********************************************/
{
   int Contagem = 0;
   try
   {  // Entrou neste método é porque UmaColuna cobre
      // ao menos uma vez a linha sem cobertura
      // e o OCjU.size() é sempre maior do que ZERO.
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
