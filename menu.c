// Inclui todas as funcionalidades do Menu Principal, assim como a Introdução, Tela de Vitoria e Tela de Derrota

#include "menu.h"

// Mostra a introduçao do jogo
int mostrarIntro(int opcao) 
{
    clearScreen();
    printf("\033[1;44m==================================================================== / /  / / ====================================================================\033[0m\n");
    const char *intro = "Em uma era de lendas e mitos, voce eh um(a) aventureiro(a) em ascensao, cujo nome representa esperanca. O destino do reino,\n"
                        "afetado pelas \033[1;45msombras do mal\033[0m, depende de voce. \033[1;41mO Rei Demonio\033[0m, um ser poderoso e cruel, ameaca a terra com desespero.\n\n"
                        "O conselheiro do rei ve em voce a \033[1;43mchama da salvacao\033[0m e o(a) escolhe para uma missao crucial:\n" 
                        "infiltrar-se no castelo corrompido, enfrentando perigos e horrores, para derrotar o mal.\n\n"
                        "Sua jornada eh cheia de adversidades, mas sua vontade eh \033[1;43minquebrantavel\033[0m. Voce representa a \033[1;44multima esperanca\033[0m"
                        " em um mundo a beira do colapso.\nSua lenda, um simbolo de coragem, inspirara geracoes, mostrando que mesmo na escuridao, a luz da coragem permanece.\n";
    printCharByChar(intro, 20000); // Imprime o texto letra por letra com um delay de 32 milissegundos entre os chars
    printf("\033[1;44m==================================================================== / /  / / ====================================================================\033[0m\n");
    printf("\033[42mPressione [1] para jogar.\033[0m\n");
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
        tituloGame();
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
                obterNomeJogador(nomeJogador, sizeof(nomeJogador));     // Obter o nome do jogador
                Carta** cartas = criarBaralho(escolherClasses());       // Criar o baralho do jogador
                if (iniciarJogo(&faseAtual, cartas))                    // Se o jogador escolher reiniciar quando ele morrer ou ganhar o jogo
                {
                    obterNomeJogador(nomeJogador, sizeof(nomeJogador)); // Obter o nome do jogador quando o jogo reiniciar
                    resetarEstadoJogo();                                // Reseta o estado do jogo
                    liberarFases(faseAtual);                            // Libera a fase atual e monstros antes de reiniciar
                    liberarBaralho(cartas, 6);                          // Libera o baralho antes de reiniciar
                    faseAtual = criarTodasFases(dificuldade);           // Cria novamente as fases e monstros
                    mostrarMenuPrincipal(faseAtual);                    // Mostra o menu principal novamente
                }
                break;
            case 2:
                escolherDificuldade(&faseAtual); // Escolher a dificuldade
                break;
            case 3:
                instrucoes();                   // Mostrar as instruçoes do jogo
                break;
            case 4:
                clearScreen();
                exibirHighScores();             // Exibir os highscores
                printf("\033[1;44m========================== / /  / / ==========================\033[0m\n");
                break;
            case 5:
                printf("Saindo...\n");
                exit(0);
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
        liberarFases(*faseAtual);                  // Liberar as fases antigas
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

// Mostra o tutorial do jogo
void instrucoes()
{
    clearScreen();
    printf( "- O jogo eh dividido em 3 fases contendo, cada uma, 3 monstros.\n"
            "- Os monstros podem escolher lhe atacar ou se defender de seus ataques.\n"
            "- Voce vai utilizar suas cartas para destruir os monstros, se defender ou recuperar seu HP.\n"
            "- Mas cuidado! Voce apenas possui 3 de energia por turno e suas cartas custam energia!\n"
            "- Voce nao pode usar a mesma carta mais de 1 vez por turno! Entao pense com cuidado nas suas acoes\n"
            "- Porem, tambem nao se preocupe que ao encerrar o turno, a sua energia sera restaurada totalmente e suas cartas resetadas.\n"
            "- Voce deve concluir as 3 fases na menor quantidade de turnos possiveis. Por isso, pense com cuidado em suas acoes.\n"
            "- Boa sorte, aventureiro(a)!\n"
            "================================== / /  / / ==================================\n");

}

// Escolha da classe que define o baralho que o player vai usar
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

// Mostra a tela de vitoria após player vencer o jogo
void telaVitoria()
{
    printf("\033[1;44m==================================================================== / /  / / ====================================================================\033[0m\n");
    const char *intro = "\033[1;43mApos uma jornada ardua e repleta de desafios, voce, um(a) valente aventureiro(a), finalmente confronta e vence o Rei Demonio, restaurando a\n"
                        "paz e a luz ao reino. O veu sombrio que uma vez ameacava engolir tudo em desespero agora se desfaz, como nevoa ao amanhecer.\n\n"
                        "Em um ato final de bravura e forca, voce triunfa sobre a escuridao. As sombras que assolavam a terra se dissipam, e o sol brilha novamente,\n"
                        "mais brilhante do que nunca. O reino, antes sufocado pelo medo, agora ressoa com canticos de vitoria e alegria.\n\n"
                        "Voce retorna ao castelo, nao mais como um(a) mero(a) aventureiro(a), mas como um(a) heroi(ina) cujo nome sera lembrado por todas as geracoes\n"
                        "futuras. O conselheiro do rei, com lagrimas nos olhos, agradece por sua coragem inabalavel que salvou o reino da ruina.\n\n"
                        "Sua jornada se torna uma lenda, um conto de esperanca e coragem que sera passado adiante, inspirando outros a acreditar que mesmo na mais\n"
                        "escura das batalhas, a luz da bravura nunca se apaga.\033[0m\n\n";
    printCharByChar(intro, 20500); // Imprime o texto letra por letra com um delay de 20ms entre cada letra
    printf("\033[1;44m==================================================================== / /  / / ====================================================================\033[0m\n");
    printf("Voce precisou de %d turnos para concluir o jogo!\n", numTurno);
}

// Mostra a tela de derrota após player perder o jogo
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
    printCharByChar(intro, 20000); // Imprime o texto letra por letra com um delay de 20ms entre cada letra
    printf("\033[1;44m==================================================================== / /  / / ====================================================================\033[0m\n");
    printf("Voce passou por %d turnos antes de morrer!\n", numTurno);
}