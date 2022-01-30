## COMANDO VALGRIND
Compilar:  gcc -Wall -pedantic -g -Og -std=gnu99 std_valgrind.c -o exec
Executar: valgrind --leak-check=full ./exec

///gcc -Wall -pedantic -g -Og -std=gnu99 buffer.c buffer.h utils_vector.c utils_vector.h mergesort_externo.c arquivo.c arquivo.h -o exec

//COMPLETO?
valgrind --leak-check=full --track-origins=yes --log-file=valgrind_output.txt ./exec Arquivos_Entrada/entrada1.csv Arquivos_Saida/saida_me.txt 10500