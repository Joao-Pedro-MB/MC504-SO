# Project 1 - Simple Life Game emulator

## Introducao
Esse primeiro projeto tem como objetivo uma primeira introducao ao processamento multi-theread em C com a aultilizacao da biblioteca POSIX thread.

## Background
Como motivador da implementacao buscamos relizar uma implementacao simples do [jogo da vida](https://pt.wikipedia.org/wiki/Jogo_da_vida) proposto por John Conway.

## Funcionamento
Para gerar a simulacao recebemos uma matriz binaria de entrada, de ate 30 linhas e 60 colunas, onde um "1" indica a presenca de uma celula e "0" a ausencia. Em seguida utilizamos uma thread por linha para atualizar o estado das celulas para cada dia. Esse ciclo de criacao de threads, atualizacao e destruicao de threads e repetido para cada iteracao do loop principal, simulando os "dias" da vida.

# Funcoes

## avalia_vida
Recebe um ponteiro de void, que na realidade se trata de um inteiro longo disfarcado. Sua funcao eh iterar por cada "celula" de uma determinada linha atualizando o valor da celula atual baseada em seus vizinhos

## recebe_matriz
Recebe um ponteiro de ponteiro de inteiro, um inteiro para colunas e um inteiro para linhas. Em seguida utiliza dois lacos aninhados para receber o valor de cada celula da matriz

## imprime_matriz
Recebe um ponteiro de ponteiro de inteiro, um inteiro para colunas, um inteiro para linhas e dois caracteres. Em seguida utiliza dois lacos aninhados para imprimir o valor de cada celula da matriz na saida padrao

## pthread_create
Recebe o id da thread e a funcao avalia_vida para que possa ser resolvida independentemente

## pthread_join
Combina todas as threads criadas para a criacao de uma matriz atualizada
