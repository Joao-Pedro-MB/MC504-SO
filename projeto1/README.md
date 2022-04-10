# Project 1 - Simple Life Game emulator

## Introdução
Esse primeiro projeto tem como objetivo uma primeira introdução ao processamento multi-thread em C com a aultilização da biblioteca POSIX thread.

## Background
Como motivador da implementação buscamos relizar uma implementação simples do [jogo da vida](https://pt.wikipedia.org/wiki/Jogo_da_vida) proposto por John Conway.

## Funcionamento
Para gerar a simulação recebemos uma matriz binaria de entrada, de ate 30 linhas e 60 colunas, onde um "1" indica a presença de uma célula e "0" a ausência. Em seguida utilizamos uma thread por linha para atualizar o estado das células para cada dia. Esse ciclo de criação de threads, atualização e destruição de threads e repetido para cada iteração do loop principal, simulando os "dias" da vida.

# Funções

## avalia_vida
Recebe um ponteiro de void, que na realidade se trata de um inteiro longo disfarçado. Sua função é iterar por cada "célula" de uma determinada linha atualizando o valor da célula atual baseada em seus vizinhos.

## recebe_matriz
Recebe um ponteiro de ponteiro de inteiro, um inteiro para colunas e um inteiro para linhas. Em seguida utiliza dois laços aninhados para receber o valor de cada célula da matriz.

## imprime_matriz
Recebe um ponteiro de ponteiro de inteiro, um inteiro para colunas, um inteiro para linhas e dois caracteres. Em seguida utiliza dois laços aninhados para imprimir o valor de cada célula da matriz na saida padrão.

## pthread_create
Recebe o id da thread e a função avalia_vida para que possa ser resolvida independentemente.

## pthread_join
Combina todas as threads criadas para a criação de uma matriz atualizada.
