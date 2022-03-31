/*
 * Passagem de parâmetros para a thread. 
 * Um inteiro pode ser recebido disfarçado de apontador.
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_THR 10

void* avalia_vida(void *v) {
  long int id = (long int) v;
  printf("Thread %ld\n", id);  
  return NULL; 
} 

void recebe_matriz(int ** matriz,int n, int m) {
    for (int i = 0 ; i < n ; i++){
        for (int j = 0; j < m; j++)
        {
            scanf("%d", &(matriz[i][j]));
        }
        
    }

}

int main() {
  pthread_t thr[N_THR];
  int **matriz;
  matriz = malloc(N_THR * sizeof(int*));
  for (int i = 0; i < N_THR; i++)
  {
    matriz[i] = malloc(N_THR * sizeof(int));
  }
  

  recebe_matriz(matriz, 10,10);
  long int i;

  for (i = 0; i < N_THR; i++)
    pthread_create(&thr[i], NULL, avalia_vida, (void*) i);

  for (i = 0; i < N_THR; i++) 
    pthread_join(thr[i], NULL);

  return 0;
}