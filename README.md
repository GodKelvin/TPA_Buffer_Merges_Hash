COMPILAR ATUAL EM ANDAMENTO
gcc -Wall -pedantic buffer.c buffer.h utils_vector.c utils_vector.h mergesort_externo.c arquivo.c arquivo.h -o exec
EXECUTAR
./exec Arquivos_Entrada/entrada1.csv Arquivos_Saida/saida_me.txt 10500


-> Calcular tamanho do arquivo para averiguar o espaço dele em RAM!
//
Criar matriz dentro do buffer,
criar funcao de matriz to buffer (file?)


//
merge sort nos arquivos _2
trabalhando atualmente no hashing : gcc -Wall -pedantic std_hashing_2.c buffer.c buffer.h arquivo.c arquivo.h -o exec_2

k-way-merge:  gcc -Wall -pedantic k_way_merge.c -o exec


## Merge externo
### Importante encontrar um equilíbrio entre a quantidade de RAM disponível para a alocação e a quantidade de arquivos criados. Isso impacta diretamente no tempo de execução e merge dos arquivos. Para essa tipo de conjunto de dados, interessante deixar o espaço de RAM para cada arquivo na faixa de 300~400 bytes. Ou seja, capacidade maxima da RAM / quantide de arquivos = 300~400


Referencia:
set_value_ht(hashtable, "Nome1", "KELVIN LEHRBACK");



//PASSAR FUNCOES DO BUFFER DO MERGE SORT EXTERNO PARA O ARQUIVO DE BUFFER

//QUANTO MENOR A QUANTIDADE DE ARQUIVOS, MAIS LENTO FICA


Arquivo 500k:
-Tamanho da ram: 105.000

Arquivo 1M: 
-Tamanho da ram: 195.400




## COMANDO VALGRIND
Compilar:  gcc -Wall -pedantic -g -Og -std=gnu99 std_valgrind.c -o exec
Executar: valgrind --leak-check=full ./exec

///gcc -Wall -pedantic -g -Og -std=gnu99 buffer.c buffer.h utils_vector.c utils_vector.h mergesort_externo.c arquivo.c arquivo.h -o exec

//COMPLETO?
valgrind --leak-check=full --track-origins=yes --log-file=valgrind_output.txt ./exec Arquivos_Entrada/entrada1.csv Arquivos_Saida/saida_me.txt 10500

