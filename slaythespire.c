#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Enum para cada Classe que o jogador pode escolher
typedef enum {
    BARALHO_GUERREIRO = 1,
    BARALHO_MAGO = 2,
    BARALHO_ARQUEIRO = 3
} TipoBaralho;

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
    int defesaParaAdicionarMonstro; // Defesa que o monstro vai ter
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

// Estrutura para representar o highscore
typedef struct {
    char nomeJogador[50];
    int faseAlcancada;
    int numTurnos;
    int hpRestante;
} HighScore;

// Protótipos das funções
void obterNomeJogador(char *nome, int tamanhoMaximo);
void clearScreen();
void printCharByChar(const char *str, useconds_t delay);
int mostrarIntro(int opcao);
void mostrarMenuPrincipal(Fase* faseAtual);
void escolherDificuldade();
void instrucoes();
Monstro* criarMonstro(char* nome, int hp, int defesa, int acao, int danoAtaqueMonstro, int defesaParaAdicionarMonstro);
void definirIntencoesMonstros(Monstro* listaMonstros);
TipoAcaoMonstro acaoMonstro();
void resetarDefesaMonstros(Monstro* listaMonstros);
Carta* criarCarta(char* nome, int energia, int acao, int quantidadeAcao, char* descricao); 
int escolherClasses();
Carta** criarBaralho(TipoBaralho tipoBaralho);
void liberarBaralho(Carta** cartas, int numCartas);
Fase* criarFase(int nivel, char* descricao, Monstro* listaMonstros);
int iniciarJogo(Fase** faseAtual, Carta** cartas);
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
Fase* criarTodasFases();
void liberarMonstros(Monstro* monstro);
void liberarFases(Fase* fase);
void resetarEstadoJogo();
void salvarHighScoresArquivo(const HighScore* scores, int numScores);
void exibirHighScores();
int compararHighScores(const void* a, const void* b);
void ordenarHighScores(HighScore* scores, int numScores);
void registrarHighScore(const char* nomeJogador, int faseAlcancada, int numTurnos, int hpRestante);

// Variaveis globais
// Do jogo
int dificuldade = 2; // Salvar a escolha da dificuldade
int numTurno = 1; // Quantidade de turnos que passaram desde que o jogador começou o jogo
int turnoFinalizado = 0; // Se o jogador escolheu terminar o seu turno
int fimDeJogo = 0; // Se o jogo acabou Ou por HP do jogador == 0 ou Jogador ganhou o jogo!

// Atributos do Jogador
char nomeJogador[30]; // Salvar o nome do jogador
int hpJogador = 10; // Salvar o hp do jogador
int energiaJogador = 3; // Salvar a energia do jogador
int defesaJogador = 0; // Salvar a defesa acumulada das cartas que o jogador jogou

int main() 
{
    srand(time(NULL)); // Inicializa a semente do gerador de números aleatórios

    int opcao = 0;
    //opcao = mostrarIntro(opcao);

    Fase* faseInicial = criarTodasFases(); // Criando as fases e monstros

    mostrarMenuPrincipal(faseInicial);

    liberarFases(faseInicial);
    return 0;
}

// Obtem o nome do jogador
void obterNomeJogador(char *nome, int tamanhoMaximo) 
{
    printf("Qual o seu nome, heroi(na)?  ");
    fgets(nome, tamanhoMaximo, stdin);

    // Remover a nova linha (se houver) do final da string
    size_t length = strlen(nome);
    if (length > 0 && nome[length - 1] == '\n') 
    {
        nome[length - 1] = '\0';
    }
}

// Limpa a tela
void clearScreen() 
{
    printf("\033[H\033[J");
}

// Imprime um texto letra por letra com um delay entre os chars
void printCharByChar(const char *str, useconds_t delay) 
{
    for (int i = 0; str[i] != '\0'; i++) 
    {
        printf("%c", str[i]);
        fflush(stdout);
        usleep(delay);
    }
}

