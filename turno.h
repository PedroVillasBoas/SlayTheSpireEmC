#ifndef TURNO_H
#define TURNO_H

#include "util.h"
#include "fases.h"
#include "carta.h"
#include "highscore.h"
#include "menu.h"

int iniciarJogo(Fase** faseAtual, Carta** cartas);
void jogarTurno(Fase** faseAtual, Carta** cartas); 
void finalizarTurno(Monstro* listaMonstros); 
void mostrarInformacoesTurnoJogador(Carta** cartas); 
void mostrarInformacoesTurnoMonstros(Fase* faseAtual);
void escolherEJogarCarta(Fase* faseAtual, Carta** cartas); 
void jogarCarta(Carta* carta, Monstro* monstro, Fase* faseAtual);
#endif // MENU_H