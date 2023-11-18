#include "turno.h"


// Iniciar o jogo | Contem a Tela de Derrota e Vitoria
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
    
    registrarHighScore(nomeJogador, dificuldade, (*faseAtual)->nivelFase, numTurno, hpJogador); // Registrar o highscore do jogador
    
    printf("Deseja jogar novamente? [1]SIM ou [2]NAO: ");
    int escolha;
    scanf("%d", &escolha);
    return escolha;
}

// Jogar o turno (Loop principal do jogo)
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
    resetarEstadoCartas(cartas, numCartasBaralho); // Resetar o estado das cartas para o proximo turno
    
    if((*faseAtual)->nivelFase == 3 && (*faseAtual)->monstros == NULL)
    {
        fimDeJogo = 1;
    }

}

// Finaliza o turno e aplica as açoes dos monstros
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
        printf("[%d] %s \033[1;46mEnergia: %d\033[0m Desc.: %s", 
        i+1,
        cartas[i]->nome, 
        cartas[i]->energia, 
        cartas[i]->descricao);
        if (cartas[i]->jaJogada) 
        {
            printf(" - \033[1;41m(Carta Jogada)\033[0m");
        }

        printf("\n");
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

// Recebe o input do jogador e joga a carta escolhida
void escolherEJogarCarta(Fase* faseAtual, Carta** cartas) 
{
    int escolha;
    printf("Escolha uma carta para jogar (1-6) OU [7] Para encerrar o turno: ");
    scanf("%d", &escolha);
    
    if (escolha < 1 || escolha > 7) 
    {
        clearScreen();
        printf("\033[1;41mEscolha invalida! Tente novamente.\033[0m\n");
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

// Jogar uma carta e aplciar suas consequencias
void jogarCarta(Carta* carta, Monstro* monstro, Fase* faseAtual)
{
    if(carta->jaJogada == 1)
    {
        clearScreen();
        printf("\033[1;43m================================== Consequencias das suas acoes ==================================\033[0m\n");
        printf("Voce ja jogou esta carta!\n");
        return;
    }

    clearScreen();
    printf("\033[1;43m================================== Consequencias das suas acoes ==================================\033[0m\n");
    Monstro* monstroAtual = faseAtual->monstros;
    if (energiaJogador >= carta->energia) 
    {
        energiaJogador -= carta->energia;
        printf("Voce jogou a carta %s!\n", carta->nome);

        switch (carta->acao) 
        {
            case ATAQUESING: // Ataque de alvo unico
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

            case ATAQUEMULT: // Ataque com multiplos alvos
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

            case DEFESA: // Gera escudo para o jogador
                defesaJogador += carta->quantidadeAcao;
                break;

            case CURA: // Cura o jogador
                hpJogador += carta->quantidadeAcao;
                break;

            default: // Acao desconhecida
                printf("Acao desconhecida!\n");
                break;
        }
    } 
    else 
    {
        printf("Energia insuficiente para jogar esta carta!\n");
    }
    carta->jaJogada = 1;
}