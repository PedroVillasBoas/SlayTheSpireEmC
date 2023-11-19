#ifndef FASES_H
#define FASES_H

#include "monstro.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Estrutura da Fase
typedef struct Fase 
{
    int nivelFase;       // Nivel da fase
    Monstro* monstros;   // Lista duplamente encadeada de monstros
    char descricao[100]; // Descriçao da fase
    struct Fase* proxima;
} Fase;

Fase* criarFase(int nivel, char* descricao, Monstro* listaMonstros);
Fase* criarTodasFases(int dificuldade);
void liberarFases(Fase* fase);

#endif // FASES_H
