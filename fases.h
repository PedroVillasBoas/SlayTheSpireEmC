#ifndef FASES_H
#define FASES_H

#include "monstro.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Fase {
    int nivelFase;
    Monstro* monstros;
    char descricao[100];
    struct Fase* proxima;
} Fase;

Fase* criarFase(int nivel, char* descricao, Monstro* listaMonstros);
Fase* criarTodasFases(int dificuldade);
void liberarFases(Fase* fase);

#endif // FASES_H
