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

// Semaforo evita que a impressao (printf) seja usada por mais de uma thread ao mesmo tempo
sem_t sem_animation;

// Queue struct and functions
typedef struct queue {
    int size;
    int ini;
    int *q;
} queue;

// Queues for animation
queue q_hackers, q_serfs, q_barco;

// Queue Constructor
void q_init(queue *q) {
    q->ini = 0;
    q->size = 0;
    q->q = (int*) malloc(N_THREADS * sizeof(int));
}

int q_at(queue *q, int idx) {
    return q->q[(q->ini+idx)%N_THREADS];
}

// Add num to end of queue
void q_push(queue *q, int num) {
    q->q[(q->ini+q->size)%N_THREADS] = num;
    q->size = q->size+1;
}

// Remove num from beginning of queue
void q_pop(queue *q) {
    q->ini = (q->ini+1)%N_THREADS;
    q->size = q->size-1;
}

void q_remove(queue *q, int num) {
    int i = 0;
    while(q_at(q, i) != num) i++;
    q->q[(q->ini+i)%N_THREADS] = q->q[q->ini];
    q_pop(q);
}

// Getter
int q_getsize(queue *q) {
    return q->size;
}

// Clear queue
void q_clear(queue *q) {
    q->ini=0;
    q->size=0;
}

// Arrays for animation
char barco_tipo[4];
int barco_nums[4];

// Barreira da travessia
pthread_barrier_t barrier;

// Mutexes de controle e filas
sem_t 
    mutex, // controle da edicao das variaveis hackers e serfs
    hackerQueue, serfQueue, // "Filas" para limitar acesso a funcao embarque
    hackerQueueEdit, serfQueueEdit, // Limita (encapsula) a edicao dos dados de array usados pra impressao
    barcoQueueEdit;

// Contadores de hackers e serfs (controlados por mutex)
int hackers = 0, serfs = 0;

// caracteres para facilitar a impressao
char *z_ptr, *ln_ptr,
    *space_ptr,  *h_ptr,
    *s_ptr, *e_ptr;

// Tripulantes embarcam
void* embarca(char *valor, long int num) {

    sem_wait(&barcoQueueEdit);
    sem_wait(&sem_animation);

    // Sobe no barco
    int idx = q_getsize(&q_barco);
    barco_nums[idx] = num+1;
    barco_tipo[idx] = (strcmp(valor,"serf")==0)? 'S':'H';
    q_push(&q_barco, (li)(num + 1));
    
    animation(0);
    sem_post(&sem_animation);
    sem_post(&barcoQueueEdit);
}

// Capitao rema
void* rema(long int num) {
    
    sem_wait(&barcoQueueEdit);

    // Tira os tripulantes da terra firme
    sem_wait(&serfQueueEdit);
    sem_wait(&hackerQueueEdit);
    for (int i = 0; i < 4; ++i)
    {
        if (barco_tipo[i] == 'H') {
            q_remove(&q_hackers, barco_nums[i]);
        } else {
            q_remove(&q_serfs, barco_nums[i]);
        }
    }
    sem_post(&serfQueueEdit);
    sem_post(&hackerQueueEdit);

    sem_wait(&sem_animation);
    animation(1);
    sem_post(&sem_animation);
    
    //Reinicializa array
    q_clear(&q_barco);
    for (int i = 0; i < 4; ++i)
    {
        barco_nums[i] = 0;
        barco_tipo[i] = 'E';
    }
    
    sem_post(&barcoQueueEdit);
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


        hackers -= 4;
        isCaptain = 1;

    // checa se temos 2 hackers e 2 serfs e libera seus lugares
    } else if (hackers ==2 && serfs >= 2) {
        
        sem_post(&hackerQueue);
        sem_post(&hackerQueue);

        sem_post(&serfQueue);
        sem_post(&serfQueue);

        serfs -= 2;
        hackers -= 2;

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


        serfs -= 4;
        isCaptain = 1;

    // checa se temos 2 hackers e 2 serfs e libera seus lugares
    } else if (serfs ==2 && hackers >= 2) {
        
        sem_post(&serfQueue);
        sem_post(&serfQueue);

        sem_post(&hackerQueue);
        sem_post(&hackerQueue);

        hackers -= 2;
        serfs -= 2;

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

    char* num_ptr = malloc(sizeof(char));

    char *type_ptr;

    strcat(str, space_ptr);

    if (type == *h_ptr) {
        type_ptr = h_ptr;
    } else if (type == *s_ptr) {
        type_ptr = s_ptr;
    } else {
        type_ptr = e_ptr;
    }

    strcat(str, type_ptr);
    if (num < 10) {
        strcat(str, z_ptr);
        *num_ptr = (char) num + '0';
        strcat(str, num_ptr);
     } else {
        *num_ptr = (char) num/10 + '0';
        strcat(str, num_ptr);
        *num_ptr = (char) num%10 + '0';
        strcat(str, num_ptr);
     }

    
    strcat(str, space_ptr);
}

