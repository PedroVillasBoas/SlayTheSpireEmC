// Inclui todas as funcionalidades de utilidade do jogo

#include "util.h"
#include <stdio.h>
#include <stdlib.h>

// Variaveis globais
    int dificuldade = 2;      // Salvar a escolha da dificuldade
    int numTurno = 1;         // Quantidade de turnos que passaram desde que o jogador começou o jogo
    int turnoFinalizado = 0;  // Se o jogador escolheu terminar o seu turno
    int fimDeJogo = 0;        // Se o jogo acabou Ou por HP do jogador == 0 ou Jogador ganhou o jogo!

// Atributos do Jogador
    char nomeJogador[30];     // Salvar o nome do jogador
    int hpJogador = 10;       // Salvar o hp do jogador
    int energiaJogador = 3;   // Salvar a energia do jogador
    int defesaJogador = 0;    // Salvar a defesa acumulada das cartas que o jogador jogou
    int numCartasBaralho = 6; // Numero de cartas no baralho do jogador

// Limpa a tela
void clearScreen() 
{
    printf("\033[H\033[J");
}

void tituloGame()
{
printf(" _______  ___      _______  __   __    _______  __   __  _______    _______  _______  ___   ______    _______   \n");
printf("|       ||   |    |   _   ||  | |  |  |       ||  | |  ||       |  |       ||       ||   | |    _ |  |       |  \n");
printf("|  _____||   |    |  |_|  ||  |_|  |  |_     _||  |_|  ||    ___|  |  _____||    _  ||   | |   | ||  |    ___|  \n");
printf("| |_____ |   |    |       ||       |    |   |  |       ||   |___   | |_____ |   |_| ||   | |   |_||_ |   |___   \n");
printf("|_____  ||   |___ |       ||_     _|    |   |  |       ||    ___|  |_____  ||    ___||   | |    __  ||    ___|  \n");
printf(" _____| ||       ||   _   |  |   |      |   |  |   _   ||   |___    _____| ||   |    |   | |   |  | ||   |___   \n");
printf("|_______||_______||__| |__|  |___|      |___|  |__| |__||_______|  |_______||___|    |___| |___|  |_||_______|  \n");
printf("                                 _______  __   __    _______                                                    \n");
printf("                                |       ||  |_|  |  |       |                                                   \n");
printf("                                |    ___||       |  |       |                                                   \n");
printf("                                |   |___ |       |  |      _|                                                   \n");
printf("                                |    ___||       |  |     |                                                     \n");
printf("                                |   |___ | ||_|| |  |     |_                                                    \n");
printf("                                |_______||_|   |_|  |_______|                                                   \n");
printf("\n");
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
        if(monstroAtual->hp <= 0) // Se o monstro morreu
        {
            if(monstroAnterior != NULL)                           // Se o monstro nao for o primeiro da lista
            {
                monstroAnterior->proximo = monstroAtual->proximo; // O anterior aponta para o proximo do atual
            } 
            else 
            {
                faseAtual->monstros = monstroAtual->proximo;     // O primeiro da lista aponta para o proximo do atual
            }
            Monstro* monstroParaLiberar = monstroAtual;          // Salva o monstro atual para liberar a memoria
            monstroAtual = monstroAtual->proximo;                // Atualiza o monstro atual
            free(monstroParaLiberar);                            // Libera a memoria do monstro morto
        } 
        else 
        {
            monstroAnterior = monstroAtual;
            monstroAtual = monstroAtual->proximo;
        }
    }
}

void ordenarMonstrosPorHP(Monstro **listaMonstros) 
{
    Monstro *sorted = NULL;            // Lista ordenada
    Monstro *current = *listaMonstros; // Lista original

    while (current != NULL) // Enquanto nao chegar no final da lista
    {
        Monstro *next = current->proximo; // Salva o proximo nodo da lista original

        // Desconecta o nó atual da lista original
        if (current->anterior != NULL) 
        {
            current->anterior->proximo = current->proximo;
        }
        if (current->proximo != NULL) 
        {
            current->proximo->anterior = current->anterior;
        }

        // Localiza onde inserir o nodo atual na lista ordenada
        if (sorted == NULL || sorted->hp >= current->hp) 
        {
            current->proximo = sorted;
            current->anterior = NULL;
            if (sorted != NULL) 
            {
                sorted->anterior = current;
            }
            sorted = current;
        } 
        else // Se o nodo atual for maior que o primeiro da lista ordenada
        {
            Monstro *currSorted = sorted;
            while (currSorted->proximo != NULL && currSorted->proximo->hp < current->hp)  // Enquanto nao chegar no final da lista ordenada
            {
                currSorted = currSorted->proximo;
            }
            current->proximo = currSorted->proximo;
            current->anterior = currSorted;
            if (currSorted->proximo != NULL) // Se nao for o ultimo da lista ordenada
            {
                currSorted->proximo->anterior = current;
            }
            currSorted->proximo = current;
        }

        // Atualiza o node atual
        current = next;
    }

    *listaMonstros = sorted; // Atualiza a lista original
}

// Reseta o estado do jogo para o estado inicial
void resetarEstadoJogo() 
{
    hpJogador = 10;      // Valor inicial do HP do jogador
    energiaJogador = 3;  // Valor inicial da energia
    numTurno = 1;        // Resetar o contador de turnos
    fimDeJogo = 0;       // Resetar a condiçao de fim de jogo
    turnoFinalizado = 0; // Resetar a variavel de controle do turno
}