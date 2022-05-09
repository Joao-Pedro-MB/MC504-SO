# Project 2 - River Cross problem (Little Book of Semaphores)

## Introdução
Nesse segundo projeto, buscamos implementar uma solução animada para o problema da travessia do rio proposto no livro "Little Book of Semaphores" escrito por Allen B. Downey e colaboradores.

## Background

O problema é enunciado da seguinte forma:

  Em Redmond, Washington, existe um barco que é usado tanto por hackers quanto desenvolvedores de código para cruzar um rio local. O barco comporta exatamente 4 pessoas e não zarpará com mais ou menos pessoas que o estipulado. Para garantir a segurança dos passageiros, não é permitido embarcarem 1 hacker com 3 desenvolvedores e vice-versa, qualquer outra combinação é segura.

  Para cada thread que embarcar, a função embarca() deve ser invocada. Deve-se garantir que todas as threads chamem embarca() antes que as threads do próximo barco a invoquem. E uma das threads que embarcar será o capitão e terá que zarpar o barco.

## Funcionamento
Para realizarmos a animação, recebemos a entrada na forma de .... Em seguida, calculamos as saídas com o auxílio de 3 semáforos e uma barreira, implementados pela biblioteca POSIX threads em C. Por fim, conforme executamos a solução, também imprimimos o estado atualizado da animação ASCII na tela.

Durante a execução do código, percebemos que podem acontecer furos nas filas de threads para números grandes de threads, o que prejudica a justiça da implementação. No entanto, devido a natureza do problema não consideramos que isso comprometa a solução, uma vez que as threads são executadas uma única vez, e a thread que foi passada não deve demorar mais do que 3 iterações (travessias de barco) para ser atendida em simulações para até 200 threads.

# Funções

## boardSerf
Embarca um desenvolvedor e checa se a fila possui condições necessárias para zarpar o barco.

## boardHacker
Embarca um hacker e checa se a fila possui condições necessárias para zarpar o barco.

## embarca
Cria a animação de embarque e efetiva o embarque do indivíduo

## rema
Inicia a animação do barco partindo e efetiva a saída do barco e seus passageiros

## pthread_join
Combina todas as threads criadas para a criacao de uma matriz atualizada

## pthread_create
Cria uma thread nova para a execução de código

## pthread_barrier_init
Inicia uma barreira de threads com um valor específico de threads para liberação

## pthread_barrier_wait
Espera até que uma determinada barreira tenha o numero de threads necessário para continuar

## pthread_barrier_destroy
Destroi o barreira e limpa o espaço alocado para ela em memória

## sem_init
Inicia um semáfaro com um número específico

## sem_wait
Espera até que um dado semáfaro seja liberado


# Video
[explanation](https://drive.google.com/file/d/1WePjLv1pqwplN58hir4nxVfK5NUlvHhc/view?usp=sharing)
