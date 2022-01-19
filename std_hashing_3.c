#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashT HashT;
typedef struct CellHT CellHT;

struct CellHT
{
    char *key;
    char *phone;
    char *city;
    char *country;
    CellHT* next_cell;
};

struct HashT
{
    CellHT **buckets;
    unsigned long int size;
};

void print_cell(CellHT *cell)
{
    if(cell == NULL) 
    {
        printf("NULL\n"); 
        return;
    }
    /*
    printf("Nome Completo: %s", cell->key);
    printf("| Telefone: %s", cell->phone);
    printf("| Cidade: %s", cell->city);
    printf("| Pais: %s", cell->country);
    */
    printf("%s| ", cell->key);
    printf("%s| ", cell->phone);
    printf("%s| ", cell->city);
    printf("%s", cell->country);
    printf("\n");
}

//Criando a matriz de Hash
HashT *create_ht(unsigned long size_ht)
{
    //Alocando espaco para a tabela
    HashT *hashtable = (HashT*)malloc(sizeof(HashT) * 1);

    //Allocando espaco para cada bucket
    hashtable->buckets = malloc(sizeof(CellHT) * size_ht);

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
unsigned int hash(unsigned long int table_size, char *key)
{
    unsigned long int value = 0;
    unsigned int key_len = strlen(key);
    
    //Gerando a chave
    for(int i = 0; i < key_len; i++)
    {
        value = value * 15 + key[i];
    }

    //Associando a respectiva posicao da tabela de hash
    value = value % table_size;

    return value;
}

//Criando uma celula dado os valores
CellHT *create_cell_ht(char *key, char *phone, char *city, char *country)
{
    CellHT *cell = (CellHT*) malloc(sizeof(struct CellHT));
    cell->key = (char*) malloc(strlen(key) + 1);
    cell->phone = (char*) malloc(strlen(phone) + 1);
    cell->city = (char*) malloc(strlen(city) + 1);
    cell->country = (char*) malloc(strlen(country) + 1);

    //Alterar para os dados do trabalho
    strcpy(cell->key, key);
    strcpy(cell->phone, phone);
    strcpy(cell->city, city);
    strcpy(cell->country, country);

    cell->next_cell = NULL;

    return cell;
}

void set_value_ht(HashT *hashtable, char *key, char *phone, char *city, char *country)
{
    //Calcula a chave
    unsigned long int slot = hash(hashtable->size, key);

    //Captura o valor dessa posicao da tabela
    CellHT *cell = hashtable->buckets[slot];

    //Se o slot estiver vazio, seta o valor
    if(!cell)
    {
        hashtable->buckets[slot] = create_cell_ht(key, phone, city, country);
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
            free(cell->phone);
            free(cell->city);
            free(cell->country);

            cell->phone = malloc(sizeof(phone) + 1);
            cell->city = malloc(sizeof(city) + 1);
            cell->country = malloc(sizeof(country) + 1);

            strcpy(cell->phone, phone);
            strcpy(cell->city, city);
            strcpy(cell->country, country);
            return;
        }

        //Anda para a proxima celula do respectivo slot
        prev = cell;
        cell = prev->next_cell;
    }

    //Se chegou ao fim do slot e nao encontrou uma celula de mesma chave, cria uma nova no slot
    prev->next_cell = create_cell_ht(key, phone, city, country);
}

//char *get_value_ht(HashT *hashtable, char *key)
CellHT *get_value_ht(HashT *hashtable, char *key)
{
    //Calcula a chave
    unsigned long int slot = hash(hashtable->size, key);

    //Tenta encontrar um valor no slot da chave
    CellHT *cell = hashtable->buckets[slot];

    //Caso encontre um hash da respectiva chave, verificar se eh a mesma chave
    while(cell != NULL)
    {
        //Se a chave for a mesma informada, ou seja, hash e chaves iguais, sucesso
        if(strcmp(cell->key, key) == 0)
        {
            return cell;
        }

        //Se nao for a mesma chave informada, tenta verificar se possui uma proxima celula de mesma chave
        cell = cell->next_cell;
    }

    //Encontrou nada
    return NULL;
}

void show_ht(HashT *hashtable)
{
    printf("----------\n");
    for(int i = 0; i < hashtable->size; i++)
    {
        CellHT *cell = hashtable->buckets[i];
        if(cell == NULL) continue;

        printf("slot[%2d]:\n", i);

        while(cell != NULL)
        {
            print_cell(cell);
            if(cell->next_cell == NULL) break;
            cell = cell->next_cell;
        }
        printf("\n");
    }
    printf("----------\n");
}

void destroy_ht(HashT *hashtable)
{
    printf("\n...Destroy Hashtable...\n");
    CellHT *cell;
    CellHT *next;
    for(int i = 0; i < hashtable->size; i++)
    {
        cell = hashtable->buckets[i];
        while(cell != NULL)
        {
            next = cell->next_cell;
            free(cell->key);
            free(cell->phone);
            free(cell->city);
            free(cell->country);
            cell = next;
        }
        if(hashtable->buckets[i] != NULL) free(hashtable->buckets[i]);
    }
    free(hashtable);
}



//Testar get cell
int main(int argc, char **argv)
{
    //printf("%d\n", hash("em"));
    HashT *hashtable = create_ht(10000);

    set_value_ht(hashtable, "Kelvin", "999999999", "Cariacica", "Brasil");
    set_value_ht(hashtable, "Jamila", "999393939", "Manaus", "Brasil");
    set_value_ht(hashtable, "Temila", "999393939", "Manaus", "Brasil");
    set_value_ht(hashtable, "Camila", "999393939", "Manaus", "Brasil");

    show_ht(hashtable);
    printf("\n");
    CellHT * get_cell = get_value_ht(hashtable, "aa");
    print_cell(get_cell);

    destroy_ht(hashtable);
    return 0;
}