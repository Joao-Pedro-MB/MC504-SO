#include <stdio.h>

int main() {
	for(int i=0; i < 10; i++) {
		printf("(%d + 10 +- 1) mod 10 = %d/%d\n", i, (i-1+10) % 10, (i+1+10) % 10);
	}
	return 0;
}
