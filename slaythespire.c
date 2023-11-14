#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
    DEFAULTMONSTRO = 0,
    ATAQUEMONSTRO = 1,
    DEFESAMONSTRO = 2
} TipoAcaoMonstro;

// Estrutura para representar o monstro
typedef struct Monstro 
{
    char nome[20]; // Nome do monstro
    int hp; // HP do monstro
    int defesa; // Defesa acumulada do monstro
    TipoAcaoMonstro acao; // 1 para ataque, 2 para defesa
    int danoAtaqueMonstro; // Dano que o monstro vai causar ao jogador
    int defesaMonstro; // Defesa que o monstro vai ter
    int intencaoDefinida; // 1 se a intenção já foi definida para o turno atual, 0 caso contrário
    struct Monstro* proximo;
    struct Monstro* anterior;
} Monstro;

// Estrutura para representar a carta
typedef struct Carta 
{
    char nome[20]; // Nome da carta
    int energia; // Energia que a carta custa para ser jogada
    TipoAcao acao; // 1 para ataque, 2 para defesa, 3 para cura
    int quantidadeAcao; // Quantidade de dano/cura/escudo que a carta vai aplicar
    char descricao [100]; // Descrição da carta
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
Monstro* criarMonstro(char* nome, int hp, int defesa, int acao, int danoAtaqueMonstro, int defesaMonstro);
void definirIntencoesMonstros(Monstro* listaMonstros);
TipoAcaoMonstro acaoMonstro();
Carta* criarCarta(char* nome, int energia, int acao, int quantidadeAcao, char* descricao); 
Fase* criarFase(int nivel, char* descricao, Monstro* listaMonstros);
void iniciarJogo(Fase** faseAtual, Carta** cartas);
void jogarTurno(Fase** faseAtual, Carta** cartas); 
void finalizarTurno(Monstro* listaMonstros);
void mostrarInformacoesTurnoJogador(Carta** cartas); 
void mostrarInformacoesTurnoMonstros(Fase* faseAtual); 
void escolherEJogarCarta(Fase* faseAtual, Carta** cartas);
void jogarCarta(Carta* carta, Monstro* monstro, Fase* faseAtual);
void verificarMonstroVivo(Fase* faseAtual); 
void ordenarMonstrosPorHP(Monstro** listaMonstros);
int max(int a, int b);
int min(int a, int b);


// Variaveis globais
// Do jogo
int dificuldade = 2; // Salvar a escolha da dificuldade
int numTurno = 1; // Quantidade de turnos que passaram desde que o jogador começou o jogo
int turnoFinalizado = 0; // Se o jogador escolheu terminar o seu turno
int fimDeJogo = 0; // Se o jogo acabou Ou por HP do jogador == 0 ou Jogador ganhou o jogo!

// Atributos do Jogador
int hpJogador = 10; // Salvar o hp do jogador
int energiaJogador = 3; // Salvar a energia do jogador
int defesaJogador = 0; // Salvar a defesa acumulada das cartas que o jogador jogou

int main() 
{
    srand(time(NULL)); // Inicializa a semente do gerador de números aleatórios

    // Criando 3 monstros para fase 1
    Monstro* monstro1 = criarMonstro("Goblin Guerreiro", 3, 0, DEFAULTMONSTRO, 2, 1);
    Monstro* monstro2 = criarMonstro("Goblin Arqueiro", 2, 0, DEFAULTMONSTRO, 2, 1);
    Monstro* monstro3 = criarMonstro("Orc Guerreiro", 4, 0, DEFAULTMONSTRO, 3, 2);

    // Criando 3 monstros para fase 2
    Monstro* monstro4 = criarMonstro("Hobgoblin", 4, 0, DEFAULTMONSTRO, 3, 2);
    Monstro* monstro5 = criarMonstro("Elfo Mago", 3, 0, DEFAULTMONSTRO, 3, 1);
    Monstro* monstro6 = criarMonstro("Succubus", 4, 0, DEFAULTMONSTRO, 4, 2);

    // Criando 2 monstros e o Boss para fase 3
    Monstro* monstro7 = criarMonstro("Succubus", 4, 0, DEFAULTMONSTRO, 4, 2);
    Monstro* monstro8 = criarMonstro("Dragao", 5, 0, DEFAULTMONSTRO, 5, 3);
    Monstro* monstro9 = criarMonstro("Rei Demonio", 10, 0, DEFAULTMONSTRO, 6, 4);

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

    // Criando 6 cartas
    Carta* cartas[6];
    cartas[0] = criarCarta("Espada", 1, ATAQUESING, 2, "Uma espada brilhante que aplica 2 de dano a um inimigo");
    cartas[1] = criarCarta("Escudo", 1, DEFESA, 3, "Um escudo resistente que bloqueia 3 de dano");
    cartas[2] = criarCarta("Pocao de Cura", 3, 2,CURA, "Uma pocao que restaura 2 ao seu HP");
    cartas[3] = criarCarta("Arco", 2, ATAQUEMULT, 1, "Um arco para ataques a distancia que aplica 1 de dano a todos os inimigos");
    cartas[4] = criarCarta("Flecha Envenenada", 2, 3, ATAQUESING, "Uma flecha letal que aplica 3 de dano a um inimigo");
    cartas[5] = criarCarta("Magia de Fogo", 3, 1, ATAQUEMULT, "Uma poderosa bola de fogo que aplica 1 de dano a todos os inimigos");

    // Criando uma fase
    Fase* fase1 = criarFase(1, "A caverna escura", monstro1);
    Fase* fase2 = criarFase(2, "O calabouco da castelo", monstro4);
    Fase* fase3 = criarFase(3, "Castelo do Rei Demonio", monstro7);

    // Conectando as fases em uma lista simplesmente encadeada
    fase1->proxima = fase2;
    fase2->proxima = fase3;
    fase3->proxima = NULL;

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
        //printf("\033[1;44mThis text has a blue background\033[0m\n");
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
                iniciarJogo(&faseAtual, cartas);
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
    printf("Voce eh um(a) guerreiro(a) em ascencao e foi designado(a) para proteger o reino contra o Rei Demonio e suas tropas.\n"
            "O conselheiro do rei escalou voce para ir ate ao castelo, chegar ao topo e destruir todo o mal.\n"
            "O jogo eh dividido em 3 fases contendo, cada uma, 3 monstros.\n"
            "Os monstros podem escolher lhe atacar ou se defender de seus ataques.\n"
            "Voce vai utilizar suas cartas para destruir os monstros, se defender ou recuperar seu HP.\n"
            "Mas cuidado! Voce apenas possui 3 de energia por turno e suas cartas custam energia!\n"
            "Porem, tambem nao se preocupe que ao encerrar o turno, a sua energia sera restaurada totalmente.\n"
            "Voce deve concluir as 3 fases na menor quantidade de turnos possiveis. Por isso, pense com cuidado em suas acoes.\n"
            "Boa sorte, guerreiro(a)!\n"
            "================================== / /  / / ==================================\n");

}

// Criação do Monstro
Monstro* criarMonstro(char* nome, int hp, int defesa, int acao, int danoAtaqueMonstro, int defesaMonstro) 
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
    novoMonstro->danoAtaqueMonstro = danoAtaqueMonstro;
    novoMonstro->defesaMonstro = defesaMonstro;

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
        return ATAQUEMONSTRO;
    } 
    else 
    {
        return DEFESAMONSTRO;
    }
}

