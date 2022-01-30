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
Os códigos estão muito bem comentados, e com excessão dos arquivos [k_way_merge.c](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/k_way_merge.c), [mergesort_externo.c](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/mergesort_externo.c) e [hashing.c](https://github.com/GodKelvin/TPA_Buffer_Order_Merges/blob/master/hashing.c), o restante está bem dividido em .C e .H.

O maior empecilho deste trabalho é decorrente da sua maior força, a própria linguagem de programação (ou talvez seja o inexperiente programador que quis se aventurar nisso tudo? <b>Talvez</b>). 

<b>C</b>, uma linguagem de programação que faz EXATAMENTE o que você pedir pra ela fazer, cria variáveis a hora que você pedir, se tiver memória disponível, ela te dá, mas adivinha? Ela não pede de volta... Quer devolver a memória para o sistema? Faça você mesmo! 

<b>C</b> é uma linguagem amiga, pois te avisa quando estás prestes a fazer m&rd@, evitando assim que o PC exploda (okay, explodir talvez seja exagero, mas uma tela azul é bem comum). Veja bem, ela TENTA impedir você de fazer besteira, mas não quer dizer que ela vai te impedir. Pois bem, quer utilizar memória do sistema? Use! Quer devolver essa memória? Libere-a! Que acessar regiões de memória que não são suas? <b>Tente</b>.

## Seguindo,
Em resumo, precisava evitar de carregar um arquivo gigantesco INTEIRO na memória RAM, para isso o gerenciador de buffer foi desenvolvido. Dado o nome de um arquivo e o tamanho do buffer em BYTES, tudo estava pronto para ser utilizado!

A função principal do gerenciador (e por consequência a que me deu mais dor de cabeça (e por coincidência, a qual mais me orgulho)) é a <b> loadBuffer() </b>. Tenho total ciência que ela pode ser melhorada em inúmeros aspctos, e assim será feito posteriormente, pois percebi uma escassez na comunidade a respeito de algo desse tipo.

Para implementa-la, precisei entender como funciona o SEEK_SET em arquivos, a manipulação da função fseek, conceitos um pouco mais "avançados" de recursividade (amiga ou inimiga? Depende) e meu deus, TER CUIDADO COM A RAM, 

Comecei a estudar o VALGRIND

