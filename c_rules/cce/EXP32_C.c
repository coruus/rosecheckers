#include <stdlib.h>
#include <stdio.h>

int main(void) {
	static volatile int **ipp;
	static volatile int *ip;
	static volatile int i = 0;

	printf("i = %d.\n", i);

	ipp = &ip;
	*ipp = &i;
	if (*ip != 0) {
	  /* ... */
	}

	return 0;
}
