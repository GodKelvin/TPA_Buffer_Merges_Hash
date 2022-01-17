COMPILAR ATUAL EM ANDAMENTO
gcc buffer_2.c buffer_2.h utils_vector_2.c utils_vector_2.h mergesort_externo_2.c -o exec
gcc buffer.c buffer.h utils_vector.c utils_vector.h mergesort_externo.c -o exec

//
Criar matriz dentro do buffer,
criar funcao de matriz to buffer (file?)


//
merge sort nos arquivos _2
trabalhando atualmente no hashing : gcc -Wall -pedantic hashing.c -o exec



## Merge externo
### Importante encontrar um equilíbrio entre a quantidade de RAM disponível para a alocação e a quantidade de arquivos criados. Isso impacta diretamente no tempo de execução e merge dos arquivos. Para essa tipo de conjunto de dados, interessante deixar o espaço de RAM para cada arquivo na faixa de 300~400 bytes. Ou seja, capacidade maxima da RAM / quantide de arquivos = 300~400