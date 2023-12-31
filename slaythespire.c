#include <stdio.h> // Biblioteca padrao de entrada e saida
#include <stdlib.h> // Biblioteca padrao do C
#include <string.h> // Biblioteca para manipulaçao de strings
#include <time.h> // Biblioteca para manipulaçao de tempo
#include <unistd.h> // Biblioteca para usar a funçao usleep

// Bibliotecas criadas
#include "menu.h"
#include "util.h"
#include "fases.h"
#include "turno.h"
#include "carta.h"
#include "monstro.h"
#include "player.h"
#include "highscore.h"

// Pra fazer funcionar no windows executar os seguintes comandos no terminal (1 por vez):
// gcc -c slaythespire.c util.c turno.c player.c monstro.c menu.c highscore.c fases.c carta.c
// gcc -o SlayTheSpireGame slaythespire.o util.o turno.o player.o monstro.o menu.o highscore.o fases.o carta.o
// .\SlayTheSpireGame.exe

int main() 
{
    srand(time(NULL)); // Inicializa a seed do gerador de numeros aleatorios

    int opcao = 0;
    opcao = mostrarIntro(opcao); // Mostra a introduçao do jogo

    Fase* faseInicial = criarTodasFases(dificuldade); // Criando as fases e monstros

    mostrarMenuPrincipal(faseInicial); // Mostra o menu principal do jogo

    liberarFases(faseInicial); // Libera a memoria alocada para as fases e monstros
    return 0;
}