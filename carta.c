// Inclui todas as funcionalidades das cartas

#include "carta.h"
#include "util.h"

// Criaçao da carta
Carta* criarCarta(char* nome, int energia, int acao, int quantidadeAcao, char* descricao) 
{
    Carta* novaCarta = (Carta*)malloc(sizeof(Carta)); // Alocar memoria para a carta
    if (!novaCarta) 
    {
        printf("Erro ao alocar memoria para a carta.\n");
        exit(1);
    }
    strncpy(novaCarta->nome, nome, sizeof(novaCarta->nome));                // Copia o nome para a carta
    novaCarta->nome[sizeof(novaCarta->nome) - 1] = '\0';                    // Garante que a string e terminada em '\0'
    novaCarta->energia = energia;                                           // Copia a energia para a carta
    novaCarta->acao = acao;                                                 // Copia a açao para a carta
    novaCarta->quantidadeAcao = quantidadeAcao;                             // Copia a quantidade de açao para a carta
    strncpy(novaCarta->descricao, descricao, sizeof(novaCarta->descricao)); // Copia a descriçao para a carta
    novaCarta->descricao[sizeof(novaCarta->descricao) - 1] = '\0';          // Garante que a string e terminada em '\0'
    novaCarta->jaJogada = 0;                                                // Inicializa a carta como nao jogada

    novaCarta->proximo = NULL;
    novaCarta->anterior = NULL;
    return novaCarta;
}

// Criaçao do Baralho a partir da classe escolhida
Carta** criarBaralho(TipoBaralho tipoBaralho) 
{
    Carta** cartas = malloc(sizeof(Carta*) * numCartasBaralho);             // Alocar memoria para o baralho de acordo com o numero de cartas
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
            cartas[2] = criarCarta("Pocao de Cura", 3, CURA, 3, "Uma pocao magica que, ao ser bebida, restaura rapidamente \033[1;42m3 pontos do seu HP\033[0m.");
            cartas[3] = criarCarta("Espada Bumerangue", 2, ATAQUEMULT, 2, "Uma espada magica que ao ser arremessada, ataca multiplos inimigos causando \033[1;41m2 de Dano\033[0m e retorna a mao do lancador.");
            cartas[4] = criarCarta("Perfurar", 2, ATAQUESING, 3, "Um ataque direto e penetrante, perfurando o primeiro inimigo causando \033[1;41m3 de Dano\033[0m.");
            cartas[5] = criarCarta("Impacto Explosivo", 2, DEFESA, 4, "Uma poderosa onda de choque é gerada, deixando os monstros confusos e lhe da \033[1;43m4 de Escudo\033[0m.");
            break;
        case BARALHO_MAGO:
            // Nome, Energia, Acao, QuantidadeAcao, Descricao
            cartas[0] = criarCarta("Lanca de Fogo", 1, ATAQUESING, 2, "Uma projetil magico de fogo que se lanca contra um unico inimigo, infligindo \033[1;41m2 de Dano\033[0m."); 
            cartas[1] = criarCarta("Barreira de Gelo", 1, DEFESA, 3, "Uma muralha de gelo conjurada para lhe conceder \033[1;43m3 Escudo\033[0m, representando a protecao fria e inabalavel do mago.");
            cartas[2] = criarCarta("Toque Divino", 3, CURA, 4, "Um feitico de cura antigo que revitaliza o lancador, restaurando \033[1;42m4 pontos do seu HP\033[0m.");
            cartas[3] = criarCarta("Nevasca", 1, ATAQUEMULT, 1, "Uma tempestade congelante que envolve todos os inimigos, causando \033[1;41m1 de Dano\033[0m com a forca implacavel do inverno.");
            cartas[4] = criarCarta("Trovao de Jupiter", 2, ATAQUESING, 3, "Um relampago celestial convocado para atingir um inimigo com forca divina, causando \033[1;41m3 de Dano\033[0m eletrico.");
            cartas[5] = criarCarta("Chuva de Meteoros", 2, ATAQUEMULT, 2, "Uma chuva apocaliptica de meteoros que cai sobre todos os inimigos, causando \033[1;41m2 de dano\033[0m em cada um deles.");
            break;
        case BARALHO_ARQUEIRO:
            // Nome, Energia, Acao, QuantidadeAcao, Descricao
            cartas[0] = criarCarta("Tiro Preciso", 1, ATAQUESING, 1, "Uma flecha lancada com precisao cirurgica, capaz de encontrar seu alvo e causar \033[1;41m1 de Dano\033[0m a um unico alvo."); 
            cartas[1] = criarCarta("Manto das Sombras", 1, DEFESA, 3, "Um manto encantado que envolve o arqueiro nas sombras, proporcionando \033[1;43m3 Escudo\033[0m");
            cartas[2] = criarCarta("Pocao de Cura", 3, CURA, 2, "Um elixir curativo que, quando consumido, restaura \033[1;42m2 pontos do HP\033[0m do arqueiro, permitindo-lhe continuar a luta.");
            cartas[3] = criarCarta("Chuva de Flechas", 2, ATAQUEMULT, 1, "Uma barragem de flechas lancadas ao ceu, que caem em uma area ampla causando \033[1;41m1 de dano\033[0m a todos os inimigos.");
            cartas[4] = criarCarta("Flecha perfurante", 1, ATAQUESING, 2, "Uma flecha reforcada capaz de penetrar o coracao do primeiro inimigo, infligindo \033[1;41m2 de Dano\033[0m.");
            cartas[5] = criarCarta("Rajada Tripla de Flechas", 2, ATAQUEMULT, 1, "Um disparo simultaneo de tres flechas, cada uma causando \033[1;41m1 de dano\033[0m, ideal para alvejar multiplos adversarios.");
            break;
        default:
            fprintf(stderr, "Tipo de baralho desconhecido\n");
            free(cartas);
            exit(1);
    }

    return cartas;
}

// Liberar a memoria do baralho
void liberarBaralho(Carta** cartas, int numCartas) 
{
    for (int i = 0; i < numCartas; i++) 
    {
        free(cartas[i]); 
    }
    free(cartas);
}

// Reseta o estado das cartas ja jogadas para o proximo turno
void resetarEstadoCartas(Carta** cartas, int numCartas) 
{
    for (int i = 0; i < numCartas; i++) 
    {
        cartas[i]->jaJogada = 0; // Reinicializa a carta para o próximo turno
    }
}