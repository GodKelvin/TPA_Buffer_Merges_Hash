#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>//MKDIR

//Criados por mim
#include "utils_vector.h"
#include "buffer.h"
#include "arquivo.h"

//Tamanho MAXIMO em bytes na RAM


unsigned long int criarArquivosOrdenados(char *nome_arquivo_entrada, unsigned long int tam_ram)
{
    unsigned long int qtd_arquivos = 0;

    //Buffer principal
    Buffer* buffer = criaBuffer(nome_arquivo_entrada, tam_ram);
    char *nome_arquivos_temp = NULL;

    printf("[...Criando Arquivos menores e ordenados...]\n");
    //Enquanto nao estiver chegado ao fim do arquivo
    while(buffer->posicao < buffer->fim_arquivo)
    {
        printf("OPA1\n");
        nome_arquivos_temp = (char*) malloc(200 * sizeof(char));
        qtd_arquivos++;

        //Define nome do arquivo temporario
        sprintf(nome_arquivos_temp, "Arquivos_Saida/Temp%ld.txt", qtd_arquivos);
        //Carrega os dados
        printf("OPA1.2\n");
        loadBuffer(buffer);
        printf("OPA1.3\n");
        printf("OPA2\n");
        //Calcula a quantidade de linhas para o respectivo arquivo
        unsigned long int tamanho_matriz = calcula_tam_buffer_to_matriz(buffer, "\n");

        //Cria a matrix de string, com base no buffer lido (UTILIZAR MALLOC?)
        
        //char matriz_buffer[tamanho_matriz][255];
        char **matriz_buffer = (char**)malloc(tamanho_matriz * sizeof(char*));
        for(int i = 0; i < tamanho_matriz; i++)
        {
            matriz_buffer[i] = (char*)malloc(255 * sizeof(char));
            //printf("I == %d\n", i);
        }
        printf("OPA3\n");
        buffer_to_matriz(buffer, matriz_buffer, tamanho_matriz);
        //Ordena os dados utilizando o quick_sort para strings
        quick_sort_string(matriz_buffer, 0, tamanho_matriz-1);

        //Salva os dados do respectivo buffer no arquivo de saida temporario
        matriz_to_file(nome_arquivos_temp, matriz_buffer, tamanho_matriz);

        free(nome_arquivos_temp);
        printf("OPA4\n");
        /*
        for(int i = 0; i < tamanho_matriz; i++)
        {
            printf("OK\n");
            free(matriz_buffer[i]);
        }
        */
        //free(matriz_buffer);
    }
    printf("ACABOU\n");
    freeBuffer(buffer);


    //Retorna a quantidade de arquivos
    return qtd_arquivos;
}



//Procura o menor valor dado uma lista de arquivos
int procuraMenor(Buffer **lista_buffers, int numArqs, char menor[])
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
        //printf("%s\n", menor);

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

