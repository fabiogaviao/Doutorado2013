//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("gHeuLWB.res");
USEUNIT("Controle.cpp");
USEUNIT("..\..\Doutorado2012\pCltEtt\Projeto\Construtiva\ModelagemPorGrafo\Grafo.cpp");
USE("..\..\Doutorado2012\pCltEtt\Projeto\Construtiva\ModelagemPorGrafo\ParametroFaseConstrutiva.h", File);
USEUNIT("..\..\Doutorado2012\pCltEtt\Projeto\Construtiva\ModelagemPorGrafo\DistribuidorDePeso.cpp");
USEUNIT("AlgoritmoGenetico.cpp");
USEUNIT("LeitorSCP.cpp");
//---------------------------------------------------------------------------
#include "Controle.h"

WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
   try
   {
      Controle *pC = new Controle();
      pC->ativar();
      Application->Initialize();
      Application->Run();
   }
   catch (Exception &exception)
   {
      Application->ShowException(&exception);
   }
   return 0;
}
//---------------------------------------------------------------------------
