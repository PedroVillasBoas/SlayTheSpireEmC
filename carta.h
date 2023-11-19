#ifndef CARTA_H
#define CARTA_H

#include <stdio.h> // Biblioteca padrao de entrada e saida
#include <stdlib.h> // Biblioteca padrao do C
#include <string.h> // Biblioteca para manipulaçao de strings


// Enum para cada Classe que o jogador pode escolher
typedef enum 
{
    BARALHO_GUERREIRO = 1,
    BARALHO_MAGO = 2,
    BARALHO_ARQUEIRO = 3
} TipoBaralho;

// Enum para cada tipo de açao das cartas
typedef enum 
{
    ATAQUESING = 1, // Ataca apenas um inimigo
    ATAQUEMULT = 2, // Ataca multiplos inimigos
    DEFESA = 3,     // Gera escudo
    CURA = 4,       // Cura o jogador
} TipoAcao;

// Estrutura para representar a carta
typedef struct Carta 
{
    char nome[20];        // Nome da carta
    int energia;          // Energia que a carta custa para ser jogada
    TipoAcao acao;        // 1 para ataque, 2 para defesa, 3 para cura
    int quantidadeAcao;   // Quantidade de dano/cura/escudo que a carta vai aplicar
    char descricao [200]; // Descriçao da carta
    int jaJogada;         // 1 se a carta ja foi jogada no turno atual, 0 caso contrario
    struct Carta* proximo;
    struct Carta* anterior;
} Carta;

Carta* criarCarta(char* nome, int energia, int acao, int quantidadeAcao, char* descricao) ;
Carta** criarBaralho(TipoBaralho tipoBaralho);
void liberarBaralho(Carta** cartas, int numCartas);
void resetarEstadoCartas(Carta** cartas, int numCartas);

#endif // CARTA_H