void makeWaitingLine(char * hackers_str, char * serfs_str) {

    char* num_ptr = malloc(sizeof(char));


    sem_wait(&hackerQueueEdit);
    for (int i = 0 ; i < q_getsize(&q_hackers) ; i++) {
     


        strcat(hackers_str, h_ptr);
        
        if (q_at(&q_hackers, i) < 10) {
            
            strcat(hackers_str, z_ptr);
            *num_ptr = (char) q_at(&q_hackers, i) + '0';
            strcat(hackers_str, num_ptr);
            
        } else {
            *num_ptr = (char) q_at(&q_hackers, i)/10 + '0';
            strcat(hackers_str, num_ptr);

            *num_ptr = (char) q_at(&q_hackers, i)%10 + '0';
            strcat(hackers_str, num_ptr);
        }

        
        strcat(hackers_str, space_ptr);
    }
    sem_post(&hackerQueueEdit);

    sem_wait(&serfQueueEdit);
    for (int i = 0 ; i < q_getsize(&q_serfs) ; i++) {
        strcat(serfs_str, s_ptr);
        
        if (q_at(&q_serfs, i) < 10) {
            
            strcat(serfs_str, z_ptr);
            *num_ptr = (char) q_at(&q_serfs, i) + '0';
            strcat(serfs_str, num_ptr);
            
        } else {
            *num_ptr = (char) q_at(&q_serfs, i)/10 + '0';
            strcat(serfs_str, num_ptr);

            *num_ptr = (char) q_at(&q_serfs, i)%10 + '0';
            strcat(serfs_str, num_ptr);
        }

        
        strcat(serfs_str, space_ptr);
    }
    sem_post(&serfQueueEdit);

    strcat(hackers_str, ln_ptr);
    strcat(serfs_str, ln_ptr);


}

void show_status (int progress, char* passenger_1, char* passenger_2, char* passenger_3, char* passenger_4, int cross) {

    char *hackers_str = (char*) malloc(50*sizeof(char)); 
    char *serfs_str  =  (char*) malloc(50*sizeof(char)); 
    makeWaitingLine(hackers_str, serfs_str);

    system("clear");
    
    printf("Hackers\n");
    printf("%s", hackers_str);
    printf("\n");
    printf("Serfs\n");
    printf("%s", serfs_str);
    printf("\n");
    printf("\n");

    if (cross) {
        for(int x = 0; x < progress; x++) {   
            printf("=");
        }
        
    }
    
    printf("\\_%s_%s_´T`_%s_%s_/\n", passenger_1, passenger_2, passenger_3, passenger_4);
    if (cross) {
        system("sleep 0.2");
    } else {
        system("sleep 1"); 
    }
      
    
}

void animation(int cross) {
    int times = 10;

    char *p1 = malloc(10 * sizeof(char));
    char *p2 = malloc(10 * sizeof(char));
    char *p3 = malloc(10 * sizeof(char));
    char *p4 = malloc(10 * sizeof(char));

    dev2str(barco_tipo[0], barco_nums[0], p1);
    dev2str(barco_tipo[1], barco_nums[1], p2);
    dev2str(barco_tipo[2], barco_nums[2], p3);
    dev2str(barco_tipo[3], barco_nums[3], p4);
    
    if (cross) {
        for(int i = 0; i <= times; i++) {
            show_status(i * 8, p1, p2, p3, p4, cross);
        }
    } else {
        show_status(0, p1, p2, p3, p4, cross);
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
    sem_init(&sem_animation, 1 , 1);
    time_t t;
    srand((unsigned) time(&t));

    q_init(&q_serfs);
    q_init(&q_hackers);
    q_init(&q_barco);

    space_ptr = (char*) malloc(sizeof(char));
    h_ptr = (char*) malloc(sizeof(char));
    s_ptr = (char*) malloc(sizeof(char));
    e_ptr = (char*) malloc(sizeof(char));
    ln_ptr = (char*) malloc(sizeof(char));
    z_ptr = (char*) malloc(sizeof(char));
    *space_ptr = ' ';
    *h_ptr = 'H';
    *s_ptr = 'S';
    *e_ptr = 'E';
    *ln_ptr = '\n';
    *z_ptr = '0';

    for (int i = 0; i < 4; ++i)
    {
        barco_nums[i] = 0;
        barco_tipo[i] = 'E';
    }


    for (i; i < N_THREADS ; i++) {
        
        int r = rand() % 6;

        if (r > 2){
            if(pthread_create(&th[i], NULL, &boardHacker, (void*) num_hack) != 0) {
        } else {
            sem_wait(&hackerQueueEdit);
            q_push(&q_hackers, (li)(num_hack + 1));
            sem_post(&hackerQueueEdit);
            num_hack++;
            sem_wait(&sem_animation);
            animation(0);
            sem_post(&sem_animation);
            }
        } else {
            if(pthread_create(&th[i], NULL, &boardSerf, (void*) num_serf) != 0) {
            } else {
                sem_wait(&serfQueueEdit);
                q_push(&q_serfs, (li)(num_serf + 1));
                sem_post(&serfQueueEdit);
                num_serf++;
                sem_wait(&sem_animation);
                animation(0);
                sem_post(&sem_animation);
                }
        }
    }

    if (num_serf % 2 == 0) {
        for (i = 0; i < N_THREADS ; i++) {
            if(pthread_join(th[i], NULL) != 0) {
            }
        }


    } else {
        sleep(20);
        printf("Não foi possível transportar todos os hackers/serfs.\n");
    }

    

    pthread_barrier_destroy(&barrier);


    return 0;
}