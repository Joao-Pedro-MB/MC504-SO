#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

//pode ignorara isso aqui eu so to tentando deixar o codigo mais bonito

pthread_barrier_t barrier;
sem_t mutex, hackerQueue, serfQueue;
int hackers = 0, serfs = 0;

void* embarca(char *valor, long int num) {
    printf("- %s %ld embarcou\n", valor, num+1);
}

void* rema() {
    printf("! o barco zarpou !\n\n\n");
}

void* boardHacker(void* args) {
    // espera o semafaro compartilhado ser liberado
    sem_wait(&mutex);
    int isCaptain = 0;

    hackers += 1;

    // checa se ja temos 4 hackers e libera seus lugares
    if (hackers == 4) {
        sem_post(&hackerQueue);
        sem_post(&hackerQueue);
        sem_post(&hackerQueue);
        sem_post(&hackerQueue);


        hackers = 0;
        isCaptain = 1;

    // checa se temos 2 hackers e 2 serfs e libera seus lugares
    } else if (hackers ==2 && serfs >= 2) {
        
        sem_post(&hackerQueue);
        sem_post(&hackerQueue);

        sem_post(&serfQueue);
        sem_post(&serfQueue);

        serfs -= 2;
        hackers = 0;

        isCaptain = 1;

    // caso contrario o barco n pode partir ainda, apenas libera o controle
    } else {
        sem_post(&mutex);
    }

    // entra na fila de hackers para esperar o barco
    sem_wait(&hackerQueue);

    // entra no barco e espera mais 3 threads entrarem também
    embarca("hacker", args);
    pthread_barrier_wait(&barrier);

    if (isCaptain == 1) {
        rema();
        sem_post(&mutex);
    }
}

void* boardSerf(void* args) {
    // espera o semafaro compartilhado ser liberado
    sem_wait(&mutex);
    int isCaptain = 0;

    serfs += 1;

    // checa se ja temos 4 serfs e libera seus lugares
    if (serfs == 4) {
        sem_post(&serfQueue);
        sem_post(&serfQueue);
        sem_post(&serfQueue);
        sem_post(&serfQueue);


        serfs = 0;
        isCaptain = 1;

    // checa se temos 2 hackers e 2 serfs e libera seus lugares
    } else if (serfs ==2 && hackers >= 2) {
        
        sem_post(&serfQueue);
        sem_post(&serfQueue);

        sem_post(&hackerQueue);
        sem_post(&hackerQueue);

        hackers -= 2;
        serfs = 0;

        isCaptain = 1;

    // caso contrario o barco n pode partir ainda, apenas libera o controle
    } else {
        sem_post(&mutex);
    }

    // entra na fila de serfs para esperar o barco
    sem_wait(&serfQueue);

    // entra no barco e espera mais 3 threads entrarem também
    embarca("serf", args);
    pthread_barrier_wait(&barrier);

    if (isCaptain == 1) {
        rema();
        sem_post(&mutex);
    }
}

int main() {
    int N_THREADS=20;
    pthread_t th[N_THREADS];
    int i = 0;
    long int num_serf=0, num_hack=0;
    pthread_barrier_init(&barrier, NULL, 4);
    sem_init(&mutex, 1 , 1);
    sem_init(&hackerQueue, 1 , 0);
    sem_init(&serfQueue, 1 , 0);

    for (i; i < N_THREADS ; i++) {
        int r = rand() % 6;

        if (r > 3){
            if(pthread_create(&th[i], NULL, &boardHacker, num_hack) != 0) {
            printf("a criação da thread %d, falhou\n", i);
        } else {
            num_hack++;
            printf("+ Pthread %ld hacker criada\n", num_hack);
            }
        } else {
            if(pthread_create(&th[i], NULL, &boardSerf, num_serf) != 0) {
                printf("a criação da thread %d, falhou\n", i);
            } else {
                num_serf++;
                printf("+ Pthread %ld serf criada\n", num_serf);
                }
        }
    }

    for (i = 0; i < N_THREADS ; i++) {
        if(pthread_join(th[i], NULL) != 0) {
            printf("a criação da thread %d, falhou\n", i);
        }
    }


    pthread_barrier_destroy(&barrier);


    return 0;
}