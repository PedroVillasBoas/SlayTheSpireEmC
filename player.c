// Inclui todas as funcionalidades do Player
#include "player.h"

// Obtem o nome do jogador
void obterNomeJogador(char *nome, int tamanhoMaximo) 
{
    printf("Qual o seu nome, heroi(na)?  ");
    fgets(nome, tamanhoMaximo, stdin);

    // Remove a nova linha (se tiver) do final da string
    size_t length = strlen(nome);
    if (length > 0 && nome[length - 1] == '\n') 
    {
        nome[length - 1] = '\0';
    }
}