#include <stdio.h> // Biblioteca padrao de entrada e saida
#include <stdlib.h> // Biblioteca padrao do C
#include <string.h> // Biblioteca para manipulaçao de strings
#include <time.h> // Biblioteca para manipulaçao de tempo
#include <unistd.h> // Biblioteca para usar a funçao usleep

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
    ATAQUESING = 1,
    ATAQUEMULT = 2,
    DEFESA = 3,
    CURA = 4,
} TipoAcao;

//Enum para cada tipo de açao dos monstros
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
    int intencaoDefinida; // 1 se a intençao ja foi definida para o turno atual, 0 caso contrario
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
    char descricao [200]; // Descriçao da carta
    struct Carta* proximo;
    struct Carta* anterior;
} Carta;

// Estrutura para representar uma fase
typedef struct Fase 
{
    int nivelFase; // Nivel da fase
    Monstro* monstros; // Lista duplamente encadeada de monstros da fase
    char descricao[100]; // Descriçao da fase
    struct Fase* proxima;
} Fase;

// Estrutura para representar o highscore
typedef struct HighScore
{
    char nomeJogador[50];
    int faseAlcancada;
    int numTurnos;
    int hpRestante;
} HighScore;

// Prototipos das funções
void obterNomeJogador(char *nome, int tamanhoMaximo);
void clearScreen();
void printCharByChar(const char *str, useconds_t delay);
int mostrarIntro(int opcao);
void mostrarMenuPrincipal(Fase* faseAtual);
void escolherDificuldade(Fase** faseAtual);
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
Fase* criarTodasFases(int dificuldade);
void liberarMonstros(Monstro* monstro);
void liberarFases(Fase* fase);
void resetarEstadoJogo();
void salvarHighScoresArquivo(const HighScore* scores, int numScores);
void exibirHighScores();
int compararHighScores(const void* a, const void* b);
void ordenarHighScores(HighScore* scores, int numScores);
void registrarHighScore(const char* nomeJogador, int faseAlcancada, int numTurnos, int hpRestante);
void telaVitoria();
void telaDerrota();

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
    srand(time(NULL)); // Inicializa a semente do gerador de numeros aleatorios

    int opcao = 0;
    opcao = mostrarIntro(opcao);

    Fase* faseInicial = criarTodasFases(dificuldade); // Criando as fases e monstros

    mostrarMenuPrincipal(faseInicial);

    liberarFases(faseInicial);
    return 0;
}

