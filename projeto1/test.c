/*
 * Passagem de parâmetros para a thread. 
 * Um inteiro pode ser recebido disfarçado de apontador.
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define N_DIA 180
#define N_COL 60
#define N_LIN 30
#define N_THR N_LIN
#define modl(i) ((i)+N_LIN) % N_LIN // func que permite a volta no mundo
#define modc(i) ((i)+N_COL) % N_COL // func que permite a volta no mundo

int **matriz_p; //matriz par
int **matriz_i; //matriz impar
int **matriz_v; //matriz velha
int **matriz_n; //matriz nova

// aqui utilizamos um void como um long int, no entanto recomendamos
// que verifique o tamanho do pontiro void em seu ambiente para utilizar
// o tamanho correto de inteiro no casting

void* avalia_vida(void *v) {
  long int id = (long int) v;
  int vida, vizinho;
  //printf("Thread %d\n", id);
  for (int j = 0; j < N_COL; ++j)
  {
    // Determina condições da célula
    vida = matriz_v[id][j];
    vizinho = 0;
    for (int k = -1; k < 2; ++k) {
      if (matriz_v[modl(id-1)][modc(j+k)] == 1) {
        vizinho++;
      }
      if (matriz_v[modl(id+1)][modc(j+k)] == 1) {
        vizinho++;
      }
    }
    if (matriz_v[id][modc(j-1)] == 1) {
      vizinho++;
    }
    if (matriz_v[id][modc(j+1)] == 1) {
      vizinho++;
    }
    
    // Aplica Regra 23/3
    if (vida == 0 && vizinho == 3) {
      matriz_n[id][j] = 1;
    } else if (vida == 1 && (vizinho < 2 || vizinho > 3)) {
      matriz_n[id][j] = 0;
    } else {
      matriz_n[id][j] = matriz_v[id][j];
    }
    //printf("i=%ld; j=%d; vida=%d; vizinho=%d; veredito=%d\n", id, j, vida, vizinho, matriz_n[id][j]);

  }
  return NULL; 
} 

void recebe_matriz(int ** matriz,int n, int m) {
  for (int i = 0 ; i < n ; i++){
    for (int j = 0; j < m; j++) {
      scanf("%d", &(matriz[i][j]));
    }
  }
}

void imprime_matriz(int ** matriz, int n, int m, char c0, char c1) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      printf("%c ", (matriz[i][j]==0) ? c0 : c1 );
    }
    printf("\n");
  }
  printf("\n");
}

int main() {
  pthread_t thr[N_THR];
  
  // Cria matrizes par e impar
  matriz_p = malloc(N_LIN * sizeof(int*));
  for (int i = 0; i < N_COL; i++)
  {
    matriz_p[i] = malloc(N_COL * sizeof(int));
  }

  matriz_i = malloc(N_LIN * sizeof(int*));
  for (int i = 0; i < N_COL; i++)
  {
    matriz_i[i] = malloc(N_COL * sizeof(int));
  }
  

  // Le entrada
  recebe_matriz(matriz_p, N_LIN, N_COL);
  long int i; 
  int dia;
  

  // Executa Jogo da Vida
  for (dia = 0; dia < N_DIA; ++dia) {

    // Determina quem sao as matrizes velha e nova
    if (dia % 2 == 0) {
      matriz_v = matriz_p;
      matriz_n = matriz_i;
    } else {
      matriz_v = matriz_i;
      matriz_n = matriz_p;
    }

    printf("Dia %d:\n", dia);
    imprime_matriz(matriz_v, N_LIN, N_COL, '.', '@');

    // Cria threads
    for (i = 0; i < N_THR; i++)
      pthread_create(&thr[i], NULL, avalia_vida, (void*) i);

    // Mata threads
    for (i = 0; i < N_THR; i++) 
      pthread_join(thr[i], NULL);

    sleep(1);
  }
  printf("Dia %d:\n", dia);
  imprime_matriz(matriz_n, N_LIN, N_COL, '.', '@');

  return 0;
}
