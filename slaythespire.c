#include <stdio.h>
#include <stdlib.h>

// O que falta fazer:
    // Monstros e cartas:
    // Adicionar os monstros com todos os campos (que eles tem do Struct) preenchidos;
    // Adicionar mecanica das ações dos monstros;
    // Adicionar as cartas com todos os campos (que eles tem do Struct) preenchidos;
    // Adicionar mecanica das ações das cartas;

    // Turno e Jogabilidade:
    // Adicionar como funciona mecanica de turno;
    // Adicionar condição para fim do turno;
    // Adicionar condição para fim do jogo;

    // Fases:
    // Adicionar mecanica para mudança de fase;

    // Caso eu tenha esquecido de algo, coloca aqui:


// Estrutura para representar o monstro
typedef struct Monstro 
{
    char nome[20];
    int hp;
    int acao;
    struct Monstro* proximo;
    struct Monstro* anterior;
} Monstro;

// Estrutura para representar a carta
typedef struct Carta 
{
    char nome[20];
    int energia;
    int acao;
    char descricao [100];
    struct Carta* proximo;
    struct Carta* anterior;
} Carta;

// Estrutura para representar uma fase
typedef struct Fase 
{
    Monstro* monstros;
    char descricaoFase[100];
    struct Fase* proxima;
} Fase;

// Destruir essa estrutura depois e fazer as variaveis de hp e de energia separadas e globais
// AS cartas serão utilizadas de forma em um array com as estuturas

// Protótipos das funções
void iniciarJogo(Fase* faseAtual, Carta** cartas);
void mostrarMenuPrincipal(Fase* faseInicial, Carta** cartas);
void escolherDificuldade(Fase* faseInicial, Carta** cartas);
void mostrarInstrucoes();
Monstro* criarMonstro(int hp);
Carta* criarCarta(int energia);
Fase* criarFase();
void jogarTurno(Fase* faseAtual, Carta** cartas); 
void mostrarInformacoesTurno(Fase* faseAtual, Carta** cartas);
void jogarCarta(Carta* carta, Monstro* monstro);
void escolherEJogarCarta(Fase* faseAtual, Carta** cartas);
int verificarFimDeJogo();
int verificarFimDeTurno(Fase* faseAtual);


// Variaveis globais
int dificuldade; // Salvar a escolha da dificuldade
int hpJogador; // Salvar o hp do jogador
int energiaJogador; // Salvar a energia do jogador
int turnoAtual; // Turno em que o jogador está

// Como a gente pode fazer pra de acordo com a dificuldade, mudar o hp dos monstros?
int main() 
{
    Fase* fase1 = criarFase();
    Fase* fase2 = criarFase();
    Fase* fase3 = criarFase();

    fase1->monstros = criarMonstro(5); // Supondo 5 de HP para cada monstro, só um exemplo
    fase2->monstros = criarMonstro(7);
    fase3->monstros = criarMonstro(10);

    fase1->proxima = fase2;
    fase2->proxima = fase3;

    // A criação das cartas tem que ser feito de forma individual igual aos monstros
    // Ja que cada uma vai ter um custo, uma ação e uma descrição
    Carta* cartas[6];
    for (int i = 0; i < 6; i++) 
    {
        cartas[i] = criarCarta(1); // Supondo 1 de energia para cada carta, só um exemplo
    }


    mostrarMenuPrincipal(fase1, cartas);
    return 0;
}

void iniciarJogo(Fase* faseAtual, Carta** cartas) 
{
    hpJogador = 10;
    energiaJogador = 3;
    turnoAtual = 1;

    while (hpJogador > 0 && faseAtual != NULL) 
    {
        printf("HP do Jogador: %d, Energia: %d\n", hpJogador, energiaJogador);
        // Mostrar informações das cartas
        // Mostrar informações dos monstros
        // Loop do turno do jogador
        while (1) 
        {
            // Lógica do turno do jogador
            // Como a gente pode fazer pra mostrar e escolher a carta?
            // Como aplicar a logica da carta no monstro?

            char escolha;
            printf("Deseja encerrar o seu turno? (s/n): ");
            scanf(" %c", &escolha); // espaço antes de %c consome qualquer whitespace
            if (escolha == 's' || escolha == 'S') 
            {
                break;
            }
        }

        // Lógica do turno dos monstros
        // Renovar a energia do jogador
        energiaJogador = 3; // ou qualquer valor de energia máxima
        // Verificar se a fase foi concluída
            // A fase vai ser conluida quando os monstros acabarem ou o jogador morrer
    }
}