// Criação da carta
Carta* criarCarta(char* nome, int energia, int acao, int quantidadeAcao, char* descricao) 
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
    novaCarta->quantidadeAcao = quantidadeAcao;
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

void iniciarJogo(Fase** faseAtual, Carta** cartas) 
{
    clearScreen();
    while(fimDeJogo == 0 && hpJogador > 0)
    {
        jogarTurno(faseAtual, cartas);
    }

    if(hpJogador <= 0)
    {
        printf("Voce morreu!");
        exit(0);
    }
    
}

void jogarTurno(Fase** faseAtual, Carta** cartas) 
{
    while(turnoFinalizado == 0)
    {
        verificarMonstroVivo(*faseAtual);
        ordenarMonstrosPorHP(&((*faseAtual)->monstros));
        definirIntencoesMonstros((*faseAtual)->monstros);
        printf("Fase Atual: %d\n", (*faseAtual)->nivelFase);
        mostrarInformacoesTurnoJogador(cartas);
        mostrarInformacoesTurnoMonstros(*faseAtual);
        escolherEJogarCarta(*faseAtual, cartas);
        if((*faseAtual)->monstros == NULL)
        {
            *faseAtual = (*faseAtual)->proxima;
        }
    }
    finalizarTurno((*faseAtual)->monstros);
}

