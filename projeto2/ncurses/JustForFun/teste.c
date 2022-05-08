#include <stdio.h>
#include <stdlib.h>

void show_status (int progress, char* passenger_1, char* passenger_2, char* passenger_3, char* passenger_4,int* hackers,int* devs) {

    system("clear");
    printf("\n");
    printf("\n");
    printf("Hackers\n");
    printf("H01 H02 H03 \n");
    printf("\n");
    printf("Devs\n");
    printf("S01 S02 S03 \n");

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
