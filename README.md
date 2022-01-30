# Técnicas de Programação Avançada
## K-Way Merge, Merge Sort Externo & Hashing

### Aluno: Kelvin Lehrback
### Professor: Mateus Conrad
<br>

## Objetivo
1 - Ordenar arquivos com uma grande quantidade de registros utilizando ambos os métodos de ordenação (K-Way Merge & Merge Sort Externo). De modo a utilizar pouca quantidade de RAM.

2 - Criar uma tabela hash de acesso direto aos respectivos valores, dado uma chave. De modo a evitar colisão e priorizando eficiência na busca.

## Linguagem Utilizada
As seguintes linguagens de programação estavam disponíveis para serem utilizadas: C, C++ e Java.

Para a execução dessa tarefa, foi escolhida a  <b> linguagem C</b>. 

Por uma série de fatores, dentre eles: programação em baixo nível ("conversando" mais perto da memória RAM). De modo a agregar o conhecimento desse discente.

# O início
Criar a biblioteca principal desse projeto: <b> O gerenciador de buffer </b>.
Arquivos: [buffer.c](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/buffer.c) & [buffer.h](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/buffer.h).

## Um breve resumo sobre os arquivos <b>.C e .H</b>.
Os arquivos .C contém a implementação das funções utilizadas, enquanto os arquivos .H contém a definição dessas funções, bem como dos tipos abstratos de dados (TAD), que por ventura serão utilizados.

Para que outros programas utilizem as respectivas funções implementadas, basta incluir a biblioteca com <b>#include<"nome_biblioteca.h></b>.

## Continuando,
### Tomei a liberdade de quebrar certos protocolos e utilizarei sem pensar 2x, uma linguagem coloquial, para uma leitura mais agradável e fluída do que foi o desenvolvimento deste trabalho.

## Continuando²,
Os códigos estão muito bem comentados, e com excessão dos arquivos [k_way_merge.c](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/k_way_merge.c), [mergesort_externo.c](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/mergesort_externo.c) e [hashing.c](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/hashing.c), o restante bem dividido em .C e .H.

O maior empecilho deste trabalho é decorrente da sua maior força, a própria linguagem de programação (ou talvez seja o inexperiente programador que quis se aventurar nisso tudo? <b>Talvez</b>). 

<b>C</b>, uma linguagem de programação que faz EXATAMENTE o que você pedir pra ela fazer, cria variáveis a hora que você pedir, se tiver memória disponível, ela te dá, mas adivinha? Ela não pede de volta... Quer devolver a memória para o sistema? Faça você mesmo! 

<b>C</b> é uma linguagem amiga, pois te avisa quando estás prestes a fazer m&rd@, evitando assim que o PC exploda (okay, explodir talvez seja exagero, mas uma tela azul é bem comum). Veja bem, ela TENTA impedir você de fazer besteira, mas não quer dizer que ela vai te impedir. Pois bem, quer utilizar memória do sistema? Use! Quer devolver essa memória? Libere-a! Que acessar regiões de memória que não são suas? <b>Tente</b>.

## Seguindo,
Em resumo, precisava evitar de carregar um arquivo gigantesco INTEIRO na memória RAM, para isso o gerenciador de buffer foi desenvolvido. Dado o nome de um arquivo e o tamanho do buffer em BYTES, tudo estava pronto para ser utilizado!

A função principal do gerenciador (e por consequência a que me deu mais dor de cabeça (e por coincidência, a qual mais me orgulho)) é a <b> loadBuffer() </b>. Tenho total ciência que ela pode ser melhorada em inúmeros aspctos, e assim será feito posteriormente, pois percebi uma escassez na comunidade a respeito de algo desse tipo.

Para implementa-la, precisei entender como funciona o SEEK_SET em arquivos, a manipulação da função fseek, conceitos um pouco mais "avançados" de recursividade (amiga ou inimiga? Depende) e meu deus, TER CUIDADO COM A RAM, 

