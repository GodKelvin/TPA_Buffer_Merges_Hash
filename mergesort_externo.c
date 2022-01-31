#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>//MKDIR
#include <time.h>
//Criados por mim
#include "buffer.h"
#include "arquivo.h"
#include "utils_vector.h"

//MOVER PARA O ARQUIVO DE UTILS VECTOR?

char **cria_matriz(unsigned long int tamanho_matriz)
{
    char **matriz = (char**)malloc(tamanho_matriz * sizeof(char*));
    for(int i = 0; i < tamanho_matriz; i++)
    {
        matriz[i] = (char*)malloc(200 * sizeof(char));
    }

    return matriz;
}

void free_matriz(char **matriz, unsigned long int tamanho_matriz)
{
    for(unsigned long int i = 0; i < tamanho_matriz; i++)
    {
        free(matriz[i]);
    }
    free(matriz);
}

void half_file(char *nome_arquivo, char* half_arquivo_1, char *half_arquivo_2)
{
    unsigned long int tamanho_arquivo = calcula_tamanho_arquivo(nome_arquivo);
    unsigned long int half = tamanho_arquivo / 2;

    Buffer *buffer_original_file = criaBuffer(nome_arquivo, half);

    //Carrega metade do arquivo original e salva no novo arquivo_1
    loadBuffer(buffer_original_file);
    write_buffer_on_file(half_arquivo_1, buffer_original_file);

    loadBuffer(buffer_original_file);
    write_buffer_on_file(half_arquivo_2, buffer_original_file);

    /*Talvez a divisao ao meio nao seja exata,
    por isso, adicionamos o(s) elementos restante(s) ao arquivo 2.
    cerca de um ou dois registros.*/
    if(buffer_original_file->posicao < buffer_original_file->fim_arquivo)
    {
        loadBuffer(buffer_original_file);
        write_buffer_on_file(half_arquivo_2, buffer_original_file);

    }

    //Libero o Buffer utilizado para dividir o arquivo ao meio
    freeBuffer(buffer_original_file);
}

unsigned long int new_file_to_matriz(FILE *arquivo, char **matriz, unsigned long int qtd_linhas)
{
    unsigned long int tam = 0;
    //Nao posso fechar o arquivo nessa funcao, pois perco o ponteiro de linha
    while(tam < qtd_linhas && (fscanf(arquivo, "%[^\n] ", matriz[tam]) != EOF))
    {
        //Avanco a posicao da matriz e informo que li uma linha
        tam++;
    }
    /*Informo quantas linhas eu li,
    pois o arquivo pode terminar antes da quantidade de linhas informado*/
    return tam;
}

void append_matriz_to_file(FILE *arquivo, char **matriz, unsigned long int tam_matriz)
{
    for(unsigned long int i = 0; i < tam_matriz; i++)
    {
        fprintf(arquivo, "%s\n", matriz[i]);
    }
}

void delete_empty_file(char *nome_arquivo)
{
    unsigned long int has_content = calcula_tamanho_arquivo(nome_arquivo);
    if(!has_content)
    {
        remove(nome_arquivo);;
    }
    else
    {
        rename(nome_arquivo, "Arquivos_Saida/saida_merge_sort_externo.csv");
    }
}

void new_merging(FILE *arquivo, char **matriz_1, char **matriz_2, unsigned long int tam_1, unsigned long int tam_2)
{
    unsigned long int pos_1 = 0;
    unsigned long int pos_2 = 0;

    while((pos_1 < tam_1) && (pos_2 < tam_2))
    {
        if(strcmp(matriz_1[pos_1], matriz_2[pos_2]) < 0)
        {
            fprintf(arquivo, "%s\n", matriz_1[pos_1]);
            pos_1 += 1;
        }
        else
        {
            fprintf(arquivo, "%s\n", matriz_2[pos_2]);
            pos_2 += 1;
        }            
    }

    while(pos_1 < tam_1)
    {
        fprintf(arquivo, "%s\n", matriz_1[pos_1]);   
        pos_1 += 1;
    }

    while(pos_2 < tam_2)
    {
        fprintf(arquivo, "%s\n", matriz_2[pos_2]);
        pos_2 += 1;
    }
}

