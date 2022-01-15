
#include <stdio.h>
#include <string.h>

#ifndef BUFFER_H
#define BUFFER_H
typedef struct
{
    char nome_arquivo[50];
    FILE *arquivo;
    char *conteudo;
    //0 -> 4.294.967.295
    unsigned long int posicao;
    unsigned long int fim_arquivo;

    unsigned int tamanho;
    unsigned int tamanho_original;

}Buffer;

//int calcula_tam_buffer_to_vetor(Buffer* buffer, char separador[]);
void freeBuffer(Buffer* buffer);
void merging_buffers_to_file(char nome_arquivo_destino[], Buffer *buffer1, Buffer *buffer2);
void write_buffer_on_file(char *nome_arquivo_destino, Buffer* buffer);
void merging_files(char *nome_arquivo_final, Buffer* buffer1, Buffer* buffer2);
void printBuffer(Buffer* buffer);
void limpaBuffer(Buffer* buffer);
void* loadBuffer(Buffer* buffer);
unsigned long int calcula_tamanho_arquivo(char *nome_arquivo);
Buffer* criaBuffer(char *nome_arquivo, int tamanho_buffer);
//char *get_word(char frase[], char separador[], int posicao);

#endif
