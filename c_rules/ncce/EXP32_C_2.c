#include <stdlib.h>

int main(void) {
	volatile int i = 0;
	int *ip = (int*) &i;
	*ip = 3;
	return 0;
}
