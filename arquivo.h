
#ifndef ARQUIVO_H
#define ARQUIVO_H
/*
1 == Arquivo existe
0 == Arquivo nao existe
*/
int arquivoExiste(char *nome_arquivo);

//Cria e/ou limpa o conteudo de um arquivo, tornando-o em branco
void cria_reset_file(char *nome_arquivo);
void deletaArquivo(char *nome_arquivo);
#endif
