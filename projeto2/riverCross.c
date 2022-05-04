#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

pthread_mutex_t mutex;

int hackers = 0, devs = 0, boat = 0;
p * passanger_in_boat[4];

//estrutura passageiro que marca o grupo e se o passageiro eh capitao
typedef struct passanger {
  char role[10];
  int is_captain;
}p;

void * join_line(void * pass) {

  // trava o mutex para o passageiro poder incrementar as filas sem concorrencia
  pthread_mutex_lock(&mutex);
  p *passanger = (p *) pass;

  // incrementa a fila a qual o passageiro pertence
  if (passanger->role == "hack") {
    hackers += 1;
  } else {
    devs += 1;
  }

  // incrementa a fila geral do barco
  boat += 1;
  passanger_in_boat[boat] = passanger;

  // checa se o barco pode partir
  if (devs == 4 || hackers == 4 || (devs == 2 && hackers == 2)) {
    passanger->is_captain = 1;
    row_boat(passanger);

  // caso o barco nao possa partir e esteja lotado, a thread sai do barco
  } else if (boat == 4) {
    boat -= 1;

    if (passanger->role == "hack") {
      hackers -= 1;
    } else {
      devs -= 1;
    }

    passanger_in_boat[boat] = NULL;
  }

  pthread_mutex_unlock(&mutex);
}

void row_boat() {
    
}

int main() {

  // recebe 10 passageiros e inicializa o mutex
  int n_passangers = 10;
  pthread_mutex_init(&mutex, NULL);

  // cria uma thread para cada passageiro
  pthread_t thr[10];

  // inicializa cada thread e atribui aleatoriamente um papel ao passageiro
  for (int i = 0; i < n_passangers; i++) {
    
    // define papel do passageiro
    int r = rand() % 6;
    char * role;
    if (r <= 3) {
      role = "devs";
    } else {
      role = "hack";
    }

    // inicializa a struct passageiro e a thread
    p p = {.role = role, .is_captain = 0};
    pthread_create(&thr[i], NULL, join_line, (void*) &p);
  }

  // get returns of each thread
  for (int i = 0; i < n_passangers; i++)
    pthread_join(thr[i], NULL);

  pthread_mutex_destroy(&mutex);
  return 0;
}