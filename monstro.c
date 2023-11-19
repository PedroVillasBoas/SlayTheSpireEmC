// Inclui todas as funcionalidades dos Monstros

#include "monstro.h"

// Criaçao do Monstro
Monstro* criarMonstro(char* nome, int hp, int defesa, int acao, int danoAtaqueMonstro, int defesaParaAdicionarMonstro) 
{
    Monstro* novoMonstro = (Monstro*)malloc(sizeof(Monstro)); // Alocando memoria para o monstro
    if (!novoMonstro) 
    {
        printf("Erro ao alocar memoria para o monstro.\n");
        exit(1);
    }
    strncpy(novoMonstro->nome, nome, sizeof(novoMonstro->nome));          // Copia o nome para a Struct
    novoMonstro->nome[sizeof(novoMonstro->nome) - 1] = '\0';              // Garante que a string e terminada em '\0'
    novoMonstro->hp = hp;                                                 // Copia o hp para a Struct
    novoMonstro->defesa = defesa;                                         // Copia a defesa para a Struct
    novoMonstro->acao = acao;                                             // Copia a açao para a Struct
    novoMonstro->intencaoDefinida = 0;                                    // Inicia o monstro sem nenhuma açao definida
    novoMonstro->danoAtaqueMonstro = danoAtaqueMonstro;                   // Dano que o monstro vai causar ao player
    novoMonstro->defesaParaAdicionarMonstro = defesaParaAdicionarMonstro; // Defesa que o monstro vai adicionar ao seu hp

    novoMonstro->proximo = NULL;
    novoMonstro->anterior = NULL;
    return novoMonstro;
}

// Libera a memoria de todos os monstros
void liberarMonstros(Monstro* monstro) 
{
    while (monstro != NULL) 
    {
        Monstro* proximo = monstro->proximo;
        free(monstro);
        monstro = proximo;
    }
}

// Definir as intenções dos monstros
void definirIntencoesMonstros(Monstro* listaMonstros) 
{
    Monstro* monstroAtual = listaMonstros; // Inicia o monstro atual como o primeiro da lista
    while (monstroAtual != NULL) 
    {
        if (!monstroAtual->intencaoDefinida) 
        {
            monstroAtual->acao = acaoMonstro(); // Escolhe uma açao para o monstro
            monstroAtual->intencaoDefinida = 1; // Define a flag de intençao do monstro como verdadeira
        }
        monstroAtual = monstroAtual->proximo;
    }
}

// Escolhe uma açao para o monstro
TipoAcaoMonstro acaoMonstro() 
{
    int numeroAleatorio = rand() % 100 + 1; // Gera um numero aleatorio entre 1 e 100

    if (numeroAleatorio <= 50)
    {
        return ATAQUEMONSTRO;
    } 
    else 
    {
        return DEFESAMONSTRO;
    }
}

// Resetar a defesa dos monstros para o proximo turno
void resetarDefesaMonstros(Monstro* listaMonstros) 
{
    Monstro* monstroAtual = listaMonstros; // Inicia o monstro atual como o primeiro da lista
    while (monstroAtual != NULL) 
    {
        monstroAtual->defesa = 0;          // Reseta a defesa do monstror para 0
        monstroAtual = monstroAtual->proximo;
    }
}