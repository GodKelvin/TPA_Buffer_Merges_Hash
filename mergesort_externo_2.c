//Apenas um Arquivo para estudo do mergesort
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils_vector.h"
#include "buffer.h"

//Tamanho MAXIMO em bytes na RAM
#define N 200

void get_word(char destino[], char frase[], char separador[], int posicao)
{
    char *copy_conteudo = strdup(frase);
    char *item = NULL, *saveptr = NULL;
    int i = 0;

    item = strtok_r(copy_conteudo, separador, &saveptr);
    //Captura a palavra da respectiva posicao
    while(i < posicao)
    {
        item = strtok_r(NULL, separador, &saveptr);
        i++;
    }
    //printf("ITEM: %s\n", item);
    //return item;
    strcpy(destino, item);
    free(copy_conteudo);
}

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

int criarArquivosOrdenados(char *nome_arquivo_entrada)
{
    //total == numero total de registros que cabem na RAM (ou seja, N)
    int V[N], qtd_arquivos = 0, total = 0;
    //Buffer principal
    Buffer* buffer = criaBuffer(nome_arquivo_entrada, N);
    //Novo nome do arquivo
    char nome_arquivos_temp[20];

    FILE *f = fopen(nome_arquivo_entrada, "r");
    //Enquanto nao estiver chegado ao fim do arquivo
    while(buffer->posicao < buffer->fim_arquivo)
    {

        qtd_arquivos++;
        //define nome do arquivo temporario
        sprintf(nome_arquivos_temp, "Temp%d.txt", qtd_arquivos);
        //Carrega os dados
        loadBuffer(buffer);
        //Ordena os dados
        quick_sort(V, 0, N-1);
        //...


    }
    while(!feof(f))
    {
        fscanf(f, "%d", &V[total]);
        total++;

        //Buffer cheio: Salva em disco
        if(total == N)
        {
            qtd_arquivos++;
            //Define o nome do novo arquivo
            sprintf(nome_arquivos_temp, "Temp%d.txt", qtd_arquivos);

            //Ordena o vetor (IMPLEMENTAR FUNCAO DE ORDENACAO)
            //qsort(V, total, sizeof(int), compara);
            quick_sort(V, 0, N-1);

            //Salvar o arquivo
            salvarArquivo(nome_arquivos_temp, V, total, 0);

            //Zerar o total para continuar o procedimento para o proximo arquivo
            total = 0;
        }
    }

    /*Sobraram dados no Buffer: Salvar em disco
    Ou seja, nao eh multiplo do tamanho do buffer*/
    if(total > 0)
    {
        qtd_arquivos++;
        sprintf(nome_arquivos_temp, "Temp%d.txt", qtd_arquivos);
        //qsort(V, total, sizeof(int), compara);
        salvarArquivo(nome_arquivos_temp, V, total, 0);
    }
    fclose(f);

    //Retorna a quantidade de arquivos
    return qtd_arquivos;
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
    
    //Cria o arquivo de saida e ja ordenado
    merge(nome_arq_saida, numArqs, k);

    //Apagar os arquivos temporarios
    for(i = 0; i< numArqs; i++)
    {
        //Formata o nome dos arquivos e os apaga
        sprintf(novo, "Temp%d.txt", i+1);
        //remove(novo);
    }
}
//Limpa o conteudo de um arquivo, tornando-o em branco
void cria_reset_file(char *nome_arquivo)
{
    FILE *file;
    file = fopen(nome_arquivo, "w");
    fclose(file);
}

//mover para o respectivo tipo de arquivo
int calcula_tam_buffer_to_matriz(Buffer* buffer, char separador[])
{
    int tam = 0;
    char *token = NULL;
    char *copy_frase = strdup(buffer->conteudo);

    token = strtok(copy_frase, separador);
    while(token != NULL)
    {
        tam++;
        token = strtok(NULL, separador);
    }

    free(copy_frase);
    return tam;
    
}

void print_matriz(char matriz[][100], int tam_matriz)
{
    printf("-----\n");
    for(int i = 0; i < tam_matriz; i++) printf("%s\n", matriz[i]);
    printf("-----\n");
}
//Mudar forma de associacao a matriz
void buffer_to_matriz(Buffer* buffer, char matriz[][100], int tam_matriz)
{
    //int tamanho_matriz = calcula_tam_buffer_to_matriz(buffer, "\n");

    for(int i = 0; i < tam_matriz; i++)
    {
        get_word(matriz[i], buffer->conteudo, "\n", i);
    }

    print_matriz(matriz, tam_matriz);
    printBuffer(buffer);
}

int main()
{
    
    char nome_arquivo_entrada[] = "Arquivos_Entrada/teste2.csv";
    Buffer* buffer = criaBuffer(nome_arquivo_entrada, 300);
    loadBuffer(buffer);
    //printBuffer(buffer);
    int tamanho_matriz = calcula_tam_buffer_to_matriz(buffer, "\n");
    //printf("Qtd Linhas: %d\n", tamanho_vetor);
    char matriz_buffer[tamanho_matriz][100];
    //char **matriz_buffer = NULL;
    buffer_to_matriz(buffer, matriz_buffer, tamanho_matriz);

    /*
    char nome_arquivo_saida[] = "Arquivos_Saida/saida_me.txt";
    cria_reset_file(nome_arquivo_saida);
    mergeSortExterno(nome_arquivo_entrada, nome_arquivo_saida);
    */

    freeBuffer(buffer);
    return 0;
}