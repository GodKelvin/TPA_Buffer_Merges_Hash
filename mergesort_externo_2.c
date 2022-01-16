//Apenas um Arquivo para estudo do mergesort
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>//MKDIR
//RENOMEAR ARQUIVOS!
#include "utils_vector_2.h"
#include "buffer_2.h"

//Tamanho MAXIMO em bytes na RAM
#define N 200000
//#define N 400

void get_word(char destino[], char frase[], char separador[], unsigned long int posicao)
{
    char *copy_conteudo = strdup(frase);
    char *item = NULL, *saveptr = NULL;
    unsigned long int i = 0;

    item = strtok_r(copy_conteudo, separador, &saveptr);
    //Captura a palavra da respectiva posicao
    while(i < posicao)
    {
        item = strtok_r(NULL, separador, &saveptr);
        i++;
    }
    //return item;
    strcpy(destino, item);
    free(copy_conteudo);
}

//mover para o respectivo tipo de arquivo
unsigned long int calcula_tam_buffer_to_matriz(Buffer* buffer, char separador[])
{
    unsigned long int tam = 0;
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

void print_matriz(char matriz[][255], unsigned long int tam_matriz)
{
    printf("-----\n");
    for(unsigned long int i = 0; i < tam_matriz; i++) printf("%s\n", matriz[i]);
    printf("-----\n");
}
//Mudar forma de associacao a matriz
void buffer_to_matriz(Buffer* buffer, char matriz[][255], unsigned long int tam_matriz)
{
    //int tamanho_matriz = calcula_tam_buffer_to_matriz(buffer, "\n");
    for(unsigned long int i = 0; i < tam_matriz; i++)
    {
        get_word(matriz[i], buffer->conteudo, "\n", i);
    }
}

//Salva uma matriz de strings num arquivo
//Recebe o nome do arquivos de destino, a matriz de strings e o tamanha da mesma
void matriz_to_file(char nome_arquivo[], char matriz[][255], unsigned long int tam_matriz)
{
    FILE *file_dest = NULL;
    file_dest = fopen(nome_arquivo, "w");
    if(file_dest)
    {
        for(unsigned long int i = 0; i < tam_matriz; i++)
        {
            fprintf(file_dest, "%s\n", matriz[i]);
        }
    }
    fclose(file_dest);
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

unsigned long int criarArquivosOrdenados(char *nome_arquivo_entrada)
{
    //total == numero total de registros que cabem na RAM (ou seja, N)
    //int V[N], total = 0;
    unsigned long int qtd_arquivos = 0;
    //Buffer principal
    Buffer* buffer = criaBuffer(nome_arquivo_entrada, N);
    //Novo nome do arquivo
    //char nome_arquivos_temp[200];
    char *nome_arquivos_temp = NULL;

    //FILE *f = fopen(nome_arquivo_entrada, "r");
    //Enquanto nao estiver chegado ao fim do arquivo
    printf("..Criando Arquivos menores e ordenados...\n");
    while(buffer->posicao < buffer->fim_arquivo)
    {
        nome_arquivos_temp = (char*) malloc(200 * sizeof(char));
        qtd_arquivos++;
        //printf("%ld\n", qtd_arquivos);
        //define nome do arquivo temporario
        sprintf(nome_arquivos_temp, "Arquivos_Saida/Temp%ld.txt", qtd_arquivos);
        //Carrega os dados
        loadBuffer(buffer);

        //Calcula a quantidade de linhas para o respectivo arquivo
        unsigned long int tamanho_matriz = calcula_tam_buffer_to_matriz(buffer, "\n");

        //Cria a matrix de string, com base no buffer lido
        char matriz_buffer[tamanho_matriz][255];
        /*
        char **matriz_buffer;
        matriz_buffer = malloc(tamanho_matriz * sizeof(char));
        for(int i = 0; i < tamanho_matriz; i++)
        {
            matriz_buffer[i] = malloc(255 * sizeof(char));
        }
        */

        buffer_to_matriz(buffer, matriz_buffer, tamanho_matriz);
        
        //Ordena os dados utilizando o quick_sort para strings
        quick_sort_string(matriz_buffer, 0, tamanho_matriz-1);

        //Salva os dados do respectivo buffer no arquivo de saida temporario
        matriz_to_file(nome_arquivos_temp, matriz_buffer, tamanho_matriz);

        free(nome_arquivos_temp);

        /*
        for(int i = 0; i < tamanho_matriz; i++)
        {
            free(matriz_buffer[i]);
        }
        free(matriz_buffer);
        */
    }
    freeBuffer(buffer);

    //fclose(f);

    //Retorna a quantidade de arquivos
    return qtd_arquivos;
}

//Para a funcao merge
struct arquivo
{
    FILE *f;
    int pos, MAX;
    char **buffer;
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
            //fscanf(arq->f, "%d", &arq->buffer[arq->MAX]);
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

//Lista de arquivos

int procuraMenor_2(Buffer **lista_buffers, int numArqs, char menor[])
{
    int pos = -1;
    for(int i = 0; i < numArqs; i++)
    {
        unsigned long int tamanho_matriz_i = calcula_tam_buffer_to_matriz(lista_buffers[i], "\n");
        char matriz_buffer_i[tamanho_matriz_i][255];
        buffer_to_matriz(lista_buffers[i], matriz_buffer_i, tamanho_matriz_i);
        lista_buffers[i]->pos_max_matriz = tamanho_matriz_i;

        //Se o buffer deste arquivo ainda nao foi todo percorrido
        if(lista_buffers[i]->pos_atual_matriz < lista_buffers[i]->pos_max_matriz)
        {

            //Se estou na primeira iteracao, salvo o primeiro registro (menor)
            if(pos == -1)
            {
                pos = i;
            }
            else
            {
                //Calcula a quantidade de linhas para os respectivo arquivos
                unsigned long int tamanho_matriz_pos = calcula_tam_buffer_to_matriz(lista_buffers[pos], "\n");

                //Cria a matrix de string, com base no buffer lido
                char matriz_buffer_pos[tamanho_matriz_pos][255];
                buffer_to_matriz(lista_buffers[pos], matriz_buffer_pos, tamanho_matriz_pos);

                //Compara os elementos das respectivas posicoes
                if(strcmp(matriz_buffer_i[lista_buffers[i]->pos_atual_matriz], matriz_buffer_pos[lista_buffers[pos]->pos_atual_matriz]) < 0)
                {
                    pos = i;
                }

            }
        }
    }

    if(pos != -1)
    {
        unsigned long int tamanho_matriz_menor = calcula_tam_buffer_to_matriz(lista_buffers[pos], "\n");
        char matriz_buffer_menor[tamanho_matriz_menor][255];
        buffer_to_matriz(lista_buffers[pos], matriz_buffer_menor, tamanho_matriz_menor);

        //Copia o menor valor encontrado (valor esse que pega na funcao de baixo dentro do loop)
        strcpy(menor, matriz_buffer_menor[lista_buffers[pos]->pos_atual_matriz]);

        lista_buffers[pos]->pos_atual_matriz++;
        //Se chegou ao tamanho maximo da matriz
        if(lista_buffers[pos]->pos_atual_matriz == lista_buffers[pos]->pos_max_matriz)
        {
            //Se ainda nao chegou ao final do arquivo
            if(lista_buffers[pos]->posicao < lista_buffers[pos]->fim_arquivo)
            {
                //Carrega o buffer para o proximo bloco de dados
                loadBuffer(lista_buffers[pos]);

                //Reseta as posicoes da matriz
                unsigned long int tamanho_matriz_i = calcula_tam_buffer_to_matriz(lista_buffers[pos], "\n");
                lista_buffers[pos]->pos_atual_matriz = 0;
                lista_buffers[pos]->pos_max_matriz = tamanho_matriz_i;
            }
        }
        return 1;
        
    }

    return 0;
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
                //Salvar o indice do arquivo com o menor elemento
                idx = i;
            }
        }
    }
    //Achou o menor valor. Atualiza a posicao do buffer. Encher se tiver vazio.
    if(idx != -1)
    {
        //*menor = arq[idx].buffer[arq[idx].pos];
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
void merge(char *nome_arq_saida, int numArqs, unsigned long int K)
{
    //Nome do novo arquivo temporario
    char *nome_arquivos_temp = NULL;;
    
    //Buffer utilizado para salvar no arquivo final
    Buffer* buffer = criaBuffer(nome_arq_saida, K);

    //struct arquivo* arq;
    //Uma posicao para cada arquivo
    Buffer* lista_buffers[numArqs];

    for(int i = 0; i < numArqs; i++)
    {
        nome_arquivos_temp = (char*) malloc(200 * sizeof(char));
        //Para abrir o arquivo
        sprintf(nome_arquivos_temp, "Arquivos_Saida/Temp%d.txt", i+1);
        lista_buffers[i] = criaBuffer(nome_arquivos_temp, K);
        loadBuffer(lista_buffers[i]);

        unsigned long int tamanho_matriz_i = calcula_tam_buffer_to_matriz(lista_buffers[i], "\n");
        lista_buffers[i]->pos_max_matriz = tamanho_matriz_i;
        free(nome_arquivos_temp);
    }
    char menor[255];
    int flag_salvou;
    //unsigned long int tamanho = 0;
    buffer->conteudo = (char*)malloc(buffer->tamanho * (sizeof(char) + 3));

    printf("...Mergeando arquivos...\n");
    while(procuraMenor_2(lista_buffers, numArqs, menor))
    {
        //printf("MENOR VALOR LOOP: %s\n", menor);
        unsigned long int prever_tamanho = buffer->posicao + strlen(menor);
        if(prever_tamanho < buffer->tamanho_original)
        {
            strcat(buffer->conteudo, menor);
            strcat(buffer->conteudo, "\n");
            buffer->posicao += strlen(menor);
            flag_salvou = 0;
            //printf("%s\n", buffer->conteudo);
        }
        else
        {
            //DEBUGGER
            printBuffer(buffer);
            //buffer cheio == Salvar no arquivo, limpar buffer, armazenar novo dado
            //Escrevo o buffer no arquivo de saida
            write_buffer_on_file(nome_arq_saida, buffer);

            free(buffer->conteudo);
            buffer->conteudo = (char*)malloc(buffer->tamanho * (sizeof(char) + 3));
            strcpy(buffer->conteudo, menor);
            strcat(buffer->conteudo, "\n");

            //Salvo o tamanho do inicio do buffer
            buffer->posicao = strlen(menor);
            flag_salvou = 1;
        }
        
    }

    //Verifica se tem dados restantes no buffer
    if(!flag_salvou)
    {
        write_buffer_on_file(nome_arq_saida, buffer);
    }   

    for(int i = 0; i < numArqs; i++) 
    {
        //freeBufferLista(&lista_buffers[i]);
        freeBuffer(lista_buffers[i]);
    }


    //free(arq);
    free(buffer);
}

void mergeSortExterno(char *nome_arquivo_entrada, char *nome_arq_saida)
{
    char arquivos_temp[100];
    /*Quebrar os arquivos em partes menores e depois ordenar
    Retorna o numero de arquivos que foram criados*/
    unsigned long int numArqs = criarArquivosOrdenados(nome_arquivo_entrada);
    printf("Quantidade de Arquivos: %ld\n", numArqs);

    //N == Tamanho que a RAM comporta
    //k == numero de buffers que vao ser criados
    /*ou seja, k eh o tamanho que comporta pelo menos um pedacinho de cada arquivo para levar
    para a ram e fazer a intercalacao*/

    unsigned long int k = N / (numArqs + 1);
    printf("Pedaco RAM pra cada arquivo == %ld\n", k);
    
    //Cria o arquivo de saida e ja ordenado
    merge(nome_arq_saida, numArqs, k);

    //Apagar os arquivos temporarios
    for(int i = 0; i< numArqs; i++)
    {
        //Formata o nome dos arquivos e os apaga
        sprintf(arquivos_temp, "Arquivos_Saida/Temp%d.txt", i+1);
        remove(arquivos_temp);
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
    mkdir("Arquivos_Saida", 0700);
    
    //char nome_arquivo_entrada[] = "Arquivos_Entrada/teste2.csv";
    char nome_arquivo_entrada[] = "Arquivos_Entrada/AgendaTeste1M.csv";

    //char nome_arquivo_saida_teste[] = "Arquivos_Saida/saida_quick_sort.txt";
    //cria_reset_file(nome_arquivo_saida_teste);

    /*
    Buffer* buffer = criaBuffer(nome_arquivo_entrada, 1000000);
    printBuffer(buffer);
    loadBuffer(buffer);
    printBuffer(buffer);
    unsigned long int tamanho_matriz = calcula_tam_buffer_to_matriz(buffer, "\n");
    char matriz_buffer[tamanho_matriz][255];
    buffer_to_matriz(buffer, matriz_buffer, tamanho_matriz);
    print_matriz(matriz_buffer, tamanho_matriz);
    quick_sort_string(matriz_buffer, 0, tamanho_matriz-1);
    matriz_to_file(nome_arquivo_saida_teste, matriz_buffer, tamanho_matriz);
    freeBuffer(buffer);
    */

    
    
    char nome_arquivo_saida[] = "Arquivos_Saida/saida_me.txt";
    cria_reset_file(nome_arquivo_saida);
    mergeSortExterno(nome_arquivo_entrada, nome_arquivo_saida);
    printf("TUDO CERTO!\n");

    return 0;
}