void finalizarTurno(Monstro* listaMonstros) 
{
    Monstro* monstroAtual = listaMonstros;
    while (monstroAtual != NULL) 
    {
        monstroAtual->defesa = 0; // Resetar a defesa do monstro para o próximo turno
        if(monstroAtual->acao == ATAQUEMONSTRO)
        {
            if (defesaJogador > 0) 
            {
                int danoAplicado = min(defesaJogador, monstroAtual->danoAtaqueMonstro);
                defesaJogador -= danoAplicado; // Reduzir a defesa do jogador
                if (monstroAtual->danoAtaqueMonstro > danoAplicado) 
                {
                    hpJogador -= (monstroAtual->danoAtaqueMonstro - danoAplicado); // Aplicar o dano restante ao HP
                }
            } 
            else 
            {
                hpJogador -= monstroAtual->danoAtaqueMonstro;
            }
        }
        else
        {
            monstroAtual->defesa += monstroAtual->defesaMonstro;
        }

        monstroAtual->intencaoDefinida = 0;
        monstroAtual = monstroAtual->proximo;
    }

    energiaJogador = 3;
    defesaJogador = 0; // Zerar a defesa do jogador para o próximo turno
    numTurno++;
    turnoFinalizado = 0;
    clearScreen();
}

// Mostra as informações do jogador no turno atual
void mostrarInformacoesTurnoJogador(Carta** cartas) 
{
    printf("Turno Atual: %d\n", numTurno);
    printf("HP do Jogador: %d\n", hpJogador);
    printf("Energia: %d\n", energiaJogador);
    printf("Escudos: %d\n", defesaJogador);

    printf("================================== / /  / / ==================================\n");
    
    printf("Suas Cartas:\n");
    for (int i = 0; i < 6; i++) 
    {
        printf("[%d]Nome: %s, Energia: %d, Descricao: %s\n", 
        i+1,
        cartas[i]->nome, 
        cartas[i]->energia, 
        cartas[i]->descricao);
    }
    printf("================================== / /  / / ==================================\n");
}