// Mostra a introdução do jogo
int mostrarIntro(int opcao) 
{
    clearScreen();
    printf("\033[1;44m==================================================================== / /  / / ====================================================================\033[0m\n");
    const char *intro = "Em uma era de lendas e mitos, ergue-se uma figura audaz e valente: voce, um(a) aventureiro(a) em ascensao, cujo nome ecoa pelas terras como\n"
                        "uma promessa de esperanca. O destino do reino, agora abalado pelas sombras do mal, repousa em seus ombros. O Rei Demonio, um ser de poder\n"
                        "inimaginavel e crueldade desmedida, lancou seu veu sombrio sobre a terra, ameacando engolir tudo em um abismo de desespero.\n\n"
                        "Em meio a tempos tao sombrios, o conselheiro do rei, um sabio de visao agucada e coracao puro, ve em voce a chama da salvacao. Ele o(a)\n"
                        "escolhe, dentre tantos, para uma missao que decidira o destino de todos: infiltrar-se no castelo corrompido, um labirinto de perigos\n"
                        "e horrores, e ascender ao topo, onde o mal reina supremo.\n\n"
                        "Com o peso da responsabilidade e a luz da esperanca em seu coracao, voce parte em sua jornada. O caminho eh tortuoso, repleto de adversarios\n"
                        "formidaveis e desafios que testarao sua forca, sua coragem e sua determinacao. A cada passo, a escuridao tenta lhe engolir, mas sua vontade\n"
                        "eh inquebrantavel.\n\n"
                        "Voce eh mais do que um(a) guerreiro(a); voce eh a ultima esperanca de um mundo a beira da ruina. A lenda de suas facanhas sera cantada\n"
                        "por geracoes, um farol de inspiracao para todos aqueles que acreditam que, mesmo na mais escura das noites, a luz da coragem brilha eternamente.\n";
    printCharByChar(intro, 32000); // Imprime o texto letra por letra com um delay de 32 milissegundos entre os chars
    printf("\033[1;44m==================================================================== / /  / / ====================================================================\033[0m\n");
    printf("Precione [1] para jogar.\n");
    scanf("%d", &opcao);
    return opcao;
}



