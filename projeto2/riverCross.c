#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>\

sem_t mutex, serf_Q, hacker_Q, boatload;

struct passanger {
    char role[10];
    int is_captain;
};

void receive_passanger() {
    
};

void boat() {
    
};

void row_boat() {
    
};

int main() {
    int n_passangers = 0;

    // read number of passangers
    scanf("%d", &n_passangers);

    // vreates an array of threads with the size equal num,ber os passangers
    pthread_t thr[n_passangers];

    // for each thread, receive the passanger role
    for (int i = 0; i < n_passangers; i++)
      pthread_create(&thr[i], NULL, receive_passanger, (void*) i);    
    

    return 0;
};