#ifndef UTILS_VECTOR
#define UTILS_VECTOR

void quick_sort(int vetor[], int inicio, int fim);
void print_vetor(int vetor[], int tam);
int busca_binaria(int alvo, int vetor[], int tam);
int file_to_vetor(char *nome_arquivo, int **vetor);

#endif