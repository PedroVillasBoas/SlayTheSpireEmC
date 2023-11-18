#ifndef UTIL_H
#define UTIL_H

#include <unistd.h>  // Usado para usleep
#include "monstro.h"
#include "fases.h"

// Variaveis globais
// Do jogo
extern int dificuldade; // Salvar a escolha da dificuldade
extern int numTurno; // Quantidade de turnos que passaram desde que o jogador começou o jogo
extern int turnoFinalizado; // Se o jogador escolheu terminar o seu turno
extern int fimDeJogo; // Se o jogo acabou Ou por HP do jogador == 0 ou Jogador ganhou o jogo!

// Atributos do Jogador
extern char nomeJogador[30]; // Salvar o nome do jogador
extern int hpJogador; // Salvar o hp do jogador
extern int energiaJogador; // Salvar a energia do jogador
extern int defesaJogador; // Salvar a defesa acumulada das cartas que o jogador jogou
extern int numCartasBaralho;

void clearScreen();
void printCharByChar(const char* str, useconds_t delay);
int max(int a, int b);
int min(int a, int b);
void verificarMonstroVivo(Fase* faseAtual);
void ordenarMonstrosPorHP(Monstro** listaMonstros);
void resetarEstadoJogo();


#endif // UTIL_H
