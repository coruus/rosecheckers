#include <stdlib.h>
#include <string.h>

int main(void) {
	char *temp;
	char *copy = NULL;

	if ((temp = getenv("TEST_ENV")) != NULL) {
		copy = (char *)malloc(strlen(temp) + 1);
		if (copy != NULL) {
			strcpy(copy, temp);
		}
		else {
			/* handle error condition */
			return 1;
		}
	}
	return 0;
}
