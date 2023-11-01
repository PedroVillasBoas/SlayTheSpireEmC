#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// O que falta fazer:
    // Comunicação entre a ação da carta e os monstros
        // Verificação se existe escudo no monstro antes do player bater
    // Comunicação entre a ação dos monstros e o player
        // Verificação se existe escudo no player antes do monstro bater
    // Verificação de termino de fase (Se todos os monstros da fase morreram)
    // Troca de fase após a verificação
    // Condição do player ganhar o jogo
    // Definir e construir o boss

// Enum para cada tipo de ação das cartas
typedef enum 
{
    ATAQUESING = 1,
    ATAQUEMULT = 2,
    DEFESA = 3,
    CURA = 4,
} TipoAcao;

//Enum para cada tipo de ação dos monstros
typedef enum
{
    ATAQUEMONSTRO = 1,
    DEFESAMONSTRO = 2
} TipoAcaoMonstro;

// Estrutura para representar o monstro
typedef struct Monstro 
{
    char nome[20];
    int hp;
    int defesa;
    TipoAcaoMonstro acao;
    int intencaoDefinida; // 1 se a intenção já foi definida para o turno atual, 0 caso contrário
    struct Monstro* proximo;
    struct Monstro* anterior;
} Monstro;

// Estrutura para representar a carta
typedef struct Carta 
{
    char nome[20];
    int energia;
    TipoAcao acao;
    char descricao [100];
    struct Carta* proximo;
    struct Carta* anterior;
} Carta;

// Estrutura para representar uma fase
typedef struct Fase 
{
    int nivelFase;
    Monstro* monstros;
    char descricao[100];
    struct Fase* proxima;
} Fase;

// Protótipos das funções

void clearScreen();
void mostrarMenuPrincipal(Fase* faseAtual, Carta** cartas);
void escolherDificuldade();
void instrucoes();
void jogarTurno(Fase* faseAtual, Carta** cartas); 
Monstro* criarMonstro(char* nome, int hp, int defesa, int acao);
void definirIntencoesMonstros(Monstro* listaMonstros);
TipoAcaoMonstro acaoMonstro();
Carta* criarCarta(char* nome, int energia, int acao, char* descricao); 
Fase* criarFase(int nivel, char* descricao, Monstro* listaMonstros);
void iniciarJogo(Fase* faseAtual, Carta** cartas);
void jogarTurno(Fase* faseAtual, Carta** cartas); 
void finalizarTurno(Monstro* listaMonstros);
void mostrarInformacoesTurnoJogador(Carta** cartas); 
void mostrarInformacoesTurnoMonstros(Fase* faseAtual); 
void escolherEJogarCarta(Fase* faseAtual, Carta** cartas);
void jogarCarta(Carta* carta, Monstro* monstro, Fase* faseAtual);


// Variaveis globais
// Do jogo
int dificuldade = 2; // Salvar a escolha da dificuldade
int turnoAtual; // Turno em que o jogador está
int turnoFinalizado = 0; // Se o jogador escolheu terminar o seu turno
int fimDeJogo = 0; // Se o jogo acabou Ou por HP do jogador == 0 ou Jogador ganhou o jogo!

// Atributos do Jogador
int hpJogador = 10; // Salvar o hp do jogador
int energiaJogador = 3; // Salvar a energia do jogador
int defesaJogador = 0; // Salvar a defesa acumulada das cartas que o jogador jogou

int main() 
{
    srand(time(NULL)); // Inicializa a semente do gerador de números aleatórios

    // Criando 3 monstros
    Monstro* monstro1 = criarMonstro("Goblin", rand() % 101, rand() % 101, rand() % 2);
    Monstro* monstro2 = criarMonstro("Orc", rand() % 101, rand() % 101, rand() % 2);
    Monstro* monstro3 = criarMonstro("Dragao", rand() % 101, rand() % 101, rand() % 2);

    // Conectando os monstros em uma lista duplamente encadeada
    monstro1->proximo = monstro2;
    monstro2->anterior = monstro1;
    monstro2->proximo = monstro3;
    monstro3->anterior = monstro2;

    // Criando 6 cartas
    Carta* cartas[6];
    cartas[0] = criarCarta("Espada", 1, 1, "Uma espada brilhante");
    cartas[1] = criarCarta("Escudo", 1, 3, "Um escudo resistente");
    cartas[2] = criarCarta("Pocao de Cura", 3, 4, "Restaura a vida");
    cartas[3] = criarCarta("Arco", 2, 2, "Um arco para ataques a distancia");
    cartas[4] = criarCarta("Flecha Envenenada", 2, 1, "Uma flecha letal");
    cartas[5] = criarCarta("Magia de Fogo", 3, 2, "Uma poderosa bola de fogo");

    // Criando uma fase
    Fase* fase1 = criarFase(1, "A caverna escura", monstro1);

    mostrarMenuPrincipal(fase1, cartas);
    return 0;
}

void clearScreen() 
{
    printf("\033[H\033[J");
}