Comecei a estudar o VALGRIND para poder capturar todos os vazamentos de memória e/ou outras pendências que por ventura poderiam acontecer. Porém, o estudo de tal ferramenta foi iniciado de modo tardio, não sendo possível uma aplicação completa no respectivo projeto. Porém, deixei os comandos utilizados bem como o arquivo de teste e o log que o mesmo gera (esse log foi rodado em cima do k-way merge). 
Arquivos do valgrind: [Veja aqui](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/tree/master/Studying_Valgrind).

As referências das funções e métodos utilizados para o gerenciador de buffer, bem como as demais bibliotecas, você poderá ver nas Referências deste documento (Andando em arquivos por ponteiros utilizando o fseek, como dividir uma string em tokens (strtok(recursivo e não recursivo))).

## Como Funciona o loadBuffer()?
Em resumo, não poderia carregar o arquivo inteiro, então o loadbuffer() o lê em partes dado o tamanho do buffer. 

Por exemplo, meu arquivo tem 50MB(MegaByte), e o tamanho do meu buffer é de apenas 300 bytes. Ou seja, iria ler um conteudo do arquivo cujo valor seja igual ou menor a 300 bytes. "Ah, mas porque menor?", por questão de consistência dos dados. Um registro completo equivale a uma linha do arquivo. 

Para os respectivos arquivos utilizados, uma linha tem cerca de 120 bytes, ou seja, eu consigo ler inteiramente 2 linhas do arquivo com um buffer de 300 bytes (240 bytes), os 60 bytes eu simplesmente descarto e deixo para a próxima leitura do buffer. 

Ao ler 240 bytes(e andar essa respectiva quantidade no arquivo), eu salvo a linha em que parei para a proxima leitura do buffer (próximo loadbuffer()), até chegar ao fim do arquivo, cujo o fim é denominado pelo tamanho do arquivo em bytes (foi também criada uma função para o respectivo cálculo).

Para um melhor entendimento, favor dar uma conferida nos respectivos arquivos citados anteriormente.

# Outras Bibliotecas
Conforme as ideias foram sendo postas em código, foi necessário a divisão em outras bibliotecas, sendo: [arquivo.c](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/arquivo.c) e [arquivo.h](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/arquivo.h), [utils_vector.c](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/utils_vector.c) e [utils_vector.h](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/utils_vector.h), cujo o próprio nome revela suas funções. Contêm métodos para manipulação de arquivos(verificar se arquivo existe, criar e deletar arquivos etc) e manipulação de vetores e matrizes, sendo o principal método desta segunda o <b>quick_sort()</b>.

# Tudo pronto, vamos começar.

## K-way Merge
Como funciona? Simples. Tenho um arquivo grande e dado o tamanho da minha RAM disponível, o quebro em vários arquivos e crio os meus buffers com base na RAM disponível para carda arquivo, cujo cálculo é: (número de arquivos criados + 1) / RAM disponível. 

Os arquivos criados já estão ordenados utilizando o quickSort().

"Porque o +1?", pelo simples fato de ter a necessidade de um buffer encarregado de guardar cada pedaço de todos os arquivos e descarregar no arquivo final.

### O número de ouro.
Existem um número de ouro da RAM que influência demais na execução do algorítmo. 

<i><b>Dado o tamanho da RAM e a quantidade de arquivos criados, o melhor valor para cada buffer é o que se aproxima do intervalo de 300~600 bytes</b></i>

Porque isso? Simplesmente pela quantidade de acesso ao disco X a quantidade de leitura de cada arquivo. Essa percepção só foi conseguido executando o algoritmo INÚMERAS E INÚMERAS vezes!

Para compilar o K-way Merge neste projeto, basta executar:

<i><b>gcc -Wall -pedantic buffer.c buffer.h utils_vector.c utils_vector.h arquivo.c arquivo.h k_way_merge.c -o exec</b></i>

