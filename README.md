# SlayTheSpire Em C

## Introdução:
Este projeto consiste em um jogo de cartas baseado em turnos onde o jogador enfrenta monstros em diferentes fases. O jogo é dividido em três fases, cada uma com seus respectivos monstros. O jogador pode escolher cartas de sua mão para atacar os monstros ou defender-se de seus ataques.
O *SlayTheSpireEmC* é baseado no jogo Slay the Spire que é um roguelike de construção de deck de cartas. Ele foi desenvolvido pelo estudio indie americano Mega Crit e publicado pelo Humble Bundle.

## Estrutura do Código
O código é organizado nas seguintes partes principais:

## Inicialização:
Geração de números aleatórios é inicializada.
Monstros são criados para cada fase em uma lista duplamente encadeada.
Cartas são criadas para o jogador em um array.
Fases são criadas e encadeadas em uma lista simplesmente encadeada.

## Menu Principal:
O jogador pode optar por começar o jogo, escolher a dificuldade, aprender como jogar ou sair do jogo.

## Jogo:
O jogo é jogado em turnos, onde o jogador pode jogar uma carta de sua mão utilizando a energia disponivel.
O jogador pode atacar monstros ou defender-se de ataques com suas cartas.
As intenções dos monstros são definidas no inicio do turno e executadas após o turno do jogador.
O jogo continua até que o jogador derrote todos os monstros, em todas as fases, ou seja derrotado.

## Utilitários:
Funções adicionais para limpar a tela, mostrar instruções, gerenciar o estado do jogo e criação dos Nodes para as cartas, fases e monstros.

## Como Compilar e Executar
Certifique-se de que você tem um compilador C instalado em seu sistema.
Abra um terminal ou prompt de comando.
Navegue até o diretório onde o arquivo slaythespire.c está localizado.
Compile o código usando o comando gcc -o jogo slaythespire.c.
Execute o programa usando o comando ./slaythespire.

## Funcionalidades Futuras
Implementar uma interface gráfica para melhorar a experiência do usuário.
Adicionar mais tipos de cartas e monstros para aumentar a variedade do jogo.
Implementar um sistema de progressão onde o jogador pode ganhar experiência e subir de nível.
Adicionar música e efeitos sonoros para tornar o jogo mais imersivo.

## Licença
Este projeto é licenciado sob a licença MIT - veja o arquivo LICENSE para mais detalhes.