void mostrarMenuPrincipal(Fase* faseAtual) 
{
    clearScreen();
    int opcao;
    do 
    {
        printf("Menu Principal\n");
        printf("1. Jogar\n");
        printf("2. Escolher Dificuldade (Atual: %d)\n", dificuldade);
        printf("3. Como jogar?\n");
        printf("4. High Scores\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        while (getchar() != '\n'); // Limpar o buffer do teclado

        switch(opcao) 
        {
            case 1:
                obterNomeJogador(nomeJogador, sizeof(nomeJogador)); // Obter o nome do jogador
                Carta** cartas = criarBaralho(escolherClasses()); // Criar o baralho do jogador
                if (iniciarJogo(&faseAtual, cartas)) // Se o jogador escolher reiniciar quando ele morrer ou ganhar o jogo
                {
                    obterNomeJogador(nomeJogador, sizeof(nomeJogador)); // Obter o nome do jogador quando o jogo reiniciar
                    resetarEstadoJogo(); // Reseta o estado do jogo
                    liberarFases(faseAtual); // Libera a fase atual e monstros antes de reiniciar
                    liberarBaralho(cartas, 6); // Libera o baralho antes de reiniciar
                    faseAtual = criarTodasFases(); // Cria novamente as fases e monstros
                    mostrarMenuPrincipal(faseAtual); // Mostra o menu principal novamente
                }
                break;
            case 2:
                escolherDificuldade();
                break;
            case 3:
                instrucoes();
                break;
            case 4:
                clearScreen();
                exibirHighScores();
                printf("\033[1;44m========================== / /  / / ==========================\033[0m\n");
    
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida! Por favor, escolha novamente.\n");
        }
    } while(opcao != 5);
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
    printf( "O jogo eh dividido em 3 fases contendo, cada uma, 3 monstros.\n"
            "Os monstros podem escolher lhe atacar ou se defender de seus ataques.\n"
            "Voce vai utilizar suas cartas para destruir os monstros, se defender ou recuperar seu HP.\n"
            "Mas cuidado! Voce apenas possui 3 de energia por turno e suas cartas custam energia!\n"
            "Porem, tambem nao se preocupe que ao encerrar o turno, a sua energia sera restaurada totalmente.\n"
            "Voce deve concluir as 3 fases na menor quantidade de turnos possiveis. Por isso, pense com cuidado em suas acoes.\n"
            "Boa sorte, guerreiro(a)!\n"
            "================================== / /  / / ==================================\n");

}

// Criação do Monstro
Monstro* criarMonstro(char* nome, int hp, int defesa, int acao, int danoAtaqueMonstro, int defesaParaAdicionarMonstro) 
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
    novoMonstro->defesaParaAdicionarMonstro = defesaParaAdicionarMonstro;

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

void resetarDefesaMonstros(Monstro* listaMonstros) 
{
    Monstro* monstroAtual = listaMonstros;
    while (monstroAtual != NULL) 
    {
        monstroAtual->defesa = 0;
        monstroAtual = monstroAtual->proximo;
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

int escolherClasses()
{
    int opcao;
    printf("Escolha uma classe:\n");
    printf("1. Guerreiro\n");
    printf("2. Mago\n");
    printf("3. Arqueiro\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);
    return opcao;
}

// Criação do Baralho a partir da classe escolhida
Carta** criarBaralho(TipoBaralho tipoBaralho) 
{
    Carta** cartas = malloc(sizeof(Carta*) * 6); // Assumindo 6 cartas por baralho
    if (!cartas) {
        perror("Falha ao alocar memória para o baralho");
        exit(1);
    }

    switch (tipoBaralho) 
    {
        case BARALHO_GUERREIRO:
            // Nome, Energia, Acao, QuantidadeAcao, Descricao
            cartas[0] = criarCarta("Espada Fulminante", 1, ATAQUESING, 2, "Colocar Descrição"); 
            cartas[1] = criarCarta("Barreira de Escudos", 1, DEFESA, 3, "Colocar Descrição");
            cartas[2] = criarCarta("Pocao de Cura", 3, CURA, 2, "Uma pocao que restaura 2 ao seu HP");
            cartas[3] = criarCarta("Espada Bumerangue", 2, ATAQUEMULT, 1, "Colocar Descrição");
            cartas[4] = criarCarta("Perfurar", 2, ATAQUESING, 3, "Colocar Descrição");
            cartas[5] = criarCarta("Impacto Explosivo", 3, ATAQUEMULT, 1, "Colocar Descrição");
            break;
        case BARALHO_MAGO:
            // Nome, Energia, Acao, QuantidadeAcao, Descricao
            cartas[0] = criarCarta("Lança de Fogo", 1, ATAQUESING, 2, "Colocar Descrição"); 
            cartas[1] = criarCarta("Barreira de Gelo", 1, DEFESA, 3, "Colocar Descrição");
            cartas[2] = criarCarta("Toque Divino", 3, CURA, 2, "Colocar Descrição");
            cartas[3] = criarCarta("Nevasca", 2, ATAQUEMULT, 1, "Colocar Descrição");
            cartas[4] = criarCarta("Trovao de Jupiter", 2, ATAQUESING, 3, "Colocar Descrição");
            cartas[5] = criarCarta("Chuva de Meteoros", 3, ATAQUEMULT, 1, "Colocar Descrição");
            break;
        case BARALHO_ARQUEIRO:
            // Nome, Energia, Acao, QuantidadeAcao, Descricao
            cartas[0] = criarCarta("Tiro Preciso", 1, ATAQUESING, 2, "Colocar Descrição"); 
            cartas[1] = criarCarta("Manto das Sombras", 1, DEFESA, 3, "Um manto encantado que envolve o arqueiro nas sombras, dando 3 Escudo");
            cartas[2] = criarCarta("Poção de Cura", 3, CURA, 2, "Colocar Descrição");
            cartas[3] = criarCarta("Chuva de Flechas", 2, ATAQUEMULT, 1, "Colocar Descrição");
            cartas[4] = criarCarta("Flecha perfurante", 2, ATAQUESING, 3, "Colocar Descrição");
            cartas[5] = criarCarta("Rajada Tripla de Flechas", 3, ATAQUEMULT, 1, "Colocar Descrição");
            break;
        default:
            fprintf(stderr, "Tipo de baralho desconhecido\n");
            free(cartas);
            exit(1);
    }

    return cartas;
}

void liberarBaralho(Carta** cartas, int numCartas) 
{
    for (int i = 0; i < numCartas; i++) 
    {
        free(cartas[i]); 
    }
    free(cartas);
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

// Retorna 1 se o jogador escolher reiniciar, 0 caso contrário
int iniciarJogo(Fase** faseAtual, Carta** cartas) 
{
    clearScreen();
    while(fimDeJogo == 0 && hpJogador > 0)
    {
        jogarTurno(faseAtual, cartas);
    }

    if(hpJogador <= 0)
    {
        printf("Voce morreu!\n");
    }

    if(fimDeJogo == 1)
    {
        printf("Parabens! Voce derrotou o Rei Demonio e salvou o reino!\n");
        printf("Voce precisou de %d turnos para concluir o jogo!\n", numTurno);
    }
    
    registrarHighScore(nomeJogador, (*faseAtual)->nivelFase, numTurno, hpJogador); // Registrar o highscore do jogador
    
    printf("Deseja jogar novamente? [1]SIM ou [2]NAO: ");
    int escolha;
    scanf("%d", &escolha);
    return escolha;
}


void jogarTurno(Fase** faseAtual, Carta** cartas) 
{
    while(turnoFinalizado == 0)
    {
        verificarMonstroVivo(*faseAtual); // Verificar se algum monstro morreu
        ordenarMonstrosPorHP(&((*faseAtual)->monstros)); // Ordenar os monstros por HP
        definirIntencoesMonstros((*faseAtual)->monstros); // Definir as intenções dos monstros
        printf("Fase Atual: %d\n", (*faseAtual)->nivelFase); // Mostrar a fase atual
        mostrarInformacoesTurnoJogador(cartas); // Mostrar as informações do jogador
        mostrarInformacoesTurnoMonstros(*faseAtual); // Mostrar as informações dos monstros
        escolherEJogarCarta(*faseAtual, cartas); // Escolher e jogar uma carta

        if((*faseAtual)->monstros == NULL)
        {
            if((*faseAtual)->nivelFase == 3)
            {
                fimDeJogo = 1; // Marcar como fim de jogo se todos os monstros da última fase foram derrotados
                break; // Sair do loop do turno
            }
            else
            {
                *faseAtual = (*faseAtual)->proxima; // Avançar para a próxima fase se todos os monstros morreram
                resetarDefesaMonstros((*faseAtual)->monstros); // Resetar a defesa dos monstros para a próxima fase
            }
        }
    }
    
    finalizarTurno((*faseAtual)->monstros); // Finalizar o turno
    
    if((*faseAtual)->nivelFase == 3 && (*faseAtual)->monstros == NULL)
    {
        fimDeJogo = 1;
    }

}

void finalizarTurno(Monstro* listaMonstros) 
{
    Monstro* monstroAtual = listaMonstros;
    while (monstroAtual != NULL) 
    {
        monstroAtual->defesa = 0; // Resetar a defesa do monstro para o próximo turno
        if(monstroAtual->acao == ATAQUEMONSTRO) // Se o monstro escolheu atacar
        {
            if (defesaJogador > 0) // Verificar se o jogador tem escudo para se defender
            {
                int danoAplicado = min(defesaJogador, monstroAtual->danoAtaqueMonstro); // Verificar se a defesa do jogador é maior que o dano do monstro
                defesaJogador -= danoAplicado; // Reduzir a defesa do jogador
                if (monstroAtual->danoAtaqueMonstro > danoAplicado) // Verificar se o dano do monstro é maior que a defesa do jogador
                {
                    hpJogador -= (monstroAtual->danoAtaqueMonstro - danoAplicado); // Aplicar o dano restante ao HP
                }
            } 
            else 
            {
                hpJogador -= monstroAtual->danoAtaqueMonstro; // Aplicar o dano ao HP
            }
        }
        else if(monstroAtual->acao == DEFESAMONSTRO)
        {
            monstroAtual->defesa = monstroAtual->defesaParaAdicionarMonstro; // Adicionar a defesa do monstro
        }

        monstroAtual->intencaoDefinida = 0; // Resetar a variável de controle para o próximo turno
        monstroAtual->acao = DEFAULTMONSTRO; // Resetar a ação do monstro para o próximo turno
        monstroAtual = monstroAtual->proximo; // Avançar para o próximo monstro
    }

    energiaJogador = 3; // Restaurar a energia do jogador ao finalizar o turno
    defesaJogador = 0; // Zerar a defesa do jogador para o próximo turno
    numTurno++; // Incrementar o número do turno
    turnoFinalizado = 0; // Resetar a variável de controle para o próximo turno
    clearScreen(); // Limpar a tela
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
        if(monstroAtual->acao == 1)
        {
            printf("%s, vai atacar com %d de dano!\n", monstroAtual->nome, monstroAtual->danoAtaqueMonstro);
        }
        else
        {
            printf("%s, vai defender com %d de defesa!\n", monstroAtual->nome, monstroAtual->defesaParaAdicionarMonstro);
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
                    if (monstroAtual->defesa > 0) 
                    {
                        int danoRestante = carta->quantidadeAcao - monstroAtual->defesa;
                        monstroAtual->defesa = max(0, monstroAtual->defesa - carta->quantidadeAcao);
                        monstroAtual->hp = max(monstroAtual->hp, monstroAtual->hp - danoRestante);
                    } 
                    else 
                    {
                        monstroAtual->hp -= carta->quantidadeAcao;
                        printf("Voce causou %d de dano ao monstro %s!\n", carta->quantidadeAcao, monstroAtual->nome);
                    }
                    monstroAtual = monstroAtual->proximo;
                }
                free(monstroAtual);
                printf("================================== / /  / / ==================================\n");
                break;
            case DEFESA:
                defesaJogador += carta->quantidadeAcao;
                break;
            case CURA:
                hpJogador += carta->quantidadeAcao;
                break;
            default:
                printf("Acao desconhecida!\n");
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

Fase* criarTodasFases() 
{
    // Criando 3 monstros para fase 1
    Monstro* monstro1 = criarMonstro("Goblin Guerreiro", 1, 0, DEFAULTMONSTRO, 2, 1); 
    Monstro* monstro2 = criarMonstro("Goblin Arqueiro", 1, 0, DEFAULTMONSTRO, 2, 1); 
    Monstro* monstro3 = criarMonstro("Orc Guerreiro", 1, 0, DEFAULTMONSTRO, 3, 2); 

    // Criando 3 monstros para fase 2
    Monstro* monstro4 = criarMonstro("Hobgoblin", 1, 0, DEFAULTMONSTRO, 3, 2); 
    Monstro* monstro5 = criarMonstro("Elfo Mago", 1, 0, DEFAULTMONSTRO, 3, 1); 
    Monstro* monstro6 = criarMonstro("Succubus", 1, 0, DEFAULTMONSTRO, 4, 2); 

    // Criando 2 monstros e o Boss para fase 3
    Monstro* monstro7 = criarMonstro("Succubus", 1, 0, DEFAULTMONSTRO, 4, 2); 
    Monstro* monstro8 = criarMonstro("Dragao", 1, 0, DEFAULTMONSTRO, 5, 3); 
    Monstro* monstro9 = criarMonstro("Rei Demonio", 1, 0, DEFAULTMONSTRO, 6, 4); 

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
void liberarMonstros(Monstro* monstro) 
{
    while (monstro != NULL) 
    {
        Monstro* proximo = monstro->proximo;
        free(monstro);
        monstro = proximo;
    }
}

void liberarFases(Fase* fase) 
{
    while (fase != NULL) 
    {
        Fase* proxima = fase->proxima;
        
        liberarMonstros(fase->monstros); // Libera os monstros da fase atual
        free(fase); // Libera a memória da fase

        fase = proxima;
    }
}

void resetarEstadoJogo() 
{
    hpJogador = 10; // Valor inicial do HP do jogador
    energiaJogador = 3; // Valor inicial da energia
    numTurno = 1; // Resetar o contador de turnos
    fimDeJogo = 0; // Resetar a condição de fim de jogo
    turnoFinalizado = 0; // Resetar a variável de controle do turno
}

void salvarHighScoresArquivo(const HighScore* scores, int numScores)
{
    FILE* arquivo = fopen("highscores.txt", "a"); // Modo de anexação
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo de high scores");
        return;
    }

    for (int i = 0; i < numScores; i++) {
        fprintf(arquivo, "%s - Fase: %d, Turnos: %d, HP: %d\n",
                scores[i].nomeJogador, scores[i].faseAlcancada, scores[i].numTurnos, scores[i].hpRestante);
    }

    fclose(arquivo);
}


void exibirHighScores() 
{
    HighScore scores[100]; // Ajuste o tamanho conforme necessário
    int numScores = 0;

    // Carregar os scores do arquivo
    FILE* arquivo = fopen("highscores.txt", "r");
    if (arquivo == NULL) 
    {
        perror("Erro ao abrir arquivo de high scores");
        return;
    }

    while (fscanf(arquivo, "%49s - Fase: %d, Turnos: %d, HP: %d\n", 
                scores[numScores].nomeJogador, 
                &scores[numScores].faseAlcancada, 
                &scores[numScores].numTurnos, 
                &scores[numScores].hpRestante) == 4 && numScores < 100) 
                {
        numScores++;
    }

    fclose(arquivo);

    // Ordenar os scores
    qsort(scores, numScores, sizeof(HighScore), compararHighScores);

    // Exibir os scores
    printf("High Scores:\n");
    for (int i = 0; i < numScores; i++) 
    {
        printf("%d. %s - Fase: %d, Turnos: %d, HP: %d\n",
               i + 1, scores[i].nomeJogador, scores[i].faseAlcancada, scores[i].numTurnos, scores[i].hpRestante);
    }
}


int compararHighScores(const void* a, const void* b) 
{
    const HighScore* scoreA = (const HighScore*)a;
    const HighScore* scoreB = (const HighScore*)b;

    // Ordenar por maior fase alcançada
    if (scoreA->faseAlcancada != scoreB->faseAlcancada) 
    {
        return scoreB->faseAlcancada - scoreA->faseAlcancada;
    }
    // Ordenar por menor quantidade de turnos
    else if (scoreA->numTurnos != scoreB->numTurnos) 
    {
        return scoreA->numTurnos - scoreB->numTurnos;
    }
    // Ordenar por maior restante HP
    else 
    {
        return scoreB->hpRestante - scoreA->hpRestante;
    }
}


void ordenarHighScores(HighScore* scores, int numScores) 
{
    qsort(scores, numScores, sizeof(HighScore), compararHighScores);
}

void registrarHighScore(const char* nomeJogador, int faseAlcancada, int numTurnos, int hpRestante) 
{
    HighScore score;
    strcpy(score.nomeJogador, nomeJogador);
    score.faseAlcancada = faseAlcancada;
    score.numTurnos = numTurnos;
    score.hpRestante = hpRestante;

    salvarHighScoresArquivo(&score, 1);
}


// O que falta fazer:
    // EXTRA --> Deixar o jogo mais bonito
    // EXTRA --> Criar o arquivo de high score que vai ser a partir da quantidad ede turnos que foram necessarios para acaber o jogo
        // Criar a função de salvar o high score
    // EXTRA --> Criar um menu para mostrar o high score