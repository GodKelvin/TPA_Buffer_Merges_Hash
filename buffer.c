#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"
/*
Armazena o conteudo de um arquivo em um buffer
Necessita dar um free no ponteiro que recebe o retorno
Verificar se o ultimo character eh um '\n', por fins de consistencia dos dados
Caso nao for, ler menos character (nao ler mais devido a estouro de memoria)
*/
//Melhorar msgs de erro
void* loadBuffer(Buffer* buffer)
{
    
    if(buffer->arquivo != NULL)
    {
        //Deslocando para a posicao de leitura do arquivo indicada pelo buffer
        //E se nao houver erro ao fazer isso,
        if(fseek(buffer->arquivo, buffer->posicao, SEEK_SET) == 0)
        {
            free(buffer->conteudo);

            buffer->conteudo = NULL;
            //Aloca memoria suficiente para a leitura do bloco (+ um \0 por seguranca)
            buffer->conteudo = (char*)malloc(buffer->tamanho * (sizeof(char)) + (sizeof(char)));
            //Se conseguiu alocar memoria corretamente
            if(buffer->conteudo != NULL)
            {
                
                //Copia todo o conteudo do bloco de tamanho N do arquivo para o buffer
                size_t new_tam =fread(buffer->conteudo, sizeof(char), buffer->tamanho, buffer->arquivo);
                //Se nao houver erro ao ler o bloco para o buffer
                if(ferror(buffer->arquivo) == 0)
                {
                    //@TODO: (ADEQUAR PARA CASO O FIM DO ARQUIVO NAO FOR UM '\n')
                    //Verificando se o ultimo char eh um '\n'
                    if((strcmp(&buffer->conteudo[new_tam-1], "\n") != 0))
                    {
                        //APONTAR PARA NULL???
                        //buffer->conteudo = NULL;
                        //Diminui em um o tamanho do buffer, ate encontrar um '\n'
                        buffer->tamanho--;
                        loadBuffer(buffer);
                    }
                    //Tudo certo na leitura do bloco no buffer. Consistencia dos dados -> OK
                    else
                    {
                        //Por questoes de seguranca
                        //Espaco alocado anteriormente para o '\0'
                        buffer->conteudo[new_tam++] = '\0';
                        buffer->posicao += buffer->tamanho;
                        buffer->tamanho = buffer->tamanho_original;
                    }
                }
                else
                {
                    printBuffer(buffer);
                    buffer->tamanho = buffer->tamanho_original;
                    printf("LoadBuffer ERROR 4\n");
                }
            }
            else
            {
                printf("LoadBuffer ERROR 3 -> CONTEUDO NULO!\n");
                printBuffer(buffer);
                printf("AQUI %s\n", buffer->conteudo);
            }
        }
        else
        {
            printf("LoadBuffer ERROR 2\n");
        }
    }
    else
    {
        printf("LoadBuffer ERROR 1\n");
    }

    return 0;
}

//Necessario para definir quando meu buffer chegou ao fim do arquivo
unsigned long int calcula_tamanho_arquivo(char *nome_arquivo)
{
    FILE *arquivo;
    arquivo = fopen(nome_arquivo, "r");
    unsigned long int numbytes = 0;
    if(fseek(arquivo, 0, SEEK_END) == 0)
    {
        numbytes = ftell(arquivo);
        //Se numbytes >= 0. numbytes obtidos com sucesso
        if(numbytes != -1)
        {
            //se houver um erro ao voltar para o incio do arquivo
            if(!fseek(arquivo, 0, SEEK_SET) == 0)
            {
                numbytes = 0;
            }
        }
    }
    fclose(arquivo);
    return numbytes;
}

Buffer* criaBuffer(char *nome_arquivo, unsigned long int tamanho_buffer)
{
    //Alocando memoria pro buffer em si
    Buffer* buffer = (Buffer*) malloc(sizeof(Buffer));

    //Salvando o nome do arquivo
    strcpy(buffer->nome_arquivo, nome_arquivo);
    
    //Calculando o tamanho do arquivo
    buffer->fim_arquivo = calcula_tamanho_arquivo(buffer->nome_arquivo);
    //Abrindo o arquivo
    buffer->arquivo = fopen(buffer->nome_arquivo, "r");
    if(buffer->arquivo == NULL)
    {
        printf("ERROR: Falha ao abrir arquivo: %s\n", buffer->nome_arquivo);
        return NULL;
    }
    
    //Posicao inicial do bloco do buffer
    buffer->posicao = 0;

    //Tamanho atual do buffer (pode ser modificado no ato de leitura do bloco, com o objetivo de consistencia dos dados)
    buffer->tamanho = tamanho_buffer;

    //Tamanho original do buffer, utilizado caso o tamanho atual seja modificado
    buffer->tamanho_original = tamanho_buffer;

    buffer->conteudo = NULL;

    //Para matriz
    buffer->pos_atual_matriz = 0;
    buffer->pos_max_matriz = 0;
    return buffer;
}

void freeBuffer(Buffer* buffer)
{
    free(buffer->conteudo);
    fclose(buffer->arquivo);
    free(buffer);
}

void freeBufferLista(Buffer* buffer)
{
    free(buffer->conteudo);
    fclose(buffer->arquivo);
}

