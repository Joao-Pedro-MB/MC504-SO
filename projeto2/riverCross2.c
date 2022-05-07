#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

//pode ignorara isso aqui eu so to tentando deixar o codigo mais bonito

pthread_barrier_t barrier;
mutex = Semaphore (1)
hackers = 0
serfs = 0
hackerQueue = Semaphore (0)
serfQueue = Semaphore (0)
local isCaptain = False

void* sobeNoBarco(void* args) {
    printf("Waiting at the barrier\n");

    // aqui passamos a esperar por 4 passageiros para subir no barco
    ptherad_barrier_wait(&barrier);

}

int main() {
    pthread_t th[10];
    int i = 0;
    pthread_barrier_init(&barrier, NULL, 4);

    for (i; i < 10 ; i++) {
        if(ptherad_create(&th[i], NULL, &routine, NULL) != 0) {
            perror("a criação da thread %d, falhou\n", i);
        }
    }



    pthread_barrier_destroy(&barrier);


    return 0;
}