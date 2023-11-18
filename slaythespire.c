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


int main() 
{
    srand(time(NULL)); // Inicializa a semente do gerador de numeros aleatorios

    int opcao = 0;
    opcao = mostrarIntro(opcao);

    Fase* faseInicial = criarTodasFases(dificuldade); // Criando as fases e monstros

    mostrarMenuPrincipal(faseInicial);

    liberarFases(faseInicial);
    return 0;
}