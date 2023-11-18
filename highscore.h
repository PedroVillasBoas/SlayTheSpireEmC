#ifndef HIGHSCORE_H
#define HIGHSCORE_H

typedef struct HighScore {
    char nomeJogador[50];
    int faseAlcancada;
    int numTurnos;
    int hpRestante;
} HighScore;

void registrarHighScore(const char* nomeJogador, int faseAlcancada, int numTurnos, int hpRestante);
void exibirHighScores();
void salvarHighScoresArquivo(const HighScore* scores, int numScores);
int compararHighScores(const void* a, const void* b);

#endif // HIGHSCORE_H
