#include <stdio.h>
#include "fases.h"

// Criaçao da Fase
Fase* criarFase(int nivel, char* descricao, Monstro* listaMonstros) 
{
    Fase* novaFase = (Fase*)malloc(sizeof(Fase));
    if (!novaFase) 
    {
        printf("Erro ao alocar memoria para a fase.\n");
        exit(1);
    }
    novaFase->nivelFase = nivel;
    strncpy(novaFase->descricao, descricao, sizeof(novaFase->descricao));
    novaFase->descricao[sizeof(novaFase->descricao) - 1] = '\0';
    novaFase->monstros = listaMonstros;
    novaFase->proxima = NULL;
    return novaFase;
}

// Cria todas as fases e monstros
Fase* criarTodasFases(int dificuldade) 
{
    // Nome, HP, Defesa, Acao, DanoAtaqueMonstro, DefesaParaAdicionarMonstro
    // Criando 3 monstros para fase 1
    Monstro* monstro1 = criarMonstro("Goblin Guerreiro", 1 * dificuldade, 0, DEFAULTMONSTRO, 2, 1); 
    Monstro* monstro2 = criarMonstro("Goblin Arqueiro", 1 * dificuldade, 0, DEFAULTMONSTRO, 2, 1); 
    Monstro* monstro3 = criarMonstro("Orc Guerreiro", 2 * dificuldade, 0, DEFAULTMONSTRO, 3, 2); 

    // Criando 3 monstros para fase 2
    Monstro* monstro4 = criarMonstro("Hobgoblin", 2 * dificuldade, 0, DEFAULTMONSTRO, 3, 2); 
    Monstro* monstro5 = criarMonstro("Elfo Mago", 1 * dificuldade, 0, DEFAULTMONSTRO, 3, 1); 
    Monstro* monstro6 = criarMonstro("Succubus", 3 * dificuldade, 0, DEFAULTMONSTRO, 4, 2); 

    // Criando 2 monstros e o Boss para fase 3
    Monstro* monstro7 = criarMonstro("Succubus", 3 * dificuldade, 0, DEFAULTMONSTRO, 4, 2); 
    Monstro* monstro8 = criarMonstro("Dragao", 3 * dificuldade, 0, DEFAULTMONSTRO, 5, 3); 
    Monstro* monstro9 = criarMonstro("Rei Demonio", 5 * dificuldade, 0, DEFAULTMONSTRO, 6, 4); 

    // Conectando os monstros da fase 1 em uma lista duplamente encadeada
    monstro1->proximo = monstro2;
    monstro2->anterior = monstro1;
    monstro2->proximo = monstro3;
    monstro3->anterior = monstro2;

    // Conectando os monstros da fase 2 em uma lista duplamente encadeada
    monstro4->proximo = monstro5;
    monstro5->anterior = monstro4;
    monstro5->proximo = monstro6;
    monstro6->anterior = monstro5;

    // Conectando os monstros da fase 3 em uma lista duplamente encadeada
    monstro7->proximo = monstro8;
    monstro8->anterior = monstro7;
    monstro8->proximo = monstro9;
    monstro9->anterior = monstro8;
    
    // Criando as fases
    Fase* fase1 = criarFase(1, "A caverna escura", monstro1);
    Fase* fase2 = criarFase(2, "O calabouco da castelo", monstro4);
    Fase* fase3 = criarFase(3, "Castelo do Rei Demonio", monstro7);
    
    // Conectando as fases em uma lista simplesmente encadeada
    fase1->proxima = fase2;
    fase2->proxima = fase3;
    fase3->proxima = NULL;
    
    return fase1; // Retorna a primeira fase
}

// Libera a memoria de todas as fases
void liberarFases(Fase* fase) 
{
    while (fase != NULL) 
    {
        Fase* proxima = fase->proxima;
        
        liberarMonstros(fase->monstros); // Libera os monstros da fase atual
        free(fase); // Libera a memoria da fase

        fase = proxima;
    }
}