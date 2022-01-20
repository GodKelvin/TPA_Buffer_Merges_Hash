
#include <stdio.h>
#include <stdlib.h>

//Cria e/ou limpa o conteudo de um arquivo, tornando-o em branco
void cria_reset_file(char *nome_arquivo)
{
    FILE *file;
    file = fopen(nome_arquivo, "w");
    fclose(file);
}

/*
1 == Arquivo existe
0 == Arquivo nao existe
*/
int arquivoExiste(char *nome_arquivo)
{
    FILE *arquivo;
    arquivo = fopen(nome_arquivo, "r");
    if(arquivo)
    {
        fclose(arquivo);
        return 1;
    }
    else return 0;
}

//So consigo deletar se o arquivo existir
void deletaArquivo(char *nome_arquivo)
{
    if(arquivoExiste(nome_arquivo)) remove(nome_arquivo);
}

unsigned long int size_file_in_lines(char *file_name)
{
    if(!arquivoExiste(file_name)) return 0;

    FILE *file = fopen(file_name, "r");

    char *line = NULL;
    size_t len_line = 0;
    unsigned long int qtd_lines = 0;
    while(getline(&line, &len_line, file) != -1) 
    {
        qtd_lines+=1;
    }
    fclose(file);
    if(line) free(line);
    return qtd_lines;
}