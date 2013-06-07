____PARA A VERSÃO COMPLETA COM IMAGENS ABRIR ARQUIVO "MANUAL DE INSTRUÇÕES" NA PASTA /DOCS________



Manual de instruções da aplicação ANDA.







Por Lucas Amaral



__Introdução:
Esta aplicação foi criada com o objetivo de atender às necessidades da tese de doutorado do professor orientador Fábio gavião.
Suas características são o resgate de dados previamente inseridos pela aplicação no banco de dados local da máquina, e processamento para geração de dois arquivos específicos, Apt.ros (Arquivo da Primeira Transição) e Adj.ros ( Arquivo de Duração de Jornadas ), que serão utilizados na tese ” Uma nova proposta para a construção de linhas de trabalho (lines of work) em problemas de rostering”.




__Iniciando a aplicação
     Primeiramente é necessário a configuração do banco de dados MySQL no computador onde será utilizada a aplicação. É importante lembrar que se o banco de dados não for migrado, a aplicação não funcionará corretamente, pois utiliza nomes específicos para o banco, tabela e campos.
Considerando que o SGDB está configurado e o banco de dados rodando no computador, uma vez iniciada a aplicação a seguinte janela será mostrada:

	_Viagens: 
		Esta tabela mostra todas as viagens cadastradas no banco de dados, inicialmente ela aparece vazia pois não foi aplicado nenhum comando SQL de seleção.
	_Inserção:
		Seção que recebe os dados que serão inseridos no BD
	_Busca:
		Seção responsável pelo filtro aplicado por comandos SQL, que refletirão na tabela de Viagens.

__Inserindo dados
	A inserção de dados é bem simples, porém pode causar problemas caso não seja realizada corretamente. Os locais de partida e chegada devem ser padronizados e sem acentos de forma que não haja ambiguidade no processamento dos dados.
Ex.: Uma vez inserido o ‘Local de Partida’ como ‘Belo Horizonte’, todos os demais campos referentes à cidade de Belo Horizonte devem coincidir com a inserção feita no banco.

	_Local de Partida e Chegada:
		Refere-se à cidade de origem e destino da viagem.
	_Horário de Partida e Chegada:
		Refere-se aos horários de partida da cidade origem e chegada à cidade destino. Devem-se inserir as horas e minutos separados e no formato 24hrs como na imagem exemplo.
	_Número do vértice:
		Este campo é importante pois os dados são organizados e filtrados por ele, é essencial que o número do vértice seja único, para que assim seja possível mostrar apenas as viagens definidas na seção de busca.


__Busca e ordenação
	Assim que houver dados no banco, é possível filtrar e ordenar os dados que são mostrados na tabela de viagens.

	_Vértices:
		Inserindo os valores de limite inferior e superior ( 0 a 144 na imagem exemplo ) e clicando e buscar, a tabela de viagens mostrará apenas as viagens com os números de vértice dentro do limite imposto.
	_Ordenar:
		No Combo Box estão as ordenações da lista (as ordenações só se aplicam na lista inteira, sem a restrição de número de vértice)e são aplicadas ao clicar em Ordenar Grid, as ordenações possíveis são:
			Número do Vértice
			Cidade Partida
			Cidade Destino
			Duração Viagem
	_Limpar:
		Remove todas as restrições e filtros e mostra a lista de viagens por completo.


__Gerando arquivos APT e ADJ
	Este é um processo simples que leva apenas dois passos. Uma vez iniciada a aplicação, e os dados sendo mostrados na tabela, é preciso que sejam selecionadas as viagens que serão processadas. No exemplo foram selecionadas as viagens de número de vértice 1 a 8.


	Após selecionar os vértices desejados para o processamento, é necessário clicar no menu Arquivo e depois na opção Gerar Apt e Adj, após estes passos será mostrada uma mensagem que indicará que os arquivos foram gerados.


__Abrindo arquivos gerados
	É necessário que os arquivos sejam gerados toda vez que o programa for iniciado, pois ao abrir a aplicação os arquivos serão apagados.
	Para abrir os arquivos gerados, deve-se clicar no menu Arquivo e em seguida Abrir Adj.ros ou Apt.ros.
	Obs.: É possível abrir através da aplicação apenas um arquivo por vez.

	
	O arquivo será aberto pelo Notepad, e é necessário que o mesmo seja fechado antes de abrir outro.