void merging_buffers_to_file(char nome_arquivo_destino[], Buffer *buffer1, Buffer *buffer2)
{
    /*
    Abrindo o arquivo de destino no modo "append", ou seja, os dados nao serao sobrescritos
    e sim adicionados ao final
    */
    FILE *file_dest = NULL;
    file_dest = fopen(nome_arquivo_destino, "a");

    //ponteiro char para verificar se todo o buffer ja foi percorrido
    char *has_item_b1 = NULL;
    char *has_item_b2 = NULL;

    //Ponteiros para salvar a instancia de cada buffer no strtok_r
    char *saveptr_1 = NULL;
    char *saveptr_2 = NULL;
   
    /*
    Capturando o primeiro item de cada buffer.
    buffer a ser lido / separador / ponteiro para salvar onde parou a verificacao
    */
    has_item_b1 = strtok_r(buffer1->conteudo, "\n", &saveptr_1);
    has_item_b2 = strtok_r(buffer2->conteudo, "\n", &saveptr_2);


    //Enquanto houver itens em ambos os buffers
    while(has_item_b1 && has_item_b2)
    {
        if(atoi(has_item_b1) == atoi(has_item_b2)) 
        {
            //Escreve no arquivo
            fprintf(file_dest, "%s\n", has_item_b1);
            //Pego o proximo item dos buffers
            has_item_b1 = strtok_r(NULL ,"\n", &saveptr_1);
            has_item_b2 = strtok_r(NULL ,"\n", &saveptr_2);
        }
        else if(atoi(has_item_b1) < atoi(has_item_b2))
        {
            fprintf(file_dest, "%s\n", has_item_b1);
            has_item_b1 = strtok_r(NULL ,"\n", &saveptr_1);
        }
        else
        {
            fprintf(file_dest, "%s\n", has_item_b2);
            has_item_b2 = strtok_r(NULL ,"\n", &saveptr_2);
        }
    }

    /*
    Se ainda houver elementos nao percorridos nos buffer,
    escreve no novo arquivo os elementos restantes.
    */
    while(has_item_b1)
    {
        fprintf(file_dest, "%s\n", has_item_b1);
        has_item_b1 = strtok_r(NULL ,"\n", &saveptr_1);
    }
    while(has_item_b2)
    {
        fprintf(file_dest, "%s\n", has_item_b2);
        has_item_b2 = strtok_r(NULL ,"\n", &saveptr_2);
    }
    //Fechando arquiov de destino
    fclose(file_dest);
}

//Escreve item por item do buffer no arquivo de destino
void write_buffer_on_file(char *nome_arquivo_destino, Buffer* buffer)
{
    FILE *file_dest = NULL;
    file_dest = fopen(nome_arquivo_destino, "a");

    //ponteiro char para verificar se todo o buffer ja foi percorrido
    char *has_item = NULL;

    //Ponteiros para salvar a instancia de cada buffer no strtok_r
    char *saveptr= NULL;

    has_item = strtok_r(buffer->conteudo, "\n", &saveptr);
    while(has_item)
    {
        fprintf(file_dest, "%s\n", has_item);
        has_item = strtok_r(NULL ,"\n", &saveptr);
    }

    fclose(file_dest);

}

void merging_files(char *nome_arquivo_final, Buffer* buffer1, Buffer* buffer2)
{
    while(buffer1->posicao < buffer1->fim_arquivo && buffer2->posicao < buffer2->fim_arquivo)
    {
        loadBuffer(buffer1);
        loadBuffer(buffer2);
        merging_buffers_to_file(nome_arquivo_final, buffer1, buffer2);
    }

    //Se o arquivo 1 ainda nao chegou ao fim
    while(buffer1->posicao < buffer1->fim_arquivo)
    {
        loadBuffer(buffer1);
        write_buffer_on_file(nome_arquivo_final, buffer1);
    }

    //Se o arquivo 2 ainda nao chegou ao fim
    while(buffer2->posicao < buffer2->fim_arquivo)
    {
        loadBuffer(buffer2);
        write_buffer_on_file(nome_arquivo_final, buffer2);
    }
}

/*
Funcao apenas para fins de debugger.
Mostra o conteudo de um buffer.
*/
void printBuffer(Buffer* buffer)
{
    printf("------\n");
    printf("ARQUIVO: %s\n", buffer->nome_arquivo);
    printf("POSICAO: %ld\n", buffer->posicao);
    printf("FIM_ARQUIVO: %ld\n", buffer->fim_arquivo);
    printf("TAMANHO: %ld\n", buffer->tamanho);
    printf("TAMANHO_ORIGINAL: %ld\n", buffer->tamanho_original);
    printf("CONTEUDO:%s\n", buffer->conteudo);
    printf("POSICAO ATUAL MATRIZ: %ld\n", buffer->pos_atual_matriz);
    printf("POSICAO MAX MATRIZ: %ld\n", buffer->pos_max_matriz);
    printf("\n------\n");
}

/*
Nome arquivo nao eh necessario limpar, visto
que ao atribuir um novo novo, eh usado o strcpy
*/
void limpaBuffer(Buffer* buffer)
{
    fclose(buffer->arquivo);
    buffer->arquivo = NULL;
    free(buffer->conteudo);
    buffer->posicao = 0;
    buffer->fim_arquivo = 0;
    buffer->tamanho = 0;
    buffer->tamanho_original = 0;
}
///////////
//DEIXAR AQUI MESMO???
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

    strcpy(destino, item);
    free(copy_conteudo);
}

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

void buffer_to_matriz(Buffer* buffer, char **matriz, unsigned long int tam_matriz)
{
    for(unsigned long int i = 0; i < tam_matriz; i++)
    {
        get_word(matriz[i], buffer->conteudo, "\n", i);
    }
}

void print_matriz(char **matriz, unsigned long int tam_matriz)
{
    printf("-----\n");
    for(unsigned long int i = 0; i < tam_matriz; i++) printf("%s\n", matriz[i]);
    printf("-----\n");
}

//Salva uma matriz de strings num arquivo
//Recebe o nome do arquivos de destino, a matriz de strings e o tamanha da mesma
void matriz_to_file(char nome_arquivo[], char **matriz, unsigned long int tam_matriz)
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