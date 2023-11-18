#include "util.h"
#include <stdio.h>
#include <stdlib.h>

    int dificuldade = 2; // Salvar a escolha da dificuldade
    int numTurno = 1; // Quantidade de turnos que passaram desde que o jogador começou o jogo
    int turnoFinalizado = 0; // Se o jogador escolheu terminar o seu turno
    int fimDeJogo = 0; // Se o jogo acabou Ou por HP do jogador == 0 ou Jogador ganhou o jogo!

// Atributos do Jogador
    char nomeJogador[30]; // Salvar o nome do jogador
    int hpJogador = 10; // Salvar o hp do jogador
    int energiaJogador = 3; // Salvar a energia do jogador
    int defesaJogador = 0; // Salvar a defesa acumulada das cartas que o jogador jogou
    int numCartasBaralho = 6;

// Limpa a tela
void clearScreen() 
{
    printf("\033[H\033[J");
}

// Imprime um texto letra por letra com um delay entre os chars
void printCharByChar(const char *str, useconds_t delay) 
{
    for (int i = 0; str[i] != '\0'; i++) 
    {
        printf("%c", str[i]);
        fflush(stdout);
        usleep(delay);
    }
}

// Retorna o maior valor entre a e b
int max(int a, int b) 
{
    return (a > b) ? a : b;
}

// Retorna o menor valor entre a e b
int min(int a, int b) 
{
    return (a < b) ? a : b;
}

// Verifica se algum monstro morreu
void verificarMonstroVivo(Fase* faseAtual) 
{
    Monstro* monstroAtual = faseAtual->monstros;
    Monstro* monstroAnterior = NULL;
    
    while(monstroAtual != NULL) 
    {
        if(monstroAtual->hp <= 0) 
        {
            if(monstroAnterior != NULL) 
            {
                monstroAnterior->proximo = monstroAtual->proximo;
            } 
            else 
            {
                faseAtual->monstros = monstroAtual->proximo;
            }
            Monstro* monstroParaLiberar = monstroAtual;
            monstroAtual = monstroAtual->proximo;
            free(monstroParaLiberar);
        } 
        else 
        {
            monstroAnterior = monstroAtual;
            monstroAtual = monstroAtual->proximo;
        }
    }
}

// Ordena os monstros por HP usando Insertion Sort
void ordenarMonstrosPorHP(Monstro** listaMonstros) 
{
    Monstro *sorted = NULL;
    Monstro *current = *listaMonstros;
    while (current != NULL) 
    {
        Monstro *next = current->proximo;

        // Localiza onde inserir o nodo atual na lista ordenada
        if (sorted == NULL || sorted->hp >= current->hp) 
        {
            current->proximo = sorted;
            sorted = current;
        } 
        else 
        {
            Monstro *currSorted = sorted;
            while (currSorted->proximo != NULL && currSorted->proximo->hp < current->hp) 
            {
                currSorted = currSorted->proximo;
            }
            current->proximo = currSorted->proximo;
            currSorted->proximo = current;
        }

        // Atualiza o node atual
        current = next;
    }

    *listaMonstros = sorted;
}

// Reseta o estado do jogo para o estado inicial
void resetarEstadoJogo() 
{
    hpJogador = 10; // Valor inicial do HP do jogador
    energiaJogador = 3; // Valor inicial da energia
    numTurno = 1; // Resetar o contador de turnos
    fimDeJogo = 0; // Resetar a condiçao de fim de jogo
    turnoFinalizado = 0; // Resetar a variavel de controle do turno
}