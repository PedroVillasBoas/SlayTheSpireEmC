# Slay The Spire Em C

## Introdução:
* Este projeto consiste em um jogo de cartas baseado em turnos onde o jogador enfrenta monstros em diferentes fases. 
* O jogo é dividido em três fases, cada uma com seus respectivos monstros. O jogador pode escolher cartas de sua mão para atacar os monstros ou defender-se de seus ataques.
* O *SlayTheSpireEmC* é baseado no jogo Slay the Spire que é um roguelike de construção de deck de cartas. Ele foi desenvolvido pelo estudio indie americano Mega Crit e publicado pelo Humble Bundle.

## Estrutura do Código
O código é organizado nas seguintes partes principais:

## Inicialização:
* Geração de números aleatórios é inicializada.
* Monstros são criados para cada fase em uma lista duplamente encadeada.
* Cartas são criadas para o jogador em um array.
* Fases são criadas e encadeadas em uma lista simplesmente encadeada.

## Menu Principal:
O jogador pode optar por começar o jogo, escolher a dificuldade, aprender como jogar, ver o High-Score ou sair do jogo.

## Jogo:
* O jogador primeiro escolhe sua Classe. Onde cada classe possui um conjunto diferente de cartas.
* O jogo é jogado em turnos, onde o jogador pode jogar uma carta de sua mão utilizando a energia disponivel.
* O jogador pode atacar monstros ou defender-se de ataques ou curar-se com suas cartas.
* As intenções dos monstros são definidas no inicio do turno e executadas após o jogador escolher encerrar seu turno.
* O jogo continua até que o jogador derrote todos os monstros, em todas as fases, ou seja derrotado.

## Utilitários:
Funções adicionais para limpar a tela, mostrar instruções, mostrar tela de vitoria e derrota, gerenciar o estado do jogo e criação dos Nodes para as cartas, fases e monstros, entre outras.

## Como Compilar e Executar
1. Certifique-se de que você tem um compilador C instalado em seu sistema.
2. Abra um terminal ou prompt de comando.
3. Navegue até o diretório onde o arquivo slaythespire.c está localizado.
4. Pra fazer funcionar no windows executar os seguintes comandos no terminal (1 por vez):
5. gcc -c slaythespire.c util.c turno.c player.c monstro.c menu.c highscore.c fases.c carta.c
6. gcc -o SlayTheSpireGame slaythespire.o util.o turno.o player.o monstro.o menu.o highscore.o fases.o carta.o
7. E pode executar o arquivo: SlayTheSpireGame.exe

## Jogo Desenvolvido por:
* Pedro Vilas Bôas
* Guilherme Lopes

## Licença
Este projeto é licenciado sob a licença MIT - veja o arquivo LICENSE para mais detalhes.
