#include "highscore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Salva na Struct o nome do jogador, a fase alcançada, o numero de turnos e o HP restante para serem salvos no arquivo
void registrarHighScore(const char* nomeJogador,int dificuldade, int faseAlcancada, int numTurnos, int hpRestante) 
{
    HighScore score;
    strcpy(score.nomeJogador, nomeJogador);
    score.dificuldade = dificuldade;
    score.faseAlcancada = faseAlcancada;
    score.numTurnos = numTurnos;
    score.hpRestante = hpRestante;

    salvarHighScoresArquivo(&score, 1);
}

// Exibe os highscores salvos no arquivo
void exibirHighScores() 
{
    HighScore scores[100]; // Ajuste o tamanho conforme necessário
    int numScores = 0;
    char linha[200]; // Buffer para ler cada linha do arquivo

    // Carregar os scores do arquivo
    FILE* arquivo = fopen("highscores.txt", "r");
    if (arquivo == NULL) 
    {
        perror("Erro ao abrir arquivo de high scores");
        return;
    }

    while (fscanf(arquivo, "%49s - Dificuldade: %d, Fase: %d, Turnos: %d, HP: %d\n", 
                scores[numScores].nomeJogador, 
                &scores[numScores].dificuldade,
                &scores[numScores].faseAlcancada, 
                &scores[numScores].numTurnos, 
                &scores[numScores].hpRestante) == 5 && numScores < 100) 
    {
        numScores++;
    }

    fclose(arquivo);

    // Ordenar os scores
    qsort(scores, numScores, sizeof(HighScore), compararHighScores);

    // Exibir os scores
    printf("\033[1;43mHigh Scores:\033[0m\n");
    for (int i = 0; i < numScores; i++) 
    {
        printf("%d. %s - Dificuldade: %d, Fase: %d, Turnos: %d, HP: %d\n",
       i + 1, scores[i].nomeJogador, scores[i].dificuldade, scores[i].faseAlcancada, scores[i].numTurnos, scores[i].hpRestante);

    }
}

// Salva os highscores no arquivo
void salvarHighScoresArquivo(const HighScore* scores, int numScores)
{
    FILE* arquivo = fopen("highscores.txt", "a"); // Modo de anexaçao
    if (arquivo == NULL) 
    {
        perror("Erro ao abrir arquivo de high scores");
        return;
    }

    for (int i = 0; i < numScores; i++) 
    {
        fprintf(arquivo, "%s - Dificuldade: %d, Fase: %d, Turnos: %d, HP: %d\n",
                scores[i].nomeJogador, scores[i].dificuldade, scores[i].faseAlcancada, scores[i].numTurnos, scores[i].hpRestante);
    }

    fclose(arquivo);
}

// Funçao para comparar os highscores (usada no qsort)
int compararHighScores(const void* a, const void* b) 
{
    const HighScore* scoreA = (const HighScore*)a;
    const HighScore* scoreB = (const HighScore*)b;

    // Prioridade 1: Maior nível de dificuldade
    if (scoreA->dificuldade != scoreB->dificuldade) 
    {
        return scoreB->dificuldade - scoreA->dificuldade;
    }
    // Prioridade 2: Maior fase alcançada (se a dificuldade for a mesma)
    if (scoreA->faseAlcancada != scoreB->faseAlcancada) 
    {
        return scoreB->faseAlcancada - scoreA->faseAlcancada;
    }
    // Prioridade 3: Menor número de turnos (se a fase for a mesma)
    if (scoreA->numTurnos != scoreB->numTurnos) 
    {
        return scoreA->numTurnos - scoreB->numTurnos;
    }
    // Prioridade 4: Maior HP restante (se a fase e o número de turnos forem os mesmos)
    return scoreB->hpRestante - scoreA->hpRestante;
}

