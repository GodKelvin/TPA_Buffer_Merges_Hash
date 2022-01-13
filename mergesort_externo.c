//Apenas um Arquivo para estudo do mergesort
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100
void criarArquivoTeste(char *nome)
{
    int i;
    FILE *f = fopen(nome, "w");
    srand(time(NULL));
    for(i = 0; i < 1000; i++)
    {
        fprintf(f, "%d\n", rand());
    }
    fprintf(f, "%d", rand());
    fclose(f);
}
int main()
{
    criarArquivoTeste("Arquivos_Entrada/funcao_rand.txt");
    return 0;
}