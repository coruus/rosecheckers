#include <stdlib.h>
#include <stdio.h>

int main(int argc, char ** argv) {
	int si;

	if (argc > 1) {
		si = atoi(argv[1]);
	}

	printf("%d\n",si);

	return 0;
}
