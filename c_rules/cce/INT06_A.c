#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>

int main(int argc, char **argv) {
	long sl;
	int si;
	char *end_ptr;

	if (argc > 1) {
		errno = 0;

		sl = strtol(argv[1], &end_ptr, 10);

		if ((sl == LONG_MIN)
		 || (sl == LONG_MAX) 
		 || (end_ptr == argv[1]))
		{
			if (errno != 0) {
				perror("strtol error");
			}
			else {
				puts("error encountered during conversion");
			}
			return -1;
		}
		else if (sl > INT_MAX) {
			printf("%ld too large!\n", sl);
			return -1;
		}
		else if (sl < INT_MIN) {
			printf("%ld too small!\n", sl);
			return -1;
		}
		else if ('\0' != *end_ptr) {
			puts("extra characters on input line\n");
			return -1;
		}
		else {
			si = (int)sl;
		}
	}
	printf("%d\n", si);
	return 0;
}