Ou seja, estou compilando com o gcc, utilizando funções de verificação de código as bibliotecas utilizadas e o código do k-way merge, criando um executável de nome <b>exec</b>.

E para executar (exemplo):

<i><b>./exec Arquivos_Entrada/entrada1.csv Arquivos_Saida/saida_kway_merge.txt 10500</b></i>

Sendo: Arquivo de entrada, arquivo de saída e a quantidade de RAM disponível em bytes.

## Vamos ver como fica no terminal
Vale lembrar que todo o projeto foi feito no Ubuntu 18.04, utilizando VS Code.

Para fins de registro, os prints serão feitos utilizando o arquivo de apenas 10 mil registros. (Mas os testes com 500 mil e 1 milhão de registros estarão documentados logo a baixo).

Compilando e executando.

![kwaymerge image](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/imagens_readme/kwaymerge.png)

Ao final da execucao, temos o arquivo de entrada ordenado no respectivo arquivo de saída informado.

![arquivo ordenado](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/imagens_readme/kway_ordenado.png)

Por questões óbvias, impossível mostrar todo o arquivo ordenado no print, por conta disso, mostrarei apenas o fim deles (no lado direita da imagem, é possível ver o quão extenso o mesmo é).

Ao final da execução, os arquivos quebrados em partes menores são apagados, mas para ver como o algoritmo se comportar, vou comentar essa parte do código e executar novamente.

IMAGEM_AQUI
![arquivos criados](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/imagens_readme/arquivos_criados_kwaymerge.png)

Ou seja, foram criados vários arquivos temporários (e já ordenados) com base no arquivo de entrada e o tamanho do buffer para cada um, e depois disso, é realizado o K-way merge.

## Merge Sort Externo
Como funciona?

O mais complicado deste algoritmo foi entender (diferente do K-way merge que foi implementar (ou talvez o sentimento de ter sido mais complicado é devido a não ter nada pronto e a pouca experiência, mas enfim)). 

O mergesort externo utiliza 5 arquivos, sendo 1 arquivo de entrada, que gera dois arquivos de origem com a metade do conteúdo do arquivo de entrada cada e 2 arquivos de destinos em branco.

Esses 4 últimos arquivos são os que nos interessa. Os dois arquivos de origem serão posteriormente os de destino e os de destino serão os de origem. 

Ou seja, vamos supor que meu arquivo de entrada tenha 10 mil registros. O arquivo de origem_1 terá os 5 primeiros mil registros e o arquivo de origem_2 terá os outros 5 mil (essa divisão não é exata, simplesmente porque eu não conto linhas, eu conto a quantidade de bytes que o arquivos possui, ou seja, o seu tamanho). E internamente é criado dois arquivos em branco, que serão os de destino.

## Arquivos prontos, hora de executar.

Anteriormente foi citado o número de ouro do k-way merge. Aqui também temos o número de ouro que influencia de forma SIGNIFICATIVA no algorítmo. Mas vale ressaltar que o foco aqui é resultado e não velocidade. 

O número de ouro é com base no tamanho de linhas lidas de cada RUN (Sim, RUN e não RAM). O que é uma RUN? É a passada que o algoritmo faz em cada arquivo de origem.

Para um melhor entendimento, vamos tornar isso numa escala de 7.
Temos os seguintes registros:

Original: 40 05 80 43 09 12 20

Dividindo ao meio, temos:

Origem_1: 40 05 80 43<br>
Origem_2: 09 12 20<br>

Na primeira run é lido a primeira linha de cada arquivo, ordena e grava-se no arquivo de destino_1 (ou seja, serão lidos 40 e 09 e gravados 09 e 40 (pois 09 é menor que 40)). Em seguida, é lida a próxima linha de cada arquivo, ordenada e colocada no arquivo de destino_2, e assim sucessivamente. 

