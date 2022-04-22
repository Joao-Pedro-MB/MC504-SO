/*
 * Problema do barbeiro dorminhoco com vários barbeiros.
 */ 
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_CLIENTES 10
#define N_BARBEIROS 2
#define N_CADEIRAS 5

sem_t sem_cadeiras;
sem_t sem_cad_barbeiro[N_BARBEIROS];
sem_t sem_cabelo_cortado[N_BARBEIROS];
sem_t sem_cliente_cadeira[N_BARBEIROS];

sem_t sem_escreve_painel, sem_le_painel;
int painel;

void* f_barbeiro(void *v) {
  int id = *(int*)v;

  while(1) {
    sem_wait(&sem_escreve_painel);
    painel = id;
    sem_post(&sem_le_painel);
    sem_wait(&sem_cliente_cadeira[id]);
    printf("Barbeiro %d cortou o cabelo de um cliente.\n", id);
    sem_post(&sem_cabelo_cortado[id]);
    sleep(random()%3);
  }
  return NULL;
}

void* f_cliente(void* v) {
  int id = *(int*) v;
  int minha_cadeira;

  sleep(random()%3);
  if (sem_trywait(&sem_cadeiras) == 0) {
    printf("Cliente %d entrou na barbearia.\n", id);
    sem_wait(&sem_le_painel);
    minha_cadeira = painel;
    sem_post(&sem_escreve_painel);
    sem_wait(&sem_cad_barbeiro[minha_cadeira]);
    printf("Cliente %d sentou na cadeira do barbeiro %d.\n", id,
	   minha_cadeira);
    sem_post(&sem_cliente_cadeira[minha_cadeira]);
    sem_post(&sem_cadeiras);
    sem_wait(&sem_cabelo_cortado[minha_cadeira]);
    sem_post(&sem_cad_barbeiro[minha_cadeira]);
    printf("Cliente %d deixou a barbearia.\n", id);
  } else
    printf("Cliente %d não entrou na barbearia.\n", id);
  return NULL;
}

int main() {
  pthread_t thr_clientes[N_CLIENTES], thr_barbeiros[N_BARBEIROS];
  int i, id_cl[N_CLIENTES], id_bar[N_BARBEIROS];

  sem_init(&sem_cadeiras, 0, 5);
  sem_init(&sem_escreve_painel, 0, 1);
  sem_init(&sem_le_painel, 0, 0);

  for (i = 0; i < N_BARBEIROS; i++) {
    sem_init(&sem_cad_barbeiro[i], 0, 1);
    sem_init(&sem_cliente_cadeira[i], 0, 0);
    sem_init(&sem_cabelo_cortado[i], 0, 0);
  }
  
  for (i = 0; i < N_CLIENTES; i++) {
    id_cl[i] = i;
    pthread_create(&thr_clientes[i], NULL, f_cliente, (void*) &id_cl[i]);
  }

  for (i = 0; i < N_BARBEIROS; i++) {
    id_bar[i] = i;
    pthread_create(&thr_barbeiros[i], NULL, f_barbeiro, (void*) &id_bar[i]);
  }
  
  for (i = 0; i < N_CLIENTES; i++) 
    pthread_join(thr_clientes[i], NULL);

  /* Barbeiros são assassinados */
  
  return 0;
}