//Nome arquivo / Quantidade de arquivos / Tamanho do buffer
void merge(char *nome_arq_saida, int numArqs, unsigned long int pedaco_ram)
{
    //Nome do novo arquivo temporario
    char *nome_arquivos_temp = NULL;;
    
    //Buffer utilizado para salvar no arquivo final
    Buffer* buffer = criaBuffer(nome_arq_saida, pedaco_ram);
    
    //Uma posicao para cada arquivo
    Buffer* lista_buffers[numArqs];

    for(int i = 0; i < numArqs; i++)
    {
        nome_arquivos_temp = (char*) malloc(200 * sizeof(char));
        //Para abrir o arquivo
        sprintf(nome_arquivos_temp, "Arquivos_Saida/Temp%d.txt", i+1);
        lista_buffers[i] = criaBuffer(nome_arquivos_temp, pedaco_ram);
        loadBuffer(lista_buffers[i]);

        unsigned long int tamanho_matriz_i = calcula_tam_buffer_to_matriz(lista_buffers[i], "\n");
        lista_buffers[i]->pos_max_matriz = tamanho_matriz_i;
        free(nome_arquivos_temp);
    }
    //Para armazenar o menor valor encontrado
    char menor[255];
    buffer->conteudo = (char*)malloc(buffer->tamanho * (sizeof(char) + 3));

    printf("[...Mergeando arquivos...]\n");
    while(procuraMenor(lista_buffers, numArqs, menor))
    {
        unsigned long int prever_tamanho = buffer->posicao + strlen(menor);
        if(prever_tamanho < buffer->tamanho_original)
        {
            strcat(buffer->conteudo, menor);
            strcat(buffer->conteudo, "\n");
            buffer->posicao += strlen(menor);
        }
        else
        {
            //buffer cheio == Salvar no arquivo, limpar buffer, armazenar novo dado
            //Escrevo o buffer no arquivo de saida
            write_buffer_on_file(nome_arq_saida, buffer);
            
            free(buffer->conteudo);
            buffer->conteudo = (char*)malloc(buffer->tamanho * (sizeof(char) + 3));
            strcpy(buffer->conteudo, menor);
            strcat(buffer->conteudo, "\n");

            //Salvo o tamanho do inicio do buffer
            buffer->posicao = strlen(menor);
        }
    }

    write_buffer_on_file(nome_arq_saida, buffer);

    //Limpando a lista de buffers com os arquivos
    for(int i = 0; i < numArqs; i++) 
    {
        freeBuffer(lista_buffers[i]);
    }

    free(buffer);
}

void mergeSortExterno(char *nome_arquivo_entrada, char *nome_arq_saida, unsigned long int tam_ram)
{
    char arquivos_temp[100];
    /*Quebrar os arquivos em partes menores e depois ordenar
    Retorna o numero de arquivos que foram criados*/
    unsigned long int numArqs = criarArquivosOrdenados(nome_arquivo_entrada, tam_ram);
    printf("--> Quantidade de Arquivos: %ld\n", numArqs);
    
    //k == numero de buffers que vao ser criados
    /*ou seja, k eh o tamanho que comporta pelo menos um pedacinho de cada arquivo para levar
    para a ram e fazer a intercalacao*/

    unsigned long int pedaco_ram = tam_ram / (numArqs + 1);
    printf("--> Pedaco de RAM pra cada arquivo: %ld Bytes\n", pedaco_ram);
    
    //Cria o arquivo de saida e ja ordenado
    merge(nome_arq_saida, numArqs, pedaco_ram);

    //Apagar os arquivos temporarios  
    printf("[...Apagando arquivos temporarios...]\n")  ;
    for(int i = 0; i< numArqs; i++)
    {
        //Formata o nome dos arquivos e os apaga
        sprintf(arquivos_temp, "Arquivos_Saida/Temp%d.txt", i+1);
        remove(arquivos_temp);
    }
}

//argv[1] == Arquivo de Entrada, argv[2] == Arquivo de Saida
int main(int argc, char *argv[])
{
    unsigned long int tam_ram;
    if(argc < 4)
    {
        printf("--> ERROR: Informar arquivo de entrada, arquivo de saida e tamanho maximo em Bytes da RAM\n");
        printf("--> EXEMPLO: ./exec arquivo_entrada arquivo_saida 10500\n");
        return 0;
    }
    else
    {
        
        if(!arquivoExiste(argv[1])) 
        {
            printf("--> ERROR: ARQUIVO DE ENTRADA NAO EXISTE!\n");
            return 0;
        }
        
        char *ptr;
        tam_ram = strtoul(argv[3], &ptr, 10);
        printf("--> Capacidade maxima da RAM: %ld Bytes\n", tam_ram);


    }
    //Utilizado para armazenar os arquivos temporarios
    mkdir("Arquivos_Saida", 0700);

    //Reseta o arquivo de saida
    cria_reset_file(argv[2]);

    //Exemplo de tamanhos em bytes: 151200, 10500
    mergeSortExterno(argv[1], argv[2], tam_ram);
    printf("--> Arquivo ordenado com sucesso!\n--> Saida: %s\n", argv[2]);
    return 0;
}