#include <stdio.h>
#include <stdlib.h>
//Criados por mim
#include "buffer.h"

void half_file(char *nome_arquivo)
{
    unsigned long int tamanho_arquivo = calcula_tamanho_arquivo(nome_arquivo);
    unsigned long int half = tamanho_arquivo / 2;

    Buffer *buffer_original_file = criaBuffer(nome_arquivo, half);

    //Nome dos arquivos de Saida
    char half_arquivo_1[] = "Arquivos_Saida/half_file_1.csv";
    char half_arquivo_2[] = "Arquivos_Saida/half_file_2.csv";


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
    freeBuffer(buffer_original_file);
}

//void mergeSortExterno()
int main()
{
    char nome_arquivo_entrada[] = "Arquivos_Entrada/entrada1.csv" ;
    //char nome_arquivo_entrada[] = "Arquivos_Entrada/AgendaTeste1M.csv" ;
    half_file(nome_arquivo_entrada);

    return 0;
}