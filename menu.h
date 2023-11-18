#ifndef MENU_H
#define MENU_H

#include "fases.h"
#include "util.h"
#include "carta.h"
#include "highscore.h"
#include "player.h"
#include "turno.h"

int mostrarIntro(int opcao);
void mostrarMenuPrincipal(Fase* faseAtual);
void escolherDificuldade(Fase** faseAtual);
void instrucoes();
int escolherClasses();
void telaVitoria();
void telaDerrota();

#endif // MENU_H