#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//
#include "buffer.h"
#include "arquivo.h"
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

void print_beuatiful_cell(CellHT *cell)
{
    if(cell == NULL) 
    {
        printf("Contato nao encontrado\n"); 
        return;
    }
    
    printf("Nome Completo: %s\n", cell->key);
    printf("Telefone: %s\n", cell->phone);
    printf("Cidade: %s\n", cell->city);
    printf("Pais: %s\n", cell->country);
}

void print_cell(CellHT *cell)
{
    if(cell == NULL) 
    {
        printf("NULL\n"); 
        return;
    }
    
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
unsigned int hash(unsigned long int table_size, char *key)
{
    unsigned long int value = 0;
    unsigned int key_len = strlen(key);
    
    //Gerando a chave
    for(int i = 0; i < key_len; i++)
    {
        value = value * 11 + key[i];
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
    printf("Size Hashtable: %ld\n", hashtable->size);
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

void free_cell(CellHT *cell)
{
    free(cell->key);
    free(cell->phone);
    free(cell->city);
    free(cell->country);
    free(cell);
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

void delete_value_ht(HashT *hashtable, char *key)
{
    unsigned long int slot = hash(hashtable->size, key);
    CellHT *cell = hashtable->buckets[slot];

    if(cell == NULL) return;

    if(strcmp(cell->key, key) == 0)
    {
        //TOPO
        hashtable->buckets[slot] = cell->next_cell;
        free_cell(cell);
        return;
    }

    CellHT *prev = cell;
    cell = cell->next_cell;
    while(cell != NULL)
    {
        if(strcmp(cell->key, key) == 0)
        {
            prev->next_cell = cell->next_cell;
            free_cell(cell);
            return;
        }
        prev = cell;
        cell = cell->next_cell;
    }
}

void update_phone(CellHT *cell, char *new_phone)
{
    if(cell == NULL) return;
    free(cell->phone);
    cell->phone = (char*)malloc(strlen(new_phone));
    strcpy(cell->phone, new_phone);
}

void update_city(CellHT *cell, char *new_city)
{
    if(cell == NULL) return;
    free(cell->city);
    cell->city = (char*)malloc(strlen(new_city));
    strcpy(cell->city, new_city);
}

void update_country(CellHT *cell, char *new_country)
{
    if(cell == NULL) return;
    free(cell->country);
    cell->country = (char*)malloc(strlen(new_country));
    strcpy(cell->country, new_country);
}

void write_hashtablet_on_file(HashT *hashtable, char *file_name)
{
    FILE *file = fopen(file_name, "w");
    CellHT *cell;
    for(int i = 0; i < hashtable->size; i++)
    {
        cell = hashtable->buckets[i];
        while(cell != NULL)
        {
            fprintf(file, "%s,%s,%s,%s,\n", cell->key, cell->phone, cell->city, cell->country);
            cell = cell->next_cell;
        }
    }
    fclose(file);
}

HashT *load_file_on_hashtable(char *file_name)
{
    if(!arquivoExiste(file_name)) return NULL;

    unsigned long int qtd_lines = size_file_in_lines(file_name);
    HashT *hashtable = create_ht(qtd_lines * 1.5);

    FILE *file = fopen(file_name, "r");
    char complete_name[100], phone[15], city[50], country[50];

    unsigned long int i = 0;
    for(i = 0; i < qtd_lines; i++)
    {
        fscanf(file, "%[^,],%[^,],%[^,],%[^,],\n", complete_name, city, phone, country);
        set_value_ht(hashtable, complete_name, phone, city, country);
    }
    fclose(file);
    return hashtable;
}

void print_menu()
{
    printf("_____________MENU_______________\n");
    printf("|1 --> Carregar arquivo.\t|\n");
    printf("|2 --> Localizar Contato.\t|\n");
    printf("|3 --> Inserir Novo Contato.\t|\n");
    printf("|4 --> Excluir Contato.\t\t|\n");	
    printf("|5 --> Atualizar Contato.\t|\n");
	printf("|6 --> Visualizar HashTable.\t|\n");
    printf("|7 --> Salvar Dados.\t\t|\n");
    printf("|8 --> Sair do programa.\t|\n");
    printf("________________________________|\n");

}

//Clear screen
void clears()
{
	system("clear");
}

void print_menu_update()
{
	printf("_____UPDATE_____________________\n");
	printf("|1 --> Atualizar Telefone.\t|\n");
	printf("|2 --> Atualizar Cidade.\t|\n");
	printf("|3 --> Atualizar Pais.\t\t|\n");
	printf("|4 --> Voltar\t\t\t|\n");
	printf("________________________________|\n");
}


//Testar get cell
int main(int argc, char **argv)
{
    
	//AJUSTAR FUNCOES PARA RETORNA CASO DE SUCESSO E CASO DE FALHA???
	int option = 0;
	char verify_option[100];
	FILE *file = NULL;
	HashT *hashtable = NULL;
	char file_name[100];
	char contact_name[100];
	CellHT *find_contact;
	char temp;
	char del_contact[100];
	char for_update[100];

	while(option != 8)
	{
		if(hashtable != NULL) printf("-> Arquivo Carregado: %s\n", file_name);
		print_menu();
		printf("-----> Escolha uma opcao: ");
		//scanf("%c", &temp);
		//scanf("%d", &option);
		scanf("%s", verify_option);
		if(strlen(verify_option) > 1) option = 0;
		else option = atoi(verify_option);
		switch(option)
		{
			case 1:
			{
				if(!file)
				{
					clears();
					printf("___CARREGAR ARQUIVO___\n");
					printf("-> Nome do arquivo (Caminho completo): ");
					scanf("%s", file_name);
					if(arquivoExiste(file_name))
					{
						hashtable = load_file_on_hashtable("Arquivos_Entrada/entrada1.csv");
					}
					else
					{
						printf("--> ARQUIVO NAO ENCONTRADO.\n");
					}

				}
				else
				{
					printf("-->ATENCAO: Primeiro salve os dados antes de carregar outro arquivo!\n");
				}
				break;
			}
			case 2:
			{
				if(hashtable != NULL)
				{
					//clears();
					printf("___LOCALIZAR CONTATO___\n");
					printf("-> Nome do contato: ");
					scanf("%c", &temp);
					scanf("%[^\n]s", contact_name);

					printf("BUSCA: %s\n", contact_name);
					find_contact = get_value_ht(hashtable, contact_name);
					print_beuatiful_cell(find_contact);
					printf("\n");

				}
				else
				{
					clears();
					printf("-->ATENCAO: Primeiro carregue um arquivo antes de realizar uma busca.\n");
				}
				break;
			}
			case 3:
			{
				if(hashtable != NULL)
				{
					clears();
					char new_name[100];
					char new_phone[100];
					char new_city[100];
					char new_country[100];
					
					printf("___INSERIR NOVO CONTATO__\n");
					printf("--> OBS: Caso insira o nome de um contato ja existente, o mesmo sera atualizado POR COMPLETO, CUIDADO!\n");
					//fgets(lixo, sizeof(lixo), stdin);
					printf("Nome completo: ");
					scanf("%c", &temp);
					scanf("%[^\n]s", new_name);
					printf("\n");

					printf("Telefone: ");
					scanf("%c", &temp);
					scanf("%[^\n]s", new_phone);
					printf("\n");

					printf("Cidade: ");
					scanf("%c", &temp);
					scanf("%[^\n]s", new_city);
					printf("\n");

					printf("Pais: ");
					scanf("%c", &temp);
					scanf("%[^\n]s", new_country);
					printf("\n");

					set_value_ht(hashtable, new_name, new_phone, new_city, new_country);
					printf("--> Contato inserido\n");
				}
				else
				{
					clears();
					printf("-->ATENCAO: Primeiro carregue um arquivo antes de realizar uma insercao.\n");
				}

				break;
			}
			case 4:
			{
				if(hashtable != NULL)
				{
					printf("___EXCLUIR CONTATO___\n");
					printf("Nome do contato para exclusao: ");
					scanf("%c", &temp);
					scanf("%[^\n]s", del_contact);
					printf("\n");
					delete_value_ht(hashtable, del_contact);
				}
				else
				{
					clears();
					printf("-->ATENCAO: Primeiro carregue um arquivo antes de realizar uma exclusao.\n");
				}
				break;
			}
			case 5:
			{
				if(hashtable != NULL)
				{
					int update = 0;
					printf("___ATUALIZAR CONTATO___\n");
					printf("Nome do contato para atualizacao: ");
					scanf("%c", &temp);
					scanf("%[^\n]s", contact_name);
					find_contact = get_value_ht(hashtable, contact_name);

					if(find_contact == NULL)
					{
						//Sair do loop
						clears();
						printf("--> Contato nao encontrado\n");
						update = 4;
					} 
					print_beuatiful_cell(find_contact);
					printf("\n");
					while(update != 4)
					{

						print_menu_update();
						printf("-> O que desejas atualizar? ");
						scanf("%d", &update);

						switch(update)
						{
							//Telefone
							case 1:
							{
								printf("-> Atualizar Telefone: ");
								scanf("%c", &temp);
								scanf("%[^\n]s", for_update);
								update_phone(find_contact, for_update);
								break;	
							}
							//Cidade
							case 2:
							{	
								printf("-> Atualizar Cidade: ");
								scanf("%c", &temp);
								scanf("%[^\n]s", for_update);
								update_city(find_contact, for_update);
								break;
							}
							//Pais
							case 3:
							{
								printf("-> Atualizar Pais: ");
								scanf("%c", &temp);
								scanf("%[^\n]s", for_update);
								update_country(find_contact, for_update);
								break;
							}
							case 4:
							{
								break;
							}
							default:
								printf("--Opcao invalida!--\n");
						}
					}
				}
				else
				{	
					printf("-->ATENCAO: Primeiro carregue um arquivo antes de realizar uma atualizacao.\n");
				}
				break;
			}
			case 6:
			{
				if(hashtable != NULL)
				{
					show_ht(hashtable);
				}
				else
				{
					printf("-->ATENCAO: Primeiro carregue um arquivo antes de visualizar a tabela.\n");
				}
				break;
			}
			case 7:
			{
				if(hashtable != NULL)
				{
					printf("___SALVAR DADOS___\n");
					write_hashtablet_on_file(hashtable, file_name);
				}
				else
				{
					printf("-->ATENCAO: Primeiro carregue um arquivo antes de salvar.\n");
				}
				break;
			}
			case 8:
			{
				printf("Bye Bye!\n");
				break;
			}
			//Criar funcao para fechar arquivo
			default:
				printf("--->OPCAO INVALIDA <---\n");

		}
	}
    
    return 0;
}