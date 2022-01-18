#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//MODIFICAR ISSO AQUI
#define TABLE_SIZE 4

typedef struct HashT HashT;
typedef struct CellHT CellHT;

struct CellHT
{
    char *key;
    char *value;
    CellHT* next_cell;

    /*
    char *complete_name;
    char *phone;
    char *city;
    char *country;
    */
    
};

struct HashT
{

    CellHT **buckets;
    unsigned long int size;
};


//Criando a matriz de Hash
HashT *create_ht(unsigned long size_ht)
{
    //Alocando espaco para a tabela
    HashT *hashtable = malloc(sizeof(hashtable) * 1);

    //Allocando espaco para cada bucket
    hashtable->buckets = malloc(sizeof(CellHT*) * size_ht);

    //Setando cada bucket para NULL
    for(int i = 0; i < size_ht; i++)
    {
        hashtable->buckets[i] = NULL;
    }

    //Associando o tamanho da tabela
    hashtable->size = size_ht;

    return hashtable;
}

//Calcula qual vai ser a chave hash do respectivo valor
//Renomear para hash_ht
unsigned int hash(char *key)
{
    unsigned long int value = 0;
    unsigned int key_len = strlen(key);
    
    //Gerando a chave
    for(int i = 0; i < key_len; i++)
    {
        value = value * 37 + key[i];
    }

    //Associando a respectiva posicao da tabela de hash
    value = value % TABLE_SIZE;

    return value;
}

//Criando uma celula dado os valores
CellHT *create_cell_ht(char *key, char *value)
{
    CellHT *cell = malloc(sizeof(cell) * 1);
    cell->key = malloc(strlen(key) + 1);
    cell->value = malloc(strlen(value) + 1);

    //Alterar para os dados do trabalho
    strcpy(cell->key, key);
    strcpy(cell->value, value);

    cell->next_cell = NULL;

    return cell;
}

void set_value_ht(HashT *hashtable, char *key, char *value)
{
    //Calcula a chave
    unsigned long int slot = hash(key);

    //Captura o valor dessa posicao da tabela
    CellHT *cell = hashtable->buckets[slot];

    //Se o slot estiver vazio, seta o valor
    if(!cell)
    {
        hashtable->buckets[slot] = create_cell_ht(key, value);
        return;
    }

    //Se nao estiver vazio: necessario atualizar ou inserir na proxima celula do slot
    CellHT *prev;
    while(cell != NULL)
    {
        //Chaves iguais e hashs iguais logo, atualizar valor
        //Atualizar para telefone, cidade e pais
        if(strcmp(cell->key, key) == 0)
        {
            free(cell->value);
            cell->value = malloc(sizeof(value) + 1);
            strcpy(cell->value, value);
            return;
        }

        //Anda para a proxima celula do respectivo slot
        prev = cell;
        cell = prev->next_cell;
    }

    //Se chegou ao fim do slot e nao encontrou uma celula de mesma chave, cria uma nova no slot
    prev->next_cell = create_cell_ht(key, value);
}

char *get_value_ht(HashT *hashtable, char *key)
{
    //Calcula a chave
    unsigned long int slot = hash(key);

    //Tenta encontrar um valor no slot da chave
    CellHT *cell = hashtable->buckets[slot];

    //Nada encontrado
    if(cell == NULL) return NULL;

    //Caso encontre um hash da respectiva chave, verificar se eh a mesma chave
    while(cell != NULL)
    {
        //Se a chave for a mesma informada, ou seja, hash e chaves iguais, sucesso
        if(strcmp(cell->key, key) == 0)
        {
            return cell->value;
        }

        //Se nao for a mesma chave informada, tenta verificar se possui uma proxima celula de mesma chave
        cell = cell->next_cell;
    }

    //Encontrou nada
    return NULL;
}

void show_ht(HashT *hashtable)
{
    for(int i = 0; i < hashtable->size; i++)
    {
        CellHT *cell = hashtable->buckets[i];
        if(cell == NULL) continue;

        printf("slot[%4d]: ", i);

        for(;;)
        {
            printf("%s=%s ", cell->key, cell->value);
            if(cell->next_cell == NULL) break;
            cell = cell->next_cell;
        }
        printf("\n");
    }
}

void destroy_ht(HashT *hashtable)
{
    printf("\n...Destroy Hashtable...\n... ...\n");
    CellHT *cell;
    CellHT *next;
    for(int i = 0; i < hashtable->size; i++)
    {
        cell = hashtable->buckets[i];
        while(cell != NULL)
        {
            next = cell->next_cell;
            free(cell->value);
            free(cell->key);
            free(cell);
            cell = next;
        }
    }

    free(hashtable->buckets);
    free(hashtable);
    printf("...Destroy Finish...\n");
}

//Criar funcao para destruir a matriz
//Testar get cell
int main(int argc, char **argv)
{
    //printf("%d\n", hash("em"));
    HashT *hashtable = create_ht(4);


    set_value_ht(hashtable, "Nome1", "KELVIN LEHRBACK");
    set_value_ht(hashtable, "Nome2", "CLARA LEHRBACK");
    set_value_ht(hashtable, "Nome3", "KELEN LEHRBACK");
    set_value_ht(hashtable, "Nome4", "VALDECI DOS SANTOS LEHRBACK");
    set_value_ht(hashtable, "Nome5", "LUCAS LEHRBACK");
    set_value_ht(hashtable, "Nome6", "RICHIELI LEHRBACK");
    set_value_ht(hashtable, "Nome7", "JANE LEHRBACK");
    set_value_ht(hashtable, "Nome8", "CARLOS LEHRBACK");

    show_ht(hashtable);
    printf("\n");
    char key_teste[] = "Nome6";
    printf("CHAVE: %s, VALOR: %s\n", key_teste, get_value_ht(hashtable, key_teste));

    destroy_ht(hashtable);
    return 0;
}