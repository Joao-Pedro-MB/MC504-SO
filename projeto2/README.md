# Project 1 - Simple Life Game emulator

## Introducao
Nesse segundo projeto buscamos implementar uma solução animada para o problema do rio proposto no livro "Little Book of Semaphores" escrito por Allen B. Downey e colaboradores.

## Background

O problema é enunciado da seguinte forma:

  EM Redmond, Washington, existe um barco que é usado tanto por hackers quanto desenvolvedores de código para cruzar um rio local. O barco comporta exatamente 4 pessoas e não zarpará com mais ou menos pessoas que o estipulado. Para garantir a segurança dos passageiros, não é permitido embarcarem 1 hacker com 3 desenvolvedores e vice-versa, qualquer outra combinação é segura.

  Para cada thread que embarcar deve ser invocado uma função embarca(). Deve-se garantir que todas as threads chamem embarca() antes que as threads do próximo barco a invoquem. E a última thread que embarcar será o capitão e terá que zarpar o barco.

## Funcionamento
Para ralizarmos a animação recebemos a entrada na forma de .... Em seguida calculamos as saídas com o auxílio de 3 semáforos e uma barreira, implementados pela biblioteca POSIX threads em C. Por fim, conforme executamoes a solução também imprimimos o estado atualizado da animação ASCII na tela.

# Funções

## boardSerf
Recebe um ponteiro de void, que na realidade se trata de um inteiro longo disfarcado. Sua funcao eh iterar por cada "celula" de uma determinada linha atualizando o valor da celula atual baseada em seus vizinhos

## boardHacker
Recebe um ponteiro de ponteiro de inteiro, um inteiro para colunas e um inteiro para linhas. Em seguida utiliza dois lacos aninhados para receber o valor de cada celula da matriz

## embarca
Recebe um ponteiro de ponteiro de inteiro, um inteiro para colunas, um inteiro para linhas e dois caracteres. Em seguida utiliza dois lacos aninhados para imprimir o valor de cada celula da matriz na saida padrao

## rema
Recebe o id da thread e a funcao avalia_vida para que possa ser resolvida independentemente

## pthread_join
Combina todas as threads criadas para a criacao de uma matriz atualizada

## pthread_create

## pthread_barrier_init

## pthread_barrier_wait

## pthread_barrier_destroy

## sem_init

## sem_wait


# Video
[explanation](https://drive.google.com/file/d/1WePjLv1pqwplN58hir4nxVfK5NUlvHhc/view?usp=sharing)