//Quantidade de linhas dobra a cada RUN
void run_merging(unsigned long int qtd_linhas, char *origem_1, char *origem_2, char *dest_1, char *dest_2)
{
    FILE *arq_origem_1 = fopen(origem_1, "r");
    FILE *arq_origem_2 = fopen(origem_2, "r");
    FILE *arq_destino_1 = fopen(dest_1, "a");
    FILE *arq_destino_2 = fopen(dest_2, "a");

    if(arq_origem_1 == NULL) 
    {
        printf("ERROR ARQUIVO: %s\n", origem_1); return;
    }
    if(arq_origem_2 == NULL) 
    {
        printf("ERROR ARQUIVO: %s\n", origem_2); return;
    }
    if(arq_destino_1 == NULL) 
    {
        printf("ERROR ARQUIVO: %s\n", dest_1); return;
    }
    if(arq_destino_2 == NULL)
    {
        printf("ERROR ARQUIVO: %s\n", dest_2); return;
    }

    unsigned long int tamanho_matriz = qtd_linhas;

    char **matriz_1 = NULL;
    char **matriz_2 = NULL;
    unsigned long int linhas_lidas_1 = 0;
    unsigned long int linhas_lidas_2 = 0;
    //Comeca com 1 apenas para iniciar o loop
    unsigned long int total_linhas_lidas = 1;

    //Para verificar em qual arquivo escrever
    int flag_file = 1;
    int write_file_1 = 0;
    int write_file_2 = 0;

    int read_file_1 = 0;
    int read_file_2 = 0;
    printf("[...Nova Rodada...]\n");
    while(total_linhas_lidas)
    {
        matriz_1 = cria_matriz(tamanho_matriz);
        matriz_2 = cria_matriz(tamanho_matriz);

        //Reseto as variaveis utilizadas
        total_linhas_lidas = 0;
        linhas_lidas_1 = 0;
        linhas_lidas_2 = 0;

        //Leio da origem 1
        linhas_lidas_1 = new_file_to_matriz(arq_origem_1, matriz_1, qtd_linhas);
        if(linhas_lidas_1)
        { 
            read_file_1 = 1;
        }
        total_linhas_lidas += linhas_lidas_1;
        //pos_matriz = linhas_lidas;

        //Leio da origem 2
        linhas_lidas_2 = new_file_to_matriz(arq_origem_2, matriz_2, qtd_linhas);
        if(linhas_lidas_2)
        {   
            read_file_2 = 1;
        }
        //if(linhas_lidas) read_file_2 = 1;
        total_linhas_lidas += linhas_lidas_2;
        
        //Se eu li somente de um arquivo, ja esta tudo ordenado!
        if(!read_file_1 || !read_file_2)
        {
            free_matriz(matriz_1, tamanho_matriz);
            free_matriz(matriz_2, tamanho_matriz);
            break;
        }
        //Salvo no arquivo dependendo da flag
        if(flag_file)
        {
            new_merging(arq_destino_1, matriz_1, matriz_2, linhas_lidas_1, linhas_lidas_2);
            flag_file = 0;
            write_file_1 = 1;
        }
        else
        {
            new_merging(arq_destino_2, matriz_1, matriz_2, linhas_lidas_1, linhas_lidas_2);
            flag_file = 1;
            write_file_2 = 1;
        }
        free_matriz(matriz_1, tamanho_matriz);
        free_matriz(matriz_2, tamanho_matriz);
    }

    //Se eu escrevi nos dois arquivos, entao nao terminou
    if(write_file_1 && write_file_2 && read_file_1 && read_file_2)
    {   
        fclose(arq_origem_1);
        fclose(arq_origem_2);
        fclose(arq_destino_1);
        fclose(arq_destino_2);

        //Limpo os dois arquivos de destino;
        cria_reset_file(origem_1);
        cria_reset_file(origem_2);

        //Duplica a quantidade de linhas, pois ja se foi uma RUN
        qtd_linhas *= 2;

        //Agora os arquivos de origem sao os arquivos de destinos anteriores
        run_merging(qtd_linhas, dest_1, dest_2, origem_1, origem_2);
        
    }
    else
    {
        printf("\n[...Finalizando...]\n");
        fclose(arq_origem_1);
        fclose(arq_origem_2);
        fclose(arq_destino_1);
        fclose(arq_destino_2);

        /*Verifica quais os arquivos vazios e os apaga.
        Restando somente o com o conteudo ordenado.
        E renomeando-o*/
        delete_empty_file(dest_1);
        delete_empty_file(dest_2);
        delete_empty_file(origem_1);
        delete_empty_file(origem_2);
        
    }
}

//void mergeSortExterno()
int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("ERROR: Favor informar o arquivo de entrada e inicio da RUN!\n");
        printf("--> EXEMPLO: ./exec arquivo_entrada 1\n");
        return 0;
    }
    time_t inicio, fim, total;
    inicio = time(NULL);

    //Normalmente inicado em 1, mas pode ser variavel
    unsigned long int run = strtol(argv[2], NULL, 10);

    //Nome dos arquivos de Saida
    //Arquivos de origem divididos ao meio (inicialmente)
    //Vao ficar se revezando entre origem e destino.
    char aux_arq_1[] = "Arquivos_Saida/aux_arq_1.csv";
    char aux_arq_2[] = "Arquivos_Saida/aux_arq_2.csv";

    //Os dois arquivos de destinos que vão ficar se revezando entre origem e destno
    char aux_arq_3[] = "Arquivos_Saida/aux_arq_3.csv";
    char aux_arq_4[] = "Arquivos_Saida/aux_arq_4.csv";

    mkdir("Arquivos_Saida", 0700);
    cria_reset_file(aux_arq_1);
    cria_reset_file(aux_arq_2);
    cria_reset_file(aux_arq_3);
    cria_reset_file(aux_arq_4);
    
    
    //Divido o arquivo original ao meio
    printf("[...Dividindo arquivo ao meio...]\n\n");
    half_file(argv[1], aux_arq_1, aux_arq_2);
    
    /*Informo a RUN inicial, os dois arquivos de origem (arquivo inicial)
    dividido ao meio e os dois arquivos de destino.
    Durante a execucao, os 4 arquivos vao ficar se revezando entre
    origem e destino.*/
    //Quantidade de linhas lidas dobra a cada rodada.
    printf("[...Iniciando as Rodadas...]\n");
    run_merging(run, aux_arq_1, aux_arq_2, aux_arq_3, aux_arq_4);
    fim = time(NULL);
    total = fim - inicio;

    //Tempo em segundos
    printf("\n->TEMPO TOTAL DE EXECUCAO: %ld segundo(s).\n", total);
    return 0;
}