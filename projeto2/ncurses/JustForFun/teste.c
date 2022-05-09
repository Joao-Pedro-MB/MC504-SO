#include <stdio.h>
#include <stdlib.h>

void makeWaitingLine(int * hackers, int * serfs, char * hackers_str, char * serfs_str) {
    for (int i = 0 ; hackers[i] != NULL ; i++) {
        if (hackers[i] < 10) {
            hackers_str += 'H' + '0' +  (char) hackers[i];
        } else {
            hackers_str += 'H' + (char) hackers[i];
        }
    }

    for (int i = 0 ; serfs[i] != NULL ; i++) {
        if (serfs[i] < 10) {
            serfs_str += 'H' + '0' +  (char) serfs[i];
        } else {
            serfs_str += 'H' + (char) serfs[i];
        }
    }

    hackers_str += '\n';
    serfs_str += '\n';

}

void show_status (int progress, char* passenger_1, char* passenger_2, char* passenger_3, char* passenger_4,int* hackers,int* serfs) {

    char  hackers_str[50], serfs_str[50];
    makeMaikingLine(hackers, serfs, hackers_str, serfs_str);

    system("clear");
    printf("\n");
    printf("\n");
    printf("Hackers\n");
    printf(hackers);
    printf("\n");
    printf("Devs\n");
    printf(serfs);

    for(int x = 0; x < progress; x++) {   
        printf("=");
    }
        printf("\\_%s_%s_´T`_%s_%s_/\r", passenger_1, passenger_2, passenger_3, passenger_4);
        system("sleep 1");   
    
}

int main() {

    int times = 10;
    int oi[10] = {0,1,2,3,4,5,6,7,8,9};
         
    for(int i = 0; i <= times; i++) {
       show_status(i * 8, "H1", "S2", "H3", "S4", oi, oi);
    }
    
    
   return (EXIT_SUCCESS);    
    
}