void mostrarMenuPrincipal(Fase* faseAtual, Carta** cartas) 
{
    clearScreen();
    int opcao;
    do 
    {
        printf("Menu Principal\n");
        printf("1. Jogar\n");
        printf("2. Escolher Dificuldade (Atual: %d)\n", dificuldade);
        printf("3. Como jogar?\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        switch(opcao) 
        {
            case 1:
                // Adicione aqui o código para iniciar o jogo
                iniciarJogo(faseAtual, cartas);
                printf("Jogo iniciado!\n");
                break;
            case 2:
                escolherDificuldade();
                break;
            case 3:
                instrucoes();
                break;
            case 4:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida! Por favor, escolha novamente.\n");
        }
    } while(opcao != 4);
}

// Escolha da dificuldade aqui
void escolherDificuldade() 
{
    clearScreen();
    int opcao;
    printf("A dificuldade afeta apenas a quantidade de HP que cada monstro vai possuir...\n");
    printf("Dificuldade atual: %d\n", dificuldade);
    printf("1. Facil\n");
    printf("2. Medio\n");
    printf("3. Dificil\n");
    printf("4. Voltar\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);

    if(opcao >= 1 && opcao <= 3) 
    {
        dificuldade = opcao;
        clearScreen();
    } 
    else if(opcao == 4) 
    {
        clearScreen();
        return;
    } 
    else 
    {
        printf("Opcao invalida! Por favor, escolha novamente.\n");
    }
}

void instrucoes()
{
    clearScreen();
    printf("Voce eh um guerreiro em ascenção e foi designado para proteger o reino contra o Rei Demonio e suas tropas.\n"
        "Nesse mundo ha tres fases e cada fase há um quantidade de monstros a serem derrotados (a depender da dificuldade escolhida,"
        "os monstros terao mais ou menos vida)\n. Para derrotar os inimigos você escolhera quais as suas acoes de acordo com a sua quantidade "
        "de energia ate o seu turno acabar. Quando terminar  o seu turno, os monstros também farao suas acoes contra ti. "
        "\nDerrote os monstros em cada fase e enfrente o Rei Demonio na fase final para salvar o reino.\n"
        "O jogo finaliza ou quando você morrer ou quando derrotar o Rei Demonio.");

}

// Criação do Monstro
Monstro* criarMonstro(char* nome, int hp, int defesa, int acao) 
{
    Monstro* novoMonstro = (Monstro*)malloc(sizeof(Monstro));
    if (!novoMonstro) 
    {
        printf("Erro ao alocar memória para o monstro.\n");
        exit(1);
    }
    strncpy(novoMonstro->nome, nome, sizeof(novoMonstro->nome));
    novoMonstro->nome[sizeof(novoMonstro->nome) - 1] = '\0'; // Garante que a string é terminada em '\0'
    novoMonstro->hp = hp;
    novoMonstro->defesa = defesa;
    novoMonstro->acao = acao;
    novoMonstro->intencaoDefinida = 0;

    novoMonstro->proximo = NULL;
    novoMonstro->anterior = NULL;
    return novoMonstro;
}

void definirIntencoesMonstros(Monstro* listaMonstros) 
{
    Monstro* monstroAtual = listaMonstros;
    while (monstroAtual != NULL) 
    {
        if (!monstroAtual->intencaoDefinida) 
        {
            monstroAtual->acao = acaoMonstro();
            monstroAtual->intencaoDefinida = 1;
        }
        monstroAtual = monstroAtual->proximo;
    }
}

TipoAcaoMonstro acaoMonstro() 
{
    int numeroAleatorio = rand() % 100 + 1; // Gera um número aleatório entre 1 e 100

    if (numeroAleatorio <= 50) 
    {
        printf("O monstro escolheu atacar!");
        return ATAQUEMONSTRO;
    } 
    else 
    {
        printf("O monstro escolheu defender!");
        return DEFESAMONSTRO;
    }
}

// Criação da carta
Carta* criarCarta(char* nome, int energia, int acao, char* descricao) 
{
    Carta* novaCarta = (Carta*)malloc(sizeof(Carta));
    if (!novaCarta) 
    {
        printf("Erro ao alocar memória para a carta.\n");
        exit(1);
    }
    strncpy(novaCarta->nome, nome, sizeof(novaCarta->nome));
    novaCarta->nome[sizeof(novaCarta->nome) - 1] = '\0'; // Garante que a string é terminada em '\0'
    novaCarta->energia = energia;
    novaCarta->acao = acao;
    strncpy(novaCarta->descricao, descricao, sizeof(novaCarta->descricao));
    novaCarta->descricao[sizeof(novaCarta->descricao) - 1] = '\0';

    novaCarta->proximo = NULL;
    novaCarta->anterior = NULL;
    return novaCarta;
}

// Criação da Fase
Fase* criarFase(int nivel, char* descricao, Monstro* listaMonstros) 
{
    Fase* novaFase = (Fase*)malloc(sizeof(Fase));
    if (!novaFase) 
    {
        printf("Erro ao alocar memória para a fase.\n");
        exit(1);
    }
    novaFase->nivelFase = nivel;
    strncpy(novaFase->descricao, descricao, sizeof(novaFase->descricao));
    novaFase->descricao[sizeof(novaFase->descricao) - 1] = '\0';
    novaFase->monstros = listaMonstros;
    novaFase->proxima = NULL;
    return novaFase;
}

void iniciarJogo(Fase* faseAtual, Carta** cartas) 
{
    while(fimDeJogo != 0 || hpJogador >= 0)
    {
        jogarTurno(faseAtual, cartas);
    }
    
}

void jogarTurno(Fase* faseAtual, Carta** cartas) 
{
    while(turnoFinalizado == 0)
    {
        clearScreen();
        definirIntencoesMonstros(faseAtual->monstros);
        mostrarInformacoesTurnoJogador(cartas);
        mostrarInformacoesTurnoMonstros(faseAtual);
        escolherEJogarCarta(faseAtual, cartas);
    }
    finalizarTurno(faseAtual->monstros);
}

void finalizarTurno(Monstro* listaMonstros) 
{
    Monstro* monstroAtual = listaMonstros;
    while (monstroAtual != NULL) 
    {
        monstroAtual->intencaoDefinida = 0;
        monstroAtual = monstroAtual->proximo;
    }

    energiaJogador = 3;
    defesaJogador = 0;
    turnoFinalizado = 0;
}

// Mostra as informações do jogador no turno atual
void mostrarInformacoesTurnoJogador(Carta** cartas) 
{
    printf("HP do Jogador: %d\n", hpJogador);
    printf("Energia: %d\n", energiaJogador);
    printf("Escudos: %d\n", defesaJogador);
    
    printf("Suas Cartas:\n");
    for (int i = 0; i < 6; i++) 
    {
        printf("[%d]Nome: %s, Energia: %d, Descricao: %s\n", 
        i+1,
        cartas[i]->nome, 
        cartas[i]->energia, 
        cartas[i]->descricao);
    }
}

// Mostra as informações dos monstros no turno atual
void mostrarInformacoesTurnoMonstros(Fase* faseAtual)
{
    printf("Monstros:\n");
        Monstro* monstroAtual = faseAtual->monstros;
        while (monstroAtual != NULL) 
        {
            printf("Nome: %s, HP: %d\n", monstroAtual->nome, monstroAtual->hp);
            if(faseAtual->nivelFase == 1)
            {
                if(monstroAtual->acao == 1)
                {
                    printf("%s, vai atacar com 2 de dano!\n", monstroAtual->nome);
                }
                else
                {
                    printf("%s, vai defender com 2 de defesa!\n", monstroAtual->nome);
                }
            }
            else if(faseAtual->nivelFase == 2)
            {
                if(monstroAtual->acao == 1)
                {
                    printf("%s, vai atacar com 4 de dano!\n", monstroAtual->nome);
                }
                else
                {
                    printf("%s, vai defender com 3 de defesa!\n", monstroAtual->nome);
                }
            }
            else // Depois ver como a gente vai fazer pra saber do dano e defesa do boss
            {
                if(monstroAtual->acao == 1)
                {
                    printf("%s, vai atacar com 4 de dano!\n", monstroAtual->nome);
                }
                else
                {
                    printf("%s, vai defender com 4 de defesa!\n", monstroAtual->nome);
                }
            }
            monstroAtual = monstroAtual->proximo;
        }
}

void escolherEJogarCarta(Fase* faseAtual, Carta** cartas) 
{
    
    int escolha;
    printf("Escolha uma carta para jogar (1-6) OU [7] Para encerrar o turno: ");
    scanf("%d", &escolha);
    
    if (escolha < 1 || escolha > 7) 
    {
        printf("Escolha invalida! Tente novamente.\n");
        return;
    }

    if(escolha == 7)
    {
        turnoFinalizado = 1;
    }
    
    Carta* cartaEscolhida = cartas[escolha - 1];
    jogarCarta(cartaEscolhida, faseAtual->monstros, faseAtual); // Exemplo: jogando a carta no primeiro monstro
}

void jogarCarta(Carta* carta, Monstro* monstro, Fase* faseAtual)
{
    Monstro* monstroAtual = faseAtual->monstros;
    if (energiaJogador >= carta->energia) 
    {
        energiaJogador -= carta->energia;
        printf("Voce jogou a carta %s!\n", carta->nome);
        
        switch (carta->acao) 
        {
            case ATAQUESING:
                monstro->hp -= 2;
                printf("Voce causou 2 de dano ao monstro %s!\n", monstro->nome);
                break;
            case ATAQUEMULT:
                while (monstroAtual != NULL)
                {
                    monstroAtual->hp -= 2;
                    printf("Voce causou 2 de dano ao monstro %s!\n", monstroAtual->nome);
                    monstroAtual = monstroAtual->proximo; // Ver porque o dano ta sendo aplicado no mesmo mob e nao em todos
                }
                free(monstroAtual);
                break;
            case DEFESA:
                defesaJogador += 3;
                break;
            case CURA:
                hpJogador += 2;
                break;
            default:
                printf("Açao desconhecida!\n");
                break;
        }
    } 
    else 
    {
        printf("Energia insuficiente para jogar esta carta!\n");
    }
}