void mostrarMenuPrincipal(Fase* faseInicial, Carta** cartas) 
{
    int opcao;
    printf("1. Iniciar Jogo\n");
    printf("2. Escolher Dificuldade\n");
    printf("3. Instrucoes\n");
    printf("4. Sair\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);

    switch (opcao) 
    {
        case 1:
            iniciarJogo(faseInicial, cartas);
            break;
        case 2:
            escolherDificuldade(faseInicial, cartas);
            break;
        case 3:
            mostrarInstrucoes();
            break;
        case 4:
            printf("Saindo do jogo...\n");
            exit(0);
        default:
            printf("Opcao invalida! Por favor, escolha novamente.\n");
            mostrarMenuPrincipal(faseInicial, cartas);
    }
}

// Escolha da dificuldade aqui
void escolherDificuldade(Fase* faseInicial, Carta** cartas) 
{
    // Colocar um print com a dificuldade default e mostrar pro player em qual dificuldade atualmente ele esta
    printf("A dificuldade afeta apenas a quantidade de HP que cada monstro vai possuir...\n");
    printf("Dito isso, qual dificuldade voce gostaria de jogar?\n");
    printf("1. Facil\n");
    printf("2. Medio\n");
    printf("3. Dificil\n");
    printf("4. Voltar\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &dificuldade);

    switch (dificuldade) 
    {
        case 1:
            mostrarMenuPrincipal(faseInicial, cartas);
            break;
        case 2:
            mostrarMenuPrincipal(faseInicial, cartas);
            break;
        case 3:
            mostrarMenuPrincipal(faseInicial, cartas);
            break;
        case 4:
            mostrarMenuPrincipal(faseInicial, cartas);
            break;
        default:
            printf("Opção inválida! Por favor, escolha novamente.\n");
            escolherDificuldade(faseInicial, cartas);
    }
}

// Instruções do jogo aqui
void mostrarInstrucoes() 
{
    
}

// Monstro possui:
    // Nome;
    // HP;
    // Ação que irá tomar.
Monstro* criarMonstro(int hp) 
{
    Monstro* novoMonstro = (Monstro*)malloc(sizeof(Monstro));
    if (!novoMonstro) 
    {
        printf("Erro ao alocar memória para o monstro.\n");
        exit(1);
    }
    novoMonstro->hp = hp;
    // Inicializar outras propriedades 
    novoMonstro->proximo = NULL;
    novoMonstro->anterior = NULL;
    return novoMonstro;
}
// Carta possui:
    // Nome;
    // Custo de energia;
    // Ação que a carta faz.
    // A descrição da ação pro player.
Carta* criarCarta(int energia) 
{
    Carta* novaCarta = (Carta*)malloc(sizeof(Carta));
    if (!novaCarta) 
    {
        printf("Erro ao alocar memória para a carta.\n");
        exit(1);
    }
    novaCarta->energia = energia;
    // Inicializar outras propriedades 
    novaCarta->proximo = NULL;
    novaCarta->anterior = NULL;
    return novaCarta;
}

Fase* criarFase() 
{
    Fase* novaFase = (Fase*)malloc(sizeof(Fase));
    if (!novaFase) 
    {
        printf("Erro ao alocar memória para a fase.\n");
        exit(1);
    }
    // Inicializar as propriedades da fase 
    novaFase->monstros = NULL;
    novaFase->proxima = NULL;
    return novaFase;
}

void jogarTurno(Fase* faseAtual, Carta** cartas) 
{
    mostrarInformacoesTurno(faseAtual, cartas);
}

void mostrarInformacoesTurno(Fase* faseAtual, Carta** cartas) 
{
    printf("HP do Jogador: %d\n", hpJogador);
    printf("Energia: %d\n", energiaJogador);
    
    printf("Monstros:\n");
    Monstro* monstroAtual = faseAtual->monstros;
    while (monstroAtual != NULL) 
    {
        printf("Nome: %s, HP: %d\n", monstroAtual->nome, monstroAtual->hp);
        monstroAtual = monstroAtual->proximo;
    }
    
    printf("Suas Cartas:\n");
    for (int i = 0; i < 6; i++) 
    {
        printf("Nome: %s, Energia: %d, Descricao: %s\n", 
               cartas[i]->nome, 
               cartas[i]->energia, 
               cartas[i]->descricao);
    }
}

void jogarCarta(Carta* carta, Monstro* monstro) 
{
    if (energiaJogador >= carta->energia) 
    {
        energiaJogador -= carta->energia;
        // Aplica efeito da carta no monstro
        // O que tem aqui é so um exemplo
        // Depois a gente tem que implementar as ações de cada carta
    } 
    else 
    {
        printf("Energia insuficiente para jogar esta carta!\n");
    }
}

void escolherEJogarCarta(Fase* faseAtual, Carta** cartas) 
{
    printf("HP do Jogador: %d, Energia: %d\n", hpJogador, energiaJogador);

    //Mostra as cartas e pede para o jogador escolher uma
    printf("Escolha uma carta para jogar (digite o número correspondente):\n");
    for (int i = 0; i < 6; i++) 
    {
        printf("%d. %s (Custo: %d, Descrição: %s)\n", i+1, cartas[i]->nome, cartas[i]->energia, cartas[i]->descricao);
    }

    int escolhaCarta;
    scanf("%d", &escolhaCarta);

    if (escolhaCarta < 1 || escolhaCarta > 6) 
    {
        printf("Escolha inválida!\n");
        return;
    }

    Carta* cartaEscolhida = cartas[escolhaCarta - 1];

    // Verifica se o jogador tem energia suficiente pra usar a carta
    if (energiaJogador < cartaEscolhida->energia) 
    {
        printf("Você não tem energia suficiente para jogar essa carta!\n");
        return;
    }

    // Mostra os monstros e pede para o jogador escolher um (Isso depois vai ser sempre no inicio do turno junto com o hp e energia)
    printf("Escolha um monstro para usar a carta (digite o número correspondente):\n");
    Monstro* monstroAtual = faseAtual->monstros;
    int contadorMonstros = 1;
    while (monstroAtual != NULL) 
    {
        printf("%d. %s (HP: %d)\n", contadorMonstros, monstroAtual->nome, monstroAtual->hp);
        monstroAtual = monstroAtual->proximo;
        contadorMonstros++;
    }

    int escolhaMonstro;
    scanf("%d", &escolhaMonstro);

    // Aplica o efeito da carta no monstro que o jogador escolheu
    monstroAtual = faseAtual->monstros;
    for (int i = 1; i < escolhaMonstro; i++) 
    {
        if (monstroAtual != NULL) 
        {
            monstroAtual = monstroAtual->proximo;
        }
    }

    if (monstroAtual == NULL) 
    {
        printf("Escolha inválida!\n");
        return;
    }

    // Aqui a gente aplica o efeito da carta no monstro.
    // Isso vai depender de como a gente vai implementar as ações das cartas e dos monstros.
    // Um exemplo:
    monstroAtual->hp -= cartaEscolhida->acao;

    // Atualiza a energia do jogador
    energiaJogador -= cartaEscolhida->energia;
    printf("Você usou a carta %s no monstro %s.\n", cartaEscolhida->nome, monstroAtual->nome);
}

int verificarFimDeJogo() 
{
    if (hpJogador <= 0) 
    {
        printf("Você morreu! Fim de jogo.\n");
        return 1;
    }
    return 0;
}

int verificarFimDeTurno(Fase* faseAtual) 
{
    Monstro* monstroAtual = faseAtual->monstros;
    while (monstroAtual != NULL) 
    {
        if (monstroAtual->hp > 0) 
        {
            return 0; // Ainda tem monstros vivos
        }
        monstroAtual = monstroAtual->proximo;
    }
    printf("Todos os monstros estão mortos! Você venceu!\n");
    return 1;
}