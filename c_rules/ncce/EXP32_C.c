#include <stdlib.h>
#include <stdio.h>

int main(void) {
	static volatile int **ipp;
	static int *ip;
	static volatile int i = 0;

	printf("i = %d.\n", i);

	ipp = &ip; /* produces warnings in modern compilers */
	ipp = (int**) &ip; /* constraint violation */
	*ipp = &i; /* valid */
	if (*ip != 0) { /* valid */
	  /* ... */
	}
	return 0;
}
