#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <errno.h>

#define SET_USER_WEIGHT 440
#define GET_USER_WEIGHT 441

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Passe um uid e um peso, por favor\n");
		return -1;
	}

	int peso = syscall(GET_USER_WEIGHT, atoi(argv[1]));

	if(peso == -1) {
		printf("Error, errno = %d\n", errno);
		return -1;
	}
	printf("usuario %d tinha peso: %d\n", atoi(argv[1]), peso);
	if(syscall(SET_USER_WEIGHT, atoi(argv[1]), atoi(argv[2])) == -1) {
		printf("Erro %d, por favor verifique o que houve de errado\n", errno);
		return -1;
	}
	peso = syscall(GET_USER_WEIGHT, atoi(argv[1]));
	if(peso == -1) {
		printf("Erro %d, por favor verifique o que houve de errado\n", errno);
		return -1;
	}
	printf("Usuario %d tem peso atual de: %d\n", atoi(argv[1]), peso);
	return 0;
}
