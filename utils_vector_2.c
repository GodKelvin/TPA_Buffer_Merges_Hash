#include <stdio.h>
#include <stdlib.h>

//Calcula o endereco do pivo para a execucao do quick_sort
/*Insere os elementos menores a esquerda e maiores a direita,
dado inicio e fim do "range" do vetor*/
int get_pos_pivo(int vetor[], int inicio, int fim)
{
    int pivo, index, desloca, aux;
    pivo = vetor[fim];
    index = inicio;
    desloca = inicio;
    
    for(desloca = inicio; desloca < fim; desloca++)
    {
        if(vetor[desloca] <= pivo)
        {
            aux = vetor[desloca];
            vetor[desloca] = vetor[index];
            vetor[index] = aux;
            index++;
        }
    }

    //inserir o pivo na posicao correta
    aux = vetor[index];
    vetor[index] = vetor[fim];
    vetor[fim] = aux;

    //Retorna a posicao do pivo para o quick_sort
    return index;
}

//Algoritmo feito com base na videoaula: https://youtu.be/wx5juM9bbFo
//Na aula eh mostrado em python, apenas entendi e converti a logica para C.
void quick_sort(int vetor[], int inicio, int fim)
{
    int posPivo;
    //Condicao de parada do algoritmo
    if(inicio < fim)
    {
        //Calcular a posicao do pivo
        posPivo = get_pos_pivo(vetor, inicio, fim);
        //Sublista a esquerda
        quick_sort(vetor, inicio, posPivo-1);

        //Sublista a direita
        quick_sort(vetor, posPivo+1, fim);
    }
}

//Printa os valores de um vetor dado o endereco e o tamanho do mesmo
void print_vetor(int vetor[], int tam)
{
    for(int i = 0; i < tam; i++)
    {
        printf("%d, ", vetor[i]);
    }
    printf("\n");
}

//Realiza a busca de um elemento do vetor
/*
Foi utilizado o metodo de busca binaria por questao de desempenho,
visto que ele eh mais eficiente que o de busca sequencial, tendo complexidade O(log2.n),
e porque os vetores estao ordenados.
*/
int busca_binaria(int alvo, int vetor[], int tam)
{
    int lim_inf = 0, lim_sup = tam-1, meio;
    while(lim_inf <= lim_sup)
    {
        meio = (lim_inf + lim_sup) / 2;
        if(alvo == vetor[meio])
        {
            return 1;
        }
        else if(alvo < vetor[meio])
        {
            lim_sup = meio - 1;
        }
        else
        {
            lim_inf = meio + 1;
        }
    }
    return 0;
}

int busca_binaria_string(char alvo[], char matriz[][100], int tam)
{
    int lim_inf = 0, lim_sup = tam-1, meio;
    while(lim_inf <= lim_sup)
    {
        meio = (lim_inf + lim_sup) / 2;
        if(strcmp(alvo, matriz[meio]) == 0)
        {
            return 1;
        }
        //Alvo menor
        else if(strcmp(alvo, matriz[meio]) < 0)
        {
            lim_sup = meio - 1;
        }
        //Alvo Maior
        else
        {
            lim_inf = meio + 1;
        }
        /*
        if(alvo == vetor[meio])
        {
            return 1;
        }
        else if(alvo < vetor[meio])
        {
            lim_sup = meio - 1;
        }
        else
        {
            lim_inf = meio + 1;
        }
        */
    }
    return 0;
}

/*
-> Recebe o caminho do arquivo e o endereco do vetor.
-> Popula o vetor com os dados do arquivo.
-> Retorna o tamanho do respectivo vetor.
OBS: **vetor == endereco do vetor, necessario pois esta sendo allocado dentro de uma funcao
*/
int file_to_vetor(char *nome_arquivo, int **vetor)
{
    //printf("%s\n", nome_arquivo);
    int tam;
    FILE *arquivo = fopen(nome_arquivo, "r");
    if(!arquivo) 
    {
        printf("Nao foi possivel abrir o arquivo %s\n", nome_arquivo);
        return 0;
    }
    else
    {
        //Primeira linha do arquivo == tamanho do vetor
        fscanf(arquivo, "%d", &tam);
        *vetor = (int*) malloc(tam * sizeof(int));
        int i = 0;
        //As linhas posteriores do arquivos sao os valores do vetor
        while((fscanf(arquivo, "%d", &(*vetor)[i]) != EOF)) i++;
        fclose(arquivo);
    }
    return tam;
}