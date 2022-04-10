/*
 * Passagem de parâmetros para a thread. 
 * Um inteiro pode ser recebido disfarçado de apontador.
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int N_COL, N_LIN, N_THR, N_DIA;

int **matriz_p; //matriz par
int **matriz_i; //matriz impar
int **matriz_v; //matriz velha
int **matriz_n; //matriz nova


// Funcoes que permitem a volta ao mundo
int modl(int i) {
  return (i+N_LIN) % N_LIN;
}

int modc(int i) {
  return (i+N_COL) % N_COL;
}

void* avalia_vida(void *v) {
  long int id = (long int) v;
  int vida, viz;
  //printf("Thread %d\n", id);
  for (int j = 0; j < N_COL; ++j)
  {
    // Determina condições da célula
    vida = matriz_v[id][j];
    viz = 0;
    for (int k = -1; k < 2; ++k) {
      if (matriz_v[modl(id-1)][modc(j+k)] == 1) {
        viz++;
      }
      if (matriz_v[modl(id+1)][modc(j+k)] == 1) {
        viz++;
      }
    }
    if (matriz_v[id][modc(j-1)] == 1) {
      viz++;
    }
    if (matriz_v[id][modc(j+1)] == 1) {
      viz++;
    }
    
    // Aplica Regra 23/3
    if (vida == 0 && viz == 3) {
      matriz_n[id][j] = 1;
    } else if (vida == 1 && (viz < 2 || viz > 3)) {
      matriz_n[id][j] = 0;
    } else {
      matriz_n[id][j] = matriz_v[id][j];
    }
    //printf("i=%ld; j=%d; vida=%d; viz=%d; veredito=%d\n", id, j, vida, viz, matriz_n[id][j]);

  }
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

void imprime_matriz(int ** matriz, int n, int m, char c0, char c1) {
  for (int i = 0; i < n; ++i)
  {
    for (int j = 0; j < m; ++j)
    {
      printf("%c ", (matriz[i][j]==0) ? c0 : c1 );
    }
    printf("\n");
  }
  printf("\n");
}

int main() {

  scanf("%d", &N_DIA);
  scanf("%d", &N_LIN);
  scanf("%d", &N_COL);
  N_THR = N_LIN;

  pthread_t thr[N_THR];
  
  // Cria matrizes par e impar
  matriz_p = (int**) malloc(N_LIN * sizeof(int*));
  for (int i = 0; i < N_COL; i++)
  {
    matriz_p[i] = (int*) malloc(N_COL * sizeof(int));
  }

  matriz_i = (int**) malloc(N_LIN * sizeof(int*));
  for (int i = 0; i < N_COL; i++)
  {
    matriz_i[i] = (int*) malloc(N_COL * sizeof(int));
  }
  

  // Le entrada
  recebe_matriz(matriz_p, N_LIN, N_COL);
  long int i; 
  int dia;
  

  
  // Executa Jogo da Vida
  for (dia = 0; dia < N_DIA; ++dia)
  {


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
