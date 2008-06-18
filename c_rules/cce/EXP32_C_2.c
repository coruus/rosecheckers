#include <stdlib.h>

int main(void) {
	volatile int i = 0;
	volatile int *ip = &i;
	*ip = 3;
	return 0;
}
