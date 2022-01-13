//Apenas um Arquivo para estudo do mergesort
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils_vector.h"

#define N 5

//Cria arquivo com numeros aleatorios
void criarArquivoTeste(char *nome)
{
    int i;
    int div = 100;
    FILE *f = fopen(nome, "w");
    srand(time(NULL));
    for(i = 0; i < 10; i++)
    {
        fprintf(f, "%d\n", rand() % div);
    }
    fprintf(f, "%d", rand() % div);
    fclose(f);
}

void salvarArquivo(char *nome, int *vetor, int tam, int mudarLinhaFinal)
{
    int i;
    FILE *f = fopen(nome, "a");
    for(i = 0; i < tam-1; i++)
    {
        fprintf(f, "%d\n", vetor[i]);
    }

    if(mudarLinhaFinal)
    {
        fprintf(f, "%d\n", vetor[tam-1]);
    }
    else fprintf(f, "%d", vetor[tam-1]);

    fclose(f);
}

int criarArquivosOrdenados(char *nome)
{
    //total == numero total de registros que cabem na RAM (ou seja, N)
    int V[N], cont = 0, total = 0;
    //Novo nome do arquivo
    char novo[20];

    FILE *f = fopen(nome, "r");
    while(!feof(f))
    {
        fscanf(f, "%d", &V[total]);
        total++;

        //Buffer cheio: Salva em disco
        if(total == N)
        {
            cont++;
            //Define o nome do novo arquivo
            sprintf(novo, "Temp%d.txt", cont);

            //Ordena o vetor (IMPLEMENTAR FUNCAO DE ORDENACAO)
            //qsort(V, total, sizeof(int), compara);
            quick_sort(V, 0, N-1);

            //Salvar o arquivo
            salvarArquivo(novo, V, total, 0);

            //Zerar o total para continuar o procedimento para o proximo arquivo
            total = 0;
        }
    }

    /*Sobraram dados no Buffer: Salvar em disco
    Ou seja, nao eh multiplo do tamanho do buffer*/
    if(total > 0)
    {
        cont++;
        sprintf(novo, "Temp%d.txt", cont);
        //qsort(V, total, sizeof(int), compara);
        salvarArquivo(novo, V, total, 0);
    }
    fclose(f);

    //Retorna a quantidade de arquivos
    return cont;
}

//Para a funcao merge
struct arquivo
{
    FILE *f;
    int pos, MAX, *buffer;
};

//Arquivo / tamanho do buffer
void preencheBuffer(struct arquivo *arq, int K)
{
    int i;
    if(arq->f == NULL) return;

    arq->pos = 0;
    arq->MAX = 0;
    for(i = 0; i < K; i++)
    {
        if(!feof(arq->f))
        {
            fscanf(arq->f, "%d", &arq->buffer[arq->MAX]);
            arq->MAX++;
        }
        else
        {
            fclose(arq->f);
            arq->f = NULL;
            break;
        }
    }
}

int procuraMenor(struct arquivo *arq, int numArqs, int K, int *menor)
{
    //idx == controle se encontrou ou nao(e tbm a pos)
    int i, idx = -1;
    //Procura o menor valor na primeira posicao de cada buffer
    for(i = 0; i < numArqs; i++)
    {
        //Se n atingi o limite do arquivo
        if(arq[i].pos < arq[i].MAX)
        {
            //Se estou na primeira iteracao, salvo o primeiro registro (menor)
            if(idx == -1) 
            {
                idx = i;
            }
            //compara elemento do buffer atual do arquivo 2 com o elemento do buffer atual do arquivo 1
            else if(arq[i].buffer[arq[i].pos] < arq[idx].buffer[arq[idx].pos])
            {
                //Salvar o indice do arquivo 2
                idx = i;
            }
        }
    }
    //Achou o menor valor. Atualiza a posicao do buffer. Encher se tiver vazio.
    if(idx != -1)
    {
        *menor = arq[idx].buffer[arq[idx].pos];
        //incrementa o pos no arquivo no qual o menor valor foi encontrado
        arq[idx].pos++;

        //ja percorreu todos os elementos desse arquivo
        if(arq[idx].pos == arq[idx].MAX)
        {
            preencheBuffer(&arq[idx], K);
        }
        return 1;
    }
    //else
    return 0;
}

//Nome arquivo / Quantidade de arquivos / Tamanho do buffer
void merge(char *nome, int numArqs, int K)
{
    //Nome do novo arquivo temporario
    char novo[20];
    int i;
    int *buffer = (int*)malloc(K*sizeof(int));

    struct arquivo* arq;
    //Uma posicao para cada arquivo
    arq = (struct arquivo*)malloc(numArqs*sizeof(struct arquivo));

    for(i = 0; i < numArqs; i++)
    {
        sprintf(novo, "Temp%d.txt", i+1);
        arq[i].f = fopen(novo, "r");
        arq[i].MAX = 0;
        arq[i].pos = 0;
        
        //Quantidade de elementos que pode carregar na memoria para cada um dos buffers
        arq[i].buffer = (int*)malloc(K*sizeof(int));
        preencheBuffer(&arq[i], K);
    }

    int menor, qtdBuffer = 0;
    //Verificar se existe um menor elemento entre todos os buffers de cada arquivo
    while(procuraMenor(arq, numArqs, K, &menor) == 1)
    {
        //Salvar no buffer
        buffer[qtdBuffer] = menor;
        qtdBuffer++;
        //Buffer de saida cheio
        if(qtdBuffer == K)
        {
            salvarArquivo(nome, buffer, K, 1);
            qtdBuffer = 0;
        }
    }
    //Salva possiveis dados restantes no buffer
    if(qtdBuffer != 0) salvarArquivo(nome, buffer, qtdBuffer, 1);

    for(i = 0; i < numArqs; i++) free(arq[i].buffer);

    free(arq);
    free(buffer);
}

void mergeSortExterno(char *nome, char *nome_arq_saida)
{
    char novo[20];
    /*Quebrar os arquivos em partes menores e depois ordenar
    Retorna o numero de arquivos que foram criados*/
    int numArqs = criarArquivosOrdenados(nome);

    //N == Tamanho que a RAM comporta
    //k == numero de buffers que vao ser criados
    /*ou seja, k eh o tamanho que comporta pelo menos um pedacinho de cada arquivo para levar
    para a ram e fazer a intercalacao*/
    int i, k= N / (numArqs + 1);

    //Remove o arquivo original
    //remove(nome);

    //Cria o arquivo com o mesmo nome e ja ordenado (criar com nome diferente?)
    //merge(nome, numArqs, k);
    merge(nome_arq_saida, numArqs, k);

    //Apagar os arquivos temporarios
    for(i = 0; i< numArqs; i++)
    {
        //Formata o nome dos arquivos e os apaga
        sprintf(novo, "Temp%d.txt", i+1);
        remove(novo);
    }
}
//Limpa o conteudo de um arquivo, tornando-o em branco
void cria_reset_file(char *nome_arquivo)
{
    FILE *file;
    file = fopen(nome_arquivo, "w");
    fclose(file);
}

int main()
{
    char nome_arquivo_entrada[] = "Arquivos_Entrada/funcao_rand.txt";
    char nome_arquivo_saida[] = "Arquivos_Saida/saida_me.txt";

    cria_reset_file(nome_arquivo_entrada);
    cria_reset_file(nome_arquivo_saida);

    criarArquivoTeste(nome_arquivo_entrada);
    mergeSortExterno(nome_arquivo_entrada, nome_arquivo_saida);

    return 0;
}