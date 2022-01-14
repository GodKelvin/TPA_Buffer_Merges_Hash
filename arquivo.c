
#include <stdio.h>

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
    if(arquivo = fopen(nome_arquivo, "r"))
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