Então temos:<br>
Destino_1: 09 40 20 80<br>
Destino_2: 05 12 43

Agora, na segunda RUN é dobrado a quantidade de linhas lido de cada arquivo, os arquivos de Destino se tornam os de Origem e os de Origem se tornam os de destino, ou seja, durante a execução do algoritmo, os arquivos vão se revezando entre origem e destino.

Ao final desta segunda RUN, temos:

Origem_1: 05 09 12 40<br>
origem_2: 20 43 80

E por fim, temos a última RUN deste exemplo, com tamanho de linhas = 4.

Destino_1: 05 09 12 20 40 43 80<br>

Ou seja, o arquivo ordenado. 

Devido a esse vai e vem de dados entre os arquivos, a forma de verificar qual o arquivo contém o arquivo ordenado é simplesmente verificando o único que contém dados (pois depois de cada RUN, como os arquivos de destino se tornam os de origem, os de origem são limpos).

## Vamos ver como fica no terminal
Demais explicações a respeito dos prints foram dadas anteriormente.

Para compilar o projeto:<br>
<i><b>gcc -Wall -pedantic buffer.c buffer.h utils_vector.c utils_vector.h arquivo.c arquivo.h mergesort_externo.c -o exec</b></i>

Para executar:<br> 
<i><b>./exec Arquivos_Entrada/entrada1.csv 1</b></i>

![mergesorte externo terminal](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/imagens_readme/mergesorte_externo_terminal.png)

E temos então o arquivo ordenado.
![arquivo ordenado merge sort](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/imagens_readme/arquivo_ordenado_mergesort.png)

Ao término do merge, também é deletado os arquivos que não possuem conteúdos, restando apenas o arquivo ordenado.

# Testes com outros arquivos

__TABELA AQUI__

# Hashing Table
<i>"Hashtable é um conjunto de pares de valores-chave, que são organizadas no hash de suas respectivas chaves.

