#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>//MKDIR
//Criados por mim
#include "buffer.h"
#include "arquivo.h"

//MOVER PARA O ARQUIVO DE UTILS VECTOR?
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

//Dada a quantidade de linhas, armazena numa matriz
//Receber tambem a posicao da matriz para dar continuidade a insercao
void file_to_matriz(char *nome_arquivo, char **matriz, unsigned long int qtd_linhas, unsigned long int pos_matriz)
{
    int tam = 0;
    FILE *arquivo = fopen(nome_arquivo, "r");
    if(!arquivo)
    {
        printf("Nao foi possivel abrir o arquivo: %s\n", nome_arquivo);
        return;
    }
    else
    {
        while(tam < qtd_linhas)
        {
            //int leu = fscanf(arquivo, "%[^\n] ", &(*matriz)[pos_matriz]);
            int leu = fscanf(arquivo, "%[^\n] ", matriz[pos_matriz]);
            //int leu = fscanf(arquivo, "%[^\n] ", line);
            leu++;
            //printf("%s\n", line);
            //printf("%s\n", matriz[pos_matriz]);
            pos_matriz++;
            tam++;
        }
    }
    fclose(arquivo);
}

//Recebe o tamanho das rodadas, arquivos de origem e de destino
void run_merging(int number_rodadas, char *origem_1, char *origem_2, char *dest_1, char *dest2)
{
    unsigned long int pos_matriz = 0;
    /*EX: Rodada 1 == 2 linhas de cada arquivo.
    2 linhas de cada arquivo == 4 posicoes na matriz*/
    unsigned long int qtd_linhas = number_rodadas * 2;
    unsigned long int tamanho_matriz = number_rodadas * 4;

    char **matriz = (char**)malloc(tamanho_matriz * sizeof(char*));
    for(unsigned long int i = 0; i < tamanho_matriz; i++)
    {
        matriz[i] = (char*)malloc(200 * sizeof(char));
    }
    
    file_to_matriz(origem_1, matriz, qtd_linhas, pos_matriz);
    print_matriz(matriz, qtd_linhas);
    free_matriz(matriz, tamanho_matriz);
    


}

//void mergeSortExterno()
int main()
{
    char nome_arquivo_entrada[] = "Arquivos_Entrada/entrada1.csv" ;

    //Nome dos arquivos de Saida
    char aux_arq_1[] = "Arquivos_Saida/aux_arq_1.csv";
    char aux_arq_2[] = "Arquivos_Saida/aux_arq_2.csv";

    char aux_arq_3[] = "Arquivos_Saida/aux_arq_3.csv";
    char aux_arq_4[] = "Arquivos_Saida/aux_arq_4.csv";

    mkdir("Arquivos_Saida", 0700);
    cria_reset_file(aux_arq_1);
    cria_reset_file(aux_arq_2);
    cria_reset_file(aux_arq_3);
    cria_reset_file(aux_arq_4);
    
    
    //Divido o arquivo original ao meio
    half_file(nome_arquivo_entrada, aux_arq_1, aux_arq_2);
    run_merging(1, aux_arq_1, aux_arq_2, aux_arq_3, aux_arq_4);


    return 0;
}