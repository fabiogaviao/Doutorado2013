____PARA A VERS�O COMPLETA COM IMAGENS ABRIR ARQUIVO "MANUAL DE INSTRU��ES" NA PASTA /DOCS________



Manual de instru��es da aplica��o ANDA.







Por Lucas Amaral



__Introdu��o:
Esta aplica��o foi criada com o objetivo de atender �s necessidades da tese de doutorado do professor orientador F�bio gavi�o.
Suas caracter�sticas s�o o resgate de dados previamente inseridos pela aplica��o no banco de dados local da m�quina, e processamento para gera��o de dois arquivos espec�ficos, Apt.ros (Arquivo da Primeira Transi��o) e Adj.ros ( Arquivo de Dura��o de Jornadas ), que ser�o utilizados na tese � Uma nova proposta para a constru��o de linhas de trabalho (lines of work) em problemas de rostering�.




__Iniciando a aplica��o
     Primeiramente � necess�rio a configura��o do banco de dados MySQL no computador onde ser� utilizada a aplica��o. � importante lembrar que se o banco de dados n�o for migrado, a aplica��o n�o funcionar� corretamente, pois utiliza nomes espec�ficos para o banco, tabela e campos.
Considerando que o SGDB est� configurado e o banco de dados rodando no computador, uma vez iniciada a aplica��o a seguinte janela ser� mostrada:

	_Viagens: 
		Esta tabela mostra todas as viagens cadastradas no banco de dados, inicialmente ela aparece vazia pois n�o foi aplicado nenhum comando SQL de sele��o.
	_Inser��o:
		Se��o que recebe os dados que ser�o inseridos no BD
	_Busca:
		Se��o respons�vel pelo filtro aplicado por comandos SQL, que refletir�o na tabela de Viagens.

__Inserindo dados
	A inser��o de dados � bem simples, por�m pode causar problemas caso n�o seja realizada corretamente. Os locais de partida e chegada devem ser padronizados e sem acentos de forma que n�o haja ambiguidade no processamento dos dados.
Ex.: Uma vez inserido o �Local de Partida� como �Belo Horizonte�, todos os demais campos referentes � cidade de Belo Horizonte devem coincidir com a inser��o feita no banco.

	_Local de Partida e Chegada:
		Refere-se � cidade de origem e destino da viagem.
	_Hor�rio de Partida e Chegada:
		Refere-se aos hor�rios de partida da cidade origem e chegada � cidade destino. Devem-se inserir as horas e minutos separados e no formato 24hrs como na imagem exemplo.
	_N�mero do v�rtice:
		Este campo � importante pois os dados s�o organizados e filtrados por ele, � essencial que o n�mero do v�rtice seja �nico, para que assim seja poss�vel mostrar apenas as viagens definidas na se��o de busca.


__Busca e ordena��o
	Assim que houver dados no banco, � poss�vel filtrar e ordenar os dados que s�o mostrados na tabela de viagens.

	_V�rtices:
		Inserindo os valores de limite inferior e superior ( 0 a 144 na imagem exemplo ) e clicando e buscar, a tabela de viagens mostrar� apenas as viagens com os n�meros de v�rtice dentro do limite imposto.
	_Ordenar:
		No Combo Box est�o as ordena��es da lista (as ordena��es s� se aplicam na lista inteira, sem a restri��o de n�mero de v�rtice)e s�o aplicadas ao clicar em Ordenar Grid, as ordena��es poss�veis s�o:
			N�mero do V�rtice
			Cidade Partida
			Cidade Destino
			Dura��o Viagem
	_Limpar:
		Remove todas as restri��es e filtros e mostra a lista de viagens por completo.


__Gerando arquivos APT e ADJ
	Este � um processo simples que leva apenas dois passos. Uma vez iniciada a aplica��o, e os dados sendo mostrados na tabela, � preciso que sejam selecionadas as viagens que ser�o processadas. No exemplo foram selecionadas as viagens de n�mero de v�rtice 1 a 8.


	Ap�s selecionar os v�rtices desejados para o processamento, � necess�rio clicar no menu Arquivo e depois na op��o Gerar Apt e Adj, ap�s estes passos ser� mostrada uma mensagem que indicar� que os arquivos foram gerados.


__Abrindo arquivos gerados
	� necess�rio que os arquivos sejam gerados toda vez que o programa for iniciado, pois ao abrir a aplica��o os arquivos ser�o apagados.
	Para abrir os arquivos gerados, deve-se clicar no menu Arquivo e em seguida Abrir Adj.ros ou Apt.ros.
	Obs.: � poss�vel abrir atrav�s da aplica��o apenas um arquivo por vez.

	
	O arquivo ser� aberto pelo Notepad, e � necess�rio que o mesmo seja fechado antes de abrir outro.



