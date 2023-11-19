#ifndef MONSTRO_H
#define MONSTRO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enum para definir as açoes do monstro
typedef enum 
{
    DEFAULTMONSTRO = 0, // Inicia o monstro sem nenhuma açao definida
    ATAQUEMONSTRO = 1,  // O monstro vai atacar
    DEFESAMONSTRO = 2   // O monstro vai se defender
} TipoAcaoMonstro;

// Struct do Monstro
typedef struct Monstro 
{
    char nome[20];                  // Nome do monstro
    int hp;                         // HP do monstro
    int defesa;                     // Defesa do monstro
    TipoAcaoMonstro acao;           // Açao que o monstro vai tomar
    int danoAtaqueMonstro;          // Dano que o monstro vai causar ao player
    int defesaParaAdicionarMonstro; // Defesa que o monstro vai adicionar ao seu hp
    int intencaoDefinida;           // Flag para saber se a intençao do monstro ja foi definida
    struct Monstro* proximo;
    struct Monstro* anterior;
} Monstro;

Monstro* criarMonstro(char* nome, int hp, int defesa, int acao, int danoAtaqueMonstro, int defesaParaAdicionarMonstro);
void liberarMonstros(Monstro* monstro);
void definirIntencoesMonstros(Monstro* listaMonstros);
TipoAcaoMonstro acaoMonstro();
void resetarDefesaMonstros(Monstro* listaMonstros);

#endif // MONSTRO_H