// Mostra as informações dos monstros no turno atual
void mostrarInformacoesTurnoMonstros(Fase* faseAtual)
{
    printf("Monstros:\n");
    Monstro* monstroAtual = faseAtual->monstros;
    while (monstroAtual != NULL) 
    {
        printf("Nome: %s, |HP: %d| |Escudos: %d|\n", monstroAtual->nome, monstroAtual->hp, monstroAtual->defesa);
        if(faseAtual->nivelFase == 1)
        {
            if(monstroAtual->acao == 1)
            {
                printf("%s, vai atacar com %d de dano!\n", monstroAtual->nome, monstroAtual->danoAtaqueMonstro);
            }
            else
            {
                printf("%s, vai defender com %d de defesa!\n", monstroAtual->nome, monstroAtual->defesaMonstro);
            }
        }
        else if(faseAtual->nivelFase == 2)
        {
            if(monstroAtual->acao == 1)
            {
                printf("%s, vai atacar com %d de dano!\n", monstroAtual->nome, monstroAtual->danoAtaqueMonstro);
            }
            else
            {
                printf("%s, vai defender com %d de defesa!\n", monstroAtual->nome, monstroAtual->defesaMonstro);
            }
        }
        else
        {
            if(monstroAtual->acao == 1)
            {
                printf("%s, vai atacar com %d de dano!\n", monstroAtual->nome, monstroAtual->danoAtaqueMonstro);
            }
            else
            {
                printf("%s, vai defender com %d de defesa!\n", monstroAtual->nome, monstroAtual->defesaMonstro);
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

    printf("================================== Consequencias das suas acoes ==================================\n");
    
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
    jogarCarta(cartaEscolhida, faseAtual->monstros, faseAtual);
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
            if (monstro->defesa > 0) 
            {
                int danoRestante = carta->quantidadeAcao - monstro->defesa;
                monstro->defesa = max(0, monstro->defesa - carta->quantidadeAcao);
                monstro->hp = max(monstro->hp, monstro->hp - danoRestante);
            } 
            else 
            {
                monstro->hp -= carta->quantidadeAcao;
            }
            printf("Voce causou %d de dano ao monstro %s!\n", carta->quantidadeAcao, monstro->nome);
            break;
            case ATAQUEMULT:
                while (monstroAtual != NULL)
                {
                    if (monstro->defesa > 0) 
                    {
                        int danoRestante = carta->quantidadeAcao - monstro->defesa;
                        monstro->defesa = max(0, monstro->defesa - carta->quantidadeAcao);
                        monstro->hp = max(monstro->hp, monstro->hp - danoRestante);
                    } 
                    else 
                    {
                        monstro->hp -= carta->quantidadeAcao;
                        printf("Voce causou %d de dano ao monstro %s!\n", carta->quantidadeAcao, monstro->nome);
                    }
                    monstroAtual = monstroAtual->proximo;
                }
                free(monstroAtual);
                printf("================================== / /  / / ==================================\n");
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

void verificarMonstroVivo(Fase* faseAtual) 
{
    Monstro* monstroAtual = faseAtual->monstros;
    Monstro* monstroAnterior = NULL;
    
    while(monstroAtual != NULL) 
    {
        if(monstroAtual->hp <= 0) 
        {
            if(monstroAnterior != NULL) 
            {
                monstroAnterior->proximo = monstroAtual->proximo;
            } 
            else 
            {
                faseAtual->monstros = monstroAtual->proximo;
            }
            Monstro* monstroParaLiberar = monstroAtual;
            monstroAtual = monstroAtual->proximo;
            free(monstroParaLiberar);
        } 
        else 
        {
            monstroAnterior = monstroAtual;
            monstroAtual = monstroAtual->proximo;
        }
    }
}

void ordenarMonstrosPorHP(Monstro** listaMonstros) 
{
    Monstro *sorted = NULL;
    Monstro *current = *listaMonstros;
    while (current != NULL) {
        Monstro *next = current->proximo;

        // Localiza onde inserir o nodo atual na lista ordenada
        if (sorted == NULL || sorted->hp >= current->hp) 
        {
            current->proximo = sorted;
            sorted = current;
        } 
        else 
        {
            Monstro *currSorted = sorted;
            while (currSorted->proximo != NULL && currSorted->proximo->hp < current->hp) 
            {
                currSorted = currSorted->proximo;
            }
            current->proximo = currSorted->proximo;
            currSorted->proximo = current;
        }

        // Atualiza o nodo atual
        current = next;
    }

    *listaMonstros = sorted;
}


int max(int a, int b) 
{
    return (a > b) ? a : b;
}

int min(int a, int b) 
{
    return (a < b) ? a : b;
}

// O que falta fazer:
    // Ordenação dos monstros com o menor HP ficar sempre na frente [OK]
    // Comunicação entre a ação da carta e os monstros [OK]
        // Verificação se existe escudo no monstro antes do player bater [OK]
    // Comunicação entre a ação dos monstros e o player[OK]
        // Verificação se existe escudo no player antes do monstro bater [OK]
    // Verificação de termino de fase (Se todos os monstros da fase morreram) [OK]
    // Troca de fase após a verificação [OK]
    // Condição do player ganhar o jogo
    // Encerrar o programa quando o HP do player chegar a 0 [OK]
    // Definir e construir o boss [OK]
    // EXTRA --> Criar o arquivo de high score que vai ser a partir da quantidad ede turnos que foram necessarios para acaber o jogo

// O que eu acho que precisa ser mudado / Adicionado
// Adicionar mais um campo nas cartas para poder ter um valor de dano/cura/escudo para poder aplicar nos monstros
// Adicionar mais um campo nos monstros para poder ter um valor de dano para cada monstro para poder aplicar no player e diferenciar seus ataques