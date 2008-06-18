#include <stdlib.h>
#include <stdio.h>

int main(void) {
	char c = (signed char) 200;
	int i = 1000;
	printf("i/c = %d\n", i/c);
	return 0;
}