// Obtem o nome do jogador
void obterNomeJogador(char *nome, int tamanhoMaximo) 
{
    printf("Qual o seu nome, heroi(na)?  ");
    fgets(nome, tamanhoMaximo, stdin);

    // Remove a nova linha (se tiver) do final da string
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

// Mostra a introduçao do jogo
int mostrarIntro(int opcao) 
{
    clearScreen();
    printf("\033[1;44m==================================================================== / /  / / ====================================================================\033[0m\n");
    const char *intro = "Em uma era de lendas e mitos, ergue-se uma figura audaz e valente: voce, um(a) aventureiro(a) em ascensao, cujo nome ecoa pelas terras como\n"
                        "uma promessa de esperanca. O destino do reino, agora abalado pelas \033[1;45msombras do mal\033[0m, repousa em seus ombros. \033[1;41mO Rei Demonio\033[0m, um ser de poder\n"
                        "inimaginavel e crueldade desmedida, lancou seu veu sombrio sobre a terra, ameacando engolir tudo em um abismo de desespero.\n\n"
                        "Em meio a tempos tao sombrios, o conselheiro do rei, um sabio de visao agucada e coracao puro, ve em voce a \033[1;43mchama da salvacao\033[0m. Ele o(a)\n"
                        "escolhe, dentre tantos, para uma missao que decidira o destino de todos: infiltrar-se no castelo corrompido, um labirinto de perigos\n"
                        "e horrores, e ascender ao topo, onde o mal reina supremo.\n\n"
                        "Com o peso da responsabilidade e a luz da esperanca em seu coracao, voce parte em sua jornada. O caminho eh tortuoso, repleto de adversarios\n"
                        "formidaveis e desafios que testarao sua forca, sua coragem e sua determinacao. A cada passo, a escuridao tenta lhe engolir, mas sua vontade\n"
                        "eh \033[4;43minquebrantavel\033[0m.\n\n"
                        "Voce eh mais do que um(a) aventureiro(a); voce eh a \033[1;44multima esperanca de um mundo\033[0m a beira da ruina. A lenda de suas facanhas sera cantada\n"
                        "por geracoes, um farol de inspiracao para todos aqueles que acreditam que, mesmo na mais escura das noites, a luz da coragem brilha eternamente.\n";
    printCharByChar(intro, 32000); // Imprime o texto letra por letra com um delay de 32 milissegundos entre os chars
    printf("\033[1;44m==================================================================== / /  / / ====================================================================\033[0m\n");
    printf("\033[42mPrecione [1] para jogar.\033[0m\n");
    scanf("%d", &opcao);
    return opcao;
}

// Mostra o menu principal
void mostrarMenuPrincipal(Fase* faseAtual) 
{
    clearScreen();
    int opcao;
    do 
    {
        printf("\033[1;44m| Menu Principal |\033[0m\n\n");
        printf("\033[42m1. Jogar\033[0m\n");
        printf("2. Escolher Dificuldade (Atual: %d)\n", dificuldade);
        printf("3. Como jogar?\n");
        printf("4. High Scores\n");
        printf("\033[41m5. Sair\033[0m\n");
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
                    faseAtual = criarTodasFases(dificuldade); // Cria novamente as fases e monstros
                    mostrarMenuPrincipal(faseAtual); // Mostra o menu principal novamente
                }
                break;
            case 2:
                escolherDificuldade(&faseAtual); // Escolher a dificuldade
                break;
            case 3:
                instrucoes();
                break;
            case 4:
                clearScreen();
                exibirHighScores(); // Exibir os highscores
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

// Escolha da dificuldade
void escolherDificuldade(Fase** faseAtual) 
{
    clearScreen();
    int opcao;
    printf("A dificuldade afeta apenas a quantidade de HP que cada monstro vai possuir...\n");
    printf("Dificuldade atual: %d\n", dificuldade);
    printf("\033[1;42m1. Facil\033[0m\n");
    printf("\033[1;46m2. Medio\033[0m\n");
    printf("\033[1;41m3. Dificil\033[0m\n");
    printf("4. Voltar\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);

    if(opcao >= 1 && opcao <= 3) 
    {
        dificuldade = opcao;
        liberarFases(*faseAtual); // Liberar as fases antigas
        *faseAtual = criarTodasFases(dificuldade); // Criar novas fases com a dificuldade atualizada
        clearScreen();
    } 
    else if(opcao == 4) 
    {
        clearScreen();
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
            "Boa sorte, aventureiro(a)!\n"
            "================================== / /  / / ==================================\n");

}

// Criaçao do Monstro
Monstro* criarMonstro(char* nome, int hp, int defesa, int acao, int danoAtaqueMonstro, int defesaParaAdicionarMonstro) 
{
    Monstro* novoMonstro = (Monstro*)malloc(sizeof(Monstro));
    if (!novoMonstro) 
    {
        printf("Erro ao alocar memoria para o monstro.\n");
        exit(1);
    }
    strncpy(novoMonstro->nome, nome, sizeof(novoMonstro->nome));
    novoMonstro->nome[sizeof(novoMonstro->nome) - 1] = '\0'; // Garante que a string e terminada em '\0'
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

void resetarDefesaMonstros(Monstro* listaMonstros) 
{
    Monstro* monstroAtual = listaMonstros;
    while (monstroAtual != NULL) 
    {
        monstroAtual->defesa = 0;
        monstroAtual = monstroAtual->proximo;
    }
}


// Criaçao da carta
Carta* criarCarta(char* nome, int energia, int acao, int quantidadeAcao, char* descricao) 
{
    Carta* novaCarta = (Carta*)malloc(sizeof(Carta));
    if (!novaCarta) 
    {
        printf("Erro ao alocar memoria para a carta.\n");
        exit(1);
    }
    strncpy(novaCarta->nome, nome, sizeof(novaCarta->nome));
    novaCarta->nome[sizeof(novaCarta->nome) - 1] = '\0'; // Garante que a string e terminada em '\0'
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
    printf("\n\033[4;43m| Escolha uma classe: |\033[0m\n\n");
    printf("\033[1;44m1. Guerreiro\033[0m\n");
    printf("\033[1;45m2. Mago\033[0m\n");
    printf("\033[1;42m3. Arqueiro\033[0m\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);
    return opcao;
}

// Criaçao do Baralho a partir da classe escolhida
Carta** criarBaralho(TipoBaralho tipoBaralho) 
{
    Carta** cartas = malloc(sizeof(Carta*) * 6); // Assumindo 6 cartas por baralho
    if (!cartas) {
        perror("Falha ao alocar memoria para o baralho");
        exit(1);
    }

    switch (tipoBaralho) 
    {
        case BARALHO_GUERREIRO:
            // Nome, Energia, Acao, QuantidadeAcao, Descricao
            cartas[0] = criarCarta("Espada Fulminante", 1, ATAQUESING, 2, "Uma lamina carregada com energia eletrica, aplicando \033[1;41m2 de Dano\033[0m com um impacto eletrizante e rapido."); 
            cartas[1] = criarCarta("Barreira de Escudos", 1, DEFESA, 3, "Uma formacao defensiva impenetravel que proporciona \033[1;43m3 de Escudo\033[0m, simbolizando a forca e resistencia do guerreiro.");
            cartas[2] = criarCarta("Pocao de Cura", 3, CURA, 2, "Uma pocao magica que, ao ser bebida, restaura rapidamente \033[1;42m2 pontos do seu HP\033[0m.");
            cartas[3] = criarCarta("Espada Bumerangue", 2, ATAQUEMULT, 1, "Uma espada magica que ao ser arremessada, ataca multiplos inimigos causando \033[1;41m1 de Dano\033[0m e retorna a mao do lancador.");
            cartas[4] = criarCarta("Perfurar", 2, ATAQUESING, 3, "Um ataque direto e penetrante, perfurando o primeiro inimigo causando \033[1;41m3 de Dano\033[0m.");
            cartas[5] = criarCarta("Impacto Explosivo", 3, ATAQUEMULT, 1, "Uma poderosa onda de choque gerada pela espada do guerreiro, causando \033[1;41m1 de dano\033[0m a todos os inimigos ao redor.");
            break;
        case BARALHO_MAGO:
            // Nome, Energia, Acao, QuantidadeAcao, Descricao
            cartas[0] = criarCarta("Lanca de Fogo", 1, ATAQUESING, 2, "Uma projetil magico de fogo que se lanca contra um unico inimigo, infligindo \033[1;41m2 de Dano\033[0m."); 
            cartas[1] = criarCarta("Barreira de Gelo", 1, DEFESA, 3, "Uma muralha de gelo conjurada para lhe conceder \033[1;43m3 Escudo\033[0m, representando a protecao fria e inabalavel do mago.");
            cartas[2] = criarCarta("Toque Divino", 3, CURA, 2, "Um feitico de cura antigo que revitaliza o lancador, restaurando \033[1;42m2 pontos do seu HP\033[0m.");
            cartas[3] = criarCarta("Nevasca", 2, ATAQUEMULT, 1, "Uma tempestade congelante que envolve todos os inimigos, causando \033[1;41m1 de Dano\033[0m com a forca implacavel do inverno.");
            cartas[4] = criarCarta("Trovao de Jupiter", 2, ATAQUESING, 3, "Um relampago celestial convocado para atingir um inimigo com forca divina, causando \033[1;41m3 de Dano\033[0m eletrico.");
            cartas[5] = criarCarta("Chuva de Meteoros", 3, ATAQUEMULT, 1, "Uma chuva apocaliptica de meteoros que cai sobre todos os inimigos, causando \033[1;41m1 de dano\033[0m em cada um deles.");
            break;
        case BARALHO_ARQUEIRO:
            // Nome, Energia, Acao, QuantidadeAcao, Descricao
            cartas[0] = criarCarta("Tiro Preciso", 1, ATAQUESING, 2, "Uma flecha lancada com precisao cirurgica, capaz de encontrar seu alvo e causar \033[1;41m2 de Dano\033[0m a um unico alvo."); 
            cartas[1] = criarCarta("Manto das Sombras", 1, DEFESA, 3, "Um manto encantado que envolve o arqueiro nas sombras, proporcionando \033[1;43m3 Escudo\033[0m");
            cartas[2] = criarCarta("Pocao de Cura", 3, CURA, 2, "Um elixir curativo que, quando consumido, restaura \033[1;42m2 pontos do HP\033[0m do arqueiro, permitindo-lhe continuar a luta.");
            cartas[3] = criarCarta("Chuva de Flechas", 2, ATAQUEMULT, 1, "Uma barragem de flechas lancadas ao ceu, que caem em uma area ampla causando \033[1;41m1 de dano\033[0m a todos os inimigos.");
            cartas[4] = criarCarta("Flecha perfurante", 2, ATAQUESING, 3, "Uma flecha reforcada capaz de penetrar o coracao do primeiro inimigo, infligindo \033[1;41m3 de Dano\033[0m.");
            cartas[5] = criarCarta("Rajada Tripla de Flechas", 3, ATAQUEMULT, 1, "Um disparo simultaneo de tres flechas, cada uma causando \033[1;41m1 de dano\033[0m, ideal para alvejar multiplos adversarios.");
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

// Retorna 1 se o jogador escolher reiniciar, 0 caso contrario
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
        telaDerrota();
    }

    if(fimDeJogo == 1)
    {
        telaVitoria();
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
        printf("\033[4mFase Atual: %d\033[0m\n", (*faseAtual)->nivelFase); // Mostrar a fase atual
        mostrarInformacoesTurnoJogador(cartas); // Mostrar as informações do jogador
        mostrarInformacoesTurnoMonstros(*faseAtual); // Mostrar as informações dos monstros
        escolherEJogarCarta(*faseAtual, cartas); // Escolher e jogar uma carta

        if((*faseAtual)->monstros == NULL)
        {
            if((*faseAtual)->nivelFase == 3)
            {
                fimDeJogo = 1; // Marcar como fim de jogo se todos os monstros da ultima fase foram derrotados
                break; // Sair do loop do turno
            }
            else
            {
                *faseAtual = (*faseAtual)->proxima; // Avançar para a proxima fase se todos os monstros morreram
                resetarDefesaMonstros((*faseAtual)->monstros); // Resetar a defesa dos monstros para a proxima fase
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
        monstroAtual->defesa = 0; // Resetar a defesa do monstro para o proximo turno
        if(monstroAtual->acao == ATAQUEMONSTRO) // Se o monstro escolheu atacar
        {
            if (defesaJogador > 0) // Verificar se o jogador tem escudo para se defender
            {
                int danoAplicado = min(defesaJogador, monstroAtual->danoAtaqueMonstro); // Verificar se a defesa do jogador e maior que o dano do monstro
                defesaJogador -= danoAplicado; // Reduzir a defesa do jogador
                if (monstroAtual->danoAtaqueMonstro > danoAplicado) // Verificar se o dano do monstro e maior que a defesa do jogador
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

        monstroAtual->intencaoDefinida = 0; // Resetar a variavel de controle para o proximo turno
        monstroAtual->acao = DEFAULTMONSTRO; // Resetar a açao do monstro para o proximo turno
        monstroAtual = monstroAtual->proximo; // Avançar para o proximo monstro
    }

    energiaJogador = 3; // Restaurar a energia do jogador ao finalizar o turno
    defesaJogador = 0; // Zerar a defesa do jogador para o proximo turno
    numTurno++; // Incrementar o numero do turno
    turnoFinalizado = 0; // Resetar a variavel de controle para o proximo turno
    clearScreen(); // Limpar a tela
}

// Mostra as informações do jogador no turno atual
void mostrarInformacoesTurnoJogador(Carta** cartas) 
{
    printf("\033[4mTurno Atual: %d\033[0m\n", numTurno);
    printf("\033[1;42mHP do Jogador: %d\033[0m\n", hpJogador);
    printf("\033[1;44mEnergia: %d\033[0m\n", energiaJogador);
    printf("\033[1;43mEscudos: %d\033[0m\n", defesaJogador);

    printf("================================== / /  / / ==================================\n\n");
    
    printf("\033[1;7;44mSuas Cartas:\033[0m\n");
    for (int i = 0; i < 6; i++) 
    {
        printf("[%d]Nome: %s, \033[1;46mEnergia: %d\033[0m, Descricao: %s\n", 
        i+1,
        cartas[i]->nome, 
        cartas[i]->energia, 
        cartas[i]->descricao);
    }
    printf("\n================================== / /  / / ==================================\n");
}

// Mostra as informações dos monstros no turno atual
void mostrarInformacoesTurnoMonstros(Fase* faseAtual)
{
    printf("\033[1;45m| Monstros: |\033[0m\n\n");
    Monstro* monstroAtual = faseAtual->monstros;
    while (monstroAtual != NULL) 
    {
        printf("\033[1;45mNome: %s\033[0m |HP: %d| |Escudos: %d|\n", monstroAtual->nome, monstroAtual->hp, monstroAtual->defesa);
        if(monstroAtual->acao == 1)
        {
            printf("%s, vai atacar com \033[1;41m%d de dano!\033[0m\n\n", monstroAtual->nome, monstroAtual->danoAtaqueMonstro);
        }
        else
        {
            printf("%s, vai defender com \033[1;44m%d de defesa!\033[0m\n\n", monstroAtual->nome, monstroAtual->defesaParaAdicionarMonstro);
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
        return;
    }

    if(faseAtual->monstros == NULL) // Passa o turno mesmo o player apertando outro numero que nao seja 7 apos todos os monstros morrerem
    {
        turnoFinalizado = 1;
        return;
    }

    Carta* cartaEscolhida = cartas[escolha - 1];
    jogarCarta(cartaEscolhida, faseAtual->monstros, faseAtual);

    printf("\033[1;43m================================== FIM das Consequencias das suas acoes ==================================\033[0m\n");
}

void jogarCarta(Carta* carta, Monstro* monstro, Fase* faseAtual)
{
    clearScreen();
    printf("\033[1;43m================================== Consequencias das suas acoes ==================================\033[0m\n");
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
                if (danoRestante > 0) 
                {
                    monstro->hp = max(0, monstro->hp - danoRestante);
                }
            } 
            else 
            {
                monstro->hp = max(0, monstro->hp - carta->quantidadeAcao);
            }
            printf("\033[4;41mVoce causou %d de dano ao monstro %s!\033[0m\n", carta->quantidadeAcao, monstro->nome);
            break;
            case ATAQUEMULT:
            while (monstroAtual != NULL)
            {
                if (monstroAtual->defesa > 0) 
                {
                    int danoRestante = carta->quantidadeAcao - monstroAtual->defesa;
                    monstroAtual->defesa = max(0, monstroAtual->defesa - carta->quantidadeAcao);
                    if (danoRestante > 0) 
                    {
                        monstroAtual->hp = max(0, monstroAtual->hp - danoRestante);
                        printf("Voce causou %d de dano ao monstro %s!\n", danoRestante, monstroAtual->nome);
                    }
                    else
                    {
                        printf("Voce atingiu a defesa do monstro %s!\n", monstroAtual->nome);
                    }
                } 
                else 
                {
                    monstroAtual->hp = max(0, monstroAtual->hp - carta->quantidadeAcao);
                    printf("Voce causou %d de dano ao monstro %s!\n", carta->quantidadeAcao, monstroAtual->nome);
                }
                monstroAtual = monstroAtual->proximo;
            }
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
    while (current != NULL) 
    {
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

        // Atualiza o node atual
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

Fase* criarTodasFases(int dificuldade) 
{
    // Nome, HP, Defesa, Acao, DanoAtaqueMonstro, DefesaParaAdicionarMonstro
    // Criando 3 monstros para fase 1
    Monstro* monstro1 = criarMonstro("Goblin Guerreiro", 2 * dificuldade, 0, DEFAULTMONSTRO, 2, 1); 
    Monstro* monstro2 = criarMonstro("Goblin Arqueiro", 1 * dificuldade, 0, DEFAULTMONSTRO, 2, 1); 
    Monstro* monstro3 = criarMonstro("Orc Guerreiro", 2 * dificuldade, 0, DEFAULTMONSTRO, 3, 2); 

    // Criando 3 monstros para fase 2
    Monstro* monstro4 = criarMonstro("Hobgoblin", 3 * dificuldade, 0, DEFAULTMONSTRO, 3, 2); 
    Monstro* monstro5 = criarMonstro("Elfo Mago", 2 * dificuldade, 0, DEFAULTMONSTRO, 3, 1); 
    Monstro* monstro6 = criarMonstro("Succubus", 3 * dificuldade, 0, DEFAULTMONSTRO, 4, 2); 

    // Criando 2 monstros e o Boss para fase 3
    Monstro* monstro7 = criarMonstro("Succubus", 3 * dificuldade, 0, DEFAULTMONSTRO, 4, 2); 
    Monstro* monstro8 = criarMonstro("Dragao", 4 * dificuldade, 0, DEFAULTMONSTRO, 5, 3); 
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
        free(fase); // Libera a memoria da fase

        fase = proxima;
    }
}

void resetarEstadoJogo() 
{
    hpJogador = 10; // Valor inicial do HP do jogador
    energiaJogador = 3; // Valor inicial da energia
    numTurno = 1; // Resetar o contador de turnos
    fimDeJogo = 0; // Resetar a condiçao de fim de jogo
    turnoFinalizado = 0; // Resetar a variavel de controle do turno
}

void salvarHighScoresArquivo(const HighScore* scores, int numScores)
{
    FILE* arquivo = fopen("highscores.txt", "a"); // Modo de anexaçao
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
    HighScore scores[100]; // Ajuste o tamanho conforme necessario
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

void telaVitoria()
{
    printf("\033[1;44m==================================================================== / /  / / ====================================================================\033[0m\n");
    const char *intro = "\033[1;43mApos uma jornada ardua e repleta de desafios, voce, um(a) valente aventureiro(a), finalmente confronta e vence o Rei Demonio, restaurando a\n"
                        "paz e a luz ao reino. O veu sombrio que uma vez ameacava engolir tudo em desespero agora se desfaz, como nevoa ao amanhecer.\n\n"
                        "Em um ato final de bravura e forca, voce triunfa sobre a escuridao. As sombras que assolavam a terra se dissipam, e o sol brilha novamente,\n"
                        "mais brilhante do que nunca. O reino, antes sufocado pelo medo, agora ressoa com cânticos de vitoria e alegria.\n\n"
                        "Voce retorna ao castelo, nao mais como um(a) mero(a) aventureiro(a), mas como um(a) heroi(ina) cujo nome sera lembrado por todas as geracões\n"
                        "futuras. O conselheiro do rei, com lagrimas nos olhos, agradece por sua coragem inabalavel que salvou o reino da ruina.\n\n"
                        "Sua jornada se torna uma lenda, um conto de esperanca e coragem que sera passado adiante, inspirando outros a acreditar que mesmo na mais\n"
                        "escura das batalhas, a luz da bravura nunca se apaga.\033[0m\n\n";
    printCharByChar(intro, 30500); // Imprime o texto letra por letra com um delay de 30ms entre cada letra
    printf("\033[1;44m==================================================================== / /  / / ====================================================================\033[0m\n");
    printf("Voce precisou de %d turnos para concluir o jogo!\n", numTurno);
}

void telaDerrota()
{
    printf("\033[1;44m==================================================================== / /  / / ====================================================================\033[0m\n");
    const char *intro = "\033[7;45mAo cair, derrotado(a) e exausto(a), um silencio sepulcral toma conta do campo de batalha. O Rei Demonio ergue-se, imponente, sobre o reino\n"
                        "agora sem esperanca. As sombras, como garras frias e mortais, se estendem por cada canto, extinguindo a luz e a vida por onde passam.\n\n"
                        "Os sobreviventes, escondidos nas sombras de um mundo agora irreconhecivel, murmuram seu nome com um misto de respeito e desespero. Voce se\n"
                        "torna uma lembranca de uma ultima tentativa fracassada de salvacao, um simbolo da coragem que, no fim, foi insuficiente diante do avanco\n"
                        "implacavel das trevas.\n\n"
                        "Nas ruinas do que ja foi um reino prospero e cheio de vida, o lamento por sua queda eh eterno. Seu sacrificio eh lembrado, mas a sombra do\n"
                        "Rei Demonio eh longa e opressora, sufocando ate mesmo a memoria dos mais bravos herois.\033[0m\n\n";
    printCharByChar(intro, 30000); // Imprime o texto letra por letra com um delay de 30ms entre cada letra
    printf("\033[1;44m==================================================================== / /  / / ====================================================================\033[0m\n");
    printf("Voce passou por %d turnos antes de morrer!\n", numTurno);
}