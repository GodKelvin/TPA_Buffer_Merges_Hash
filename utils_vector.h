#ifndef UTILS_VECTOR
#define UTILS_VECTOR

void quick_sort(int vetor[], int inicio, int fim);
void print_vetor(int vetor[], int tam);
int busca_binaria(int alvo, int vetor[], int tam);

int get_pos_pivo_string(char **matriz, int inicio, int fim);
void quick_sort_string(char **matriz, int inicio, int fim);
int busca_binaria_string(char alvo[], char **matriz, int tam);
int file_to_vetor(char *nome_arquivo, int **vetor);

#endif