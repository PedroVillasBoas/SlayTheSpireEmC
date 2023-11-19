#ifndef HIGHSCORE_H
#define HIGHSCORE_H

// Estrutura do HighScore
typedef struct HighScore 
{
    char nomeJogador[50]; // Nome do jogador
    int faseAlcancada;    // Fase alcancada
    int numTurnos;        // Numero de turnos
    int hpRestante;       // HP restante
    int dificuldade;      // Nivel de dificuldade
} HighScore;

void registrarHighScore(const char* nomeJogador,int dificuldade, int faseAlcancada, int numTurnos, int hpRestante);
void exibirHighScores();
void salvarHighScoresArquivo(const HighScore* scores, int numScores);
int compararHighScores(const void* a, const void* b);

#endif // HIGHSCORE_H
