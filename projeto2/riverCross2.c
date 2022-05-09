#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

typedef long int li;
int const N_THREADS=20;
void animation();

// Queue struct and functions
typedef struct queue {
    int size;
    int ini;
    int *q;
} queue;

// Queue Constructor
void q_init(queue *q) {
    q->ini = 0;
    q->size = 0;
    q->q = (int*) malloc(N_THREADS * sizeof(int));
}

int q_at(queue *q, int idx) {
    return q->q[(q->ini+idx)%N_THREADS];
}

void q_print(queue *q) {
    printf("FILA: ");
    for (int i = 0; i < q->size; ++i)
    {
        printf("%d  ", q_at(q,i));
    }
    printf("\n");
}

// Add num to end of queue
void q_push(queue *q, int num) {
    q->q[(q->ini+q->size)%N_THREADS] = num;
    q->size = q->size+1;
    q_print(q);
}

// Remove num from beginning of queue
void q_pop(queue *q) {
    q->ini = (q->ini+1)%N_THREADS;
    q->size = q->size-1;
    q_print(q);
}

// Getters
int* q_getq(queue *q) {
    return q->q;
}

int q_getsize(queue *q) {
    return q->size;
}

// Clear queue
void q_clear(queue *q) {
    q->ini=0;
    q->size=0;
}

// Queues for animation
queue q_hackers, q_serfs, q_barco;

// Arrays for animation
char barco_tipo[4];
int barco_nums[4];

// Barreira da travessoa
pthread_barrier_t barrier;

// Mutexes de controle e filas
sem_t mutex, hackerQueue, serfQueue, hackerQueueEdit, serfQueueEdit, barcoQueueEdit;

// Contadores de hackers e serfs (controlados por mutex)
int hackers = 0, serfs = 0;

// Tripulantes embarcam
void* embarca(char *valor, long int num) {
    printf("- %s %ld embarcou\n", valor, num+1);


    sem_wait(&barcoQueueEdit);
    int idx = q_getsize(&q_barco);
    barco_nums[idx] = num+1;
    barco_tipo[idx] = (strcmp(valor,"serf")==0)? 'S':'H';
    q_push(&q_barco, (li)(num + 1));
    animation();
    sem_post(&barcoQueueEdit);
}

// Capitao rema
void* rema(long int num) {
    printf("! Capitão %ld zarpou o barco !\n\n\n", num+1);
    for (int i = 0; i < 4; ++i)
    {
        printf("%c%02d ", barco_tipo[i], barco_nums[i]);
    } printf("\n");
    animation();
    //Reinicializa array
    q_clear(&q_barco);
}


// Embarque do hacker
void* boardHacker(void* args) {
    // espera o semaforo compartilhado ser liberado
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
    embarca("hacker", (li) args);

    pthread_barrier_wait(&barrier);

    if (isCaptain == 1) {
        rema((li) args);
        sem_post(&mutex);
    }
}

// Embarque do serf
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
    embarca("serf", (li) args);

    pthread_barrier_wait(&barrier);

    if (isCaptain == 1) {
        rema((li) args);
        sem_post(&mutex);
    }
}

void dev2str(char type, int num, char *str) {
    if (num < 10) {
            str += type + '0' +  (char) num + ' ' + '\0';
        } else {
            str += type + (char) num + ' ' + '\0';
        }
}

void makeWaitingLine(int * hackers_ints, int * serfs_ints, char * hackers_str, char * serfs_str) {
    for (int i = 0 ; i < hackers ; i++) {
        if (hackers_ints[i] < 10) {
            hackers_str += 'H' + '0' +  (char) hackers_ints[i] + ' ';
        } else {
            hackers_str += 'H' + (char) hackers_ints[i] + ' ';
        }
    }

    for (int i = 0 ; i < serfs ; i++) {
        if (serfs_ints[i] < 10) {
            serfs_str += 'S' + '0' +  (char) serfs_ints[i] + ' ';
        } else {
            serfs_str += 'S' + (char) serfs_ints[i] + ' ';
        }
    }

    hackers_str += '\n';
    serfs_str += '\n';

}

void show_status (int progress, char* passenger_1, char* passenger_2, char* passenger_3, char* passenger_4,int* hackers_ints,int* serfs_ints) {

    char  hackers_str[50], serfs_str[50];
    makeWaitingLine(hackers_ints, serfs_ints, hackers_str, serfs_str);

    system("clear");
    printf("\n");
    printf("\n");
    printf("Hackers\n");
    printf(hackers_str);
    printf("\n");
    printf("Devs\n");
    printf(serfs_str);

    for(int x = 0; x < progress; x++) {   
        printf("=");
    }
        printf("\\_%s_%s_´T`_%s_%s_/\r", passenger_1, passenger_2, passenger_3, passenger_4);
        system("sleep 1");   
    
}

void animation() {
    int times = 10;

    char p1[10], p2[10], p3[10], p4[10];
    
    dev2str(barco_tipo[0], barco_nums[0], p1);
    dev2str(barco_tipo[1], barco_nums[1], p2);
    dev2str(barco_tipo[2], barco_nums[2], p3);
    dev2str(barco_tipo[3], barco_nums[3], p4);
    
    for(int i = 0; i <= times; i++) {
       show_status(i * 8, p1, p2, p3, p4, q_getq(&q_hackers), q_getq(&q_serfs));
    }
}

int main() {
    pthread_t th[N_THREADS];
    int i = 0;
    long int num_serf=0, num_hack=0;
    pthread_barrier_init(&barrier, NULL, 4);
    sem_init(&mutex, 1 , 1);
    sem_init(&hackerQueue, 1 , 0);
    sem_init(&serfQueue, 1 , 0);
    sem_init(&hackerQueueEdit, 1 , 1);
    sem_init(&serfQueueEdit, 1 , 1);
    sem_init(&barcoQueueEdit, 1 , 1);
    time_t t;
    srand((unsigned) time(&t));

    q_init(&q_serfs);
    q_init(&q_hackers);
    q_init(&q_barco);


    for (i; i < N_THREADS ; i++) {
        
        int r = rand() % 6;

        if (r > 2){
            if(pthread_create(&th[i], NULL, &boardHacker, (void*) num_hack) != 0) {
            printf("a criação da thread %d, falhou\n", i);
        } else {
            sem_wait(&hackerQueueEdit);
            q_push(&q_hackers, (li)(num_hack + 1));
            sem_post(&hackerQueueEdit);
            num_hack++;
            animation();
            printf("+ Pthread hacker %ld criada\n", num_hack);
            }
        } else {
            if(pthread_create(&th[i], NULL, &boardSerf, (void*) num_serf) != 0) {
                printf("a criação da thread %d, falhou\n", i);
            } else {
                sem_wait(&serfQueueEdit);
                q_push(&q_serfs, (li)(num_serf + 1));
                sem_post(&serfQueueEdit);
                num_serf++;
                animation();
                printf("+ Pthread serf %ld criada\n", num_serf);
                }
        }
    }

    if (num_serf % 2 == 0) {
        for (i = 0; i < N_THREADS ; i++) {
            if(pthread_join(th[i], NULL) != 0) {
                printf("a criação da thread %d, falhou\n", i);
            }
        }
    } else {
        sleep(2);
        printf("Não foi possível transportar todos os hackers/serfs.\n");
    }


    pthread_barrier_destroy(&barrier);


    return 0;
}