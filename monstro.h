#ifndef MONSTRO_H
#define MONSTRO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    DEFAULTMONSTRO = 0,
    ATAQUEMONSTRO = 1,
    DEFESAMONSTRO = 2
} TipoAcaoMonstro;

typedef struct Monstro {
    char nome[20];
    int hp;
    int defesa;
    TipoAcaoMonstro acao;
    int danoAtaqueMonstro;
    int defesaParaAdicionarMonstro;
    int intencaoDefinida;
    struct Monstro* proximo;
    struct Monstro* anterior;
} Monstro;

Monstro* criarMonstro(char* nome, int hp, int defesa, int acao, int danoAtaqueMonstro, int defesaParaAdicionarMonstro);
void liberarMonstros(Monstro* monstro);
void definirIntencoesMonstros(Monstro* listaMonstros);
TipoAcaoMonstro acaoMonstro();
void resetarDefesaMonstros(Monstro* listaMonstros);

#endif // MONSTRO_H
