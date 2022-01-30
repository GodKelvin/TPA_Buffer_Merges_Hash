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

Ou seja, foram criados vários arquivos temporários (e já ordenados) com base no arquivo de entrada e o tamanho do buffer para cada um, e depois disso, é realizado K way merge.

## Merge Sort Externo