Quando você adiciona um elemento, ele é adicionado à tabela hash e seu código de hash correspondente é gerado automaticamente."</i>
<br>-[codigosimples.net](https://codigosimples.net/)

Para quem conhece python, conhece bem os dicionários e sabem o quão preciso eles são em termos de velocidade. Semelhante à isso, vamos construir a Hash Table em C.

A struct de cada elemento contêm os campos: key, phone, city, country e next_cell (Chave(Nome completo da pessoa), telefone, cidade país e um ponteiro para a próxima célula de mesmo hash).

## Como funciona?
Vamos supor que eu queira inserir um elemento (uma célula) na tabela, cuja chave é "Kelvin", essa chave irá produzir uma chave com base no tamanho da tabela e me retornará a posição da célula "Kelvin" na tabela, e assim, insiro os demais dados da respectiva célula.

Porém, eu quero adicionar outro valor cuja chave é "Melvin", isso pode gerar uma colisão se a tabela for muito pequeno ou a função de gerar hash não conter um bom retorno. Caso gere um hash que retorne a mesma posição de "Kelvin", eu não posso apagar o registro de "Kelvin", e sim, faço com que a célula que contém as informações de "Kelvin" apontem para a célula que contém as informações de "Melvin", ou seja, ambas as células estão na mesma posição da tabela, porém o que difere eles é a chave "Kelvin" e "Melvin", e caso eu queira as informações de "Melvin", terei que primeiro acessar a célula de "Kelvin", pois é ele que contém o ponteiro para a célula de busca.

Então dado um arquivo de entrada, fazemos operações na Hash Table.

Tais como: <b>Carregar arquivo, Localizar Contato, inserir novo contato, excluir contato, atualizar contato, salvar dados e sair do programa.</b>

## Vamos ver como fica no terminal
Para compilar:<br>
<i><b>gcc -Wall -pedantic hashing.c buffer.c buffer.h arquivo.c arquivo.h -o exec</b></i>

Para executar:<br>
<i><b>./exec</b></i>

## Menu

![menuHT](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/imagens_readme/menuHT.png)

## Opção 1: Carregando um arquivo

![load_file](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/imagens_readme/carregando_arquivoHT.png)

Dependendo da opção selecionada, eu escolhi limpar a tela do terminal para uma melhor visualização.

E se eu tentar buscar algum contato sem ter um arquivo carregado?
![impedindo](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/imagens_readme/impedindo_carregar_arquivo.png)

Simplesmente impeço tal ação. Tais impedimentos também ocorrem ao tentar realizar as demais ações sem ter um arquivo carregado.

## Opção 2: Localizar Contato
Com um contato existente na tabela (lembrando que a tabela ja esta carregada com o arquivo informado).

![contato_existe](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/imagens_readme/contato_existe.png)


Com um contato que não existe na tabela.
![contato_nao_existe](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/imagens_readme/contato_nao_existe.png)

## Opção 3: Inserir novo contato
Vamos então inserir o contato não encontrado.

![inserindo](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/imagens_readme/buscando_contato.png)

E em seguida, buscar esse mesmo contato, que antes deu como não encontrado.

![contato_existe](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/imagens_readme/buscando_contato.png)

E está ai as informações que inserimos anteriormente.

## Opção 4: Excluir Contato

Vamos tentar excluir o contato do Thomas e em seguida, tentar buscar ele na tabela.

![apagando_e_verificando](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/imagens_readme/apagando_contato.png)

Ou seja, apagamos o contato do Thomas e em seguida o buscamos novamente, nos informando que não foi encontrado e constatando que a exclusão foi feita corretamente.

## Opção 5: Atualizar Contato

Vamos atualizar o país do Kelvin, por exemplo.

Primeiro selecionamos a opção e digitamos o nome do contato. Em seguida, é aberto um submenu (dentro da categoria de atualização), nos perguntando o que queremos atualizar, vamos escolher a opção 3, referente à atualização do país.

![atualizando_pais](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/imagens_readme/atualizar_contato.png)

E ao buscar o contato novamente, vemos o seu país atualizado.

![contato_atualizado](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/imagens_readme/contato_atualizado.png)

## Opção 6: Visualizando HashTable

O arquivo de entrada contém 10 mil registros, e o tamanho da tabela, eu defini sendo como: A quantidade de registros do arquivo carregado * 1.5, ou seja, um arquivo com 10 mil registros cria uma tabela com 15 mil posições. 

Obviamente não é possivel mostrar todo o conteúdo da tabela no terminal, sendo assi, mostro o inicio e o fim da mesma.

Mostro o tamanho da tabela (15 mil).
O slot é referente a posição do contato na tabela (ou seja, a Danyelle ocupa o slot 4). 

E em baixo do slot é informado o nome completo, telefone, cidade e país, separados por <b>|</b>.


![inicio_ht](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/imagens_readme/inicio_ht.png)

![fim_ht](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/imagens_readme/fim_ht.png)

## Opção 7: Salvar Dados

Vamos inserir mais dois contatos em nossa tabela e em seguida, salvar a tabela no nosso arquivo.

Inserindo novo contato

![insere_clara](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/imagens_readme/inserindo_contato_1.png)

Inserindo novo contato²

![insere_jon_snow](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/imagens_readme/inserindo_contato_2.png)

Salvando tabela no arquivo

![salvando_tabela](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/imagens_readme/salvando_tabela_arquivo.png)

E ao buscarmos o contato <i>Clara dos Santos</i> e <i>Jon Snow</i> em nosso arquivo, é visto que eles foram salvos corretamente.

![ver_contato1](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/imagens_readme/visualizando_contato_1.png)

![ver_contato2](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/imagens_readme/visualizando_contato_2.png)

## Opção 8: Sair do programa
Simplesmente, bye bye.

![bye_bye](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/imagens_readme/bye_bye.png)




