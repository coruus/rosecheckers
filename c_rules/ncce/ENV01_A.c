#include <stdlib.h>
#include <string.h>

int main(void) {
	char *temp;
	char copy[16];

	temp = getenv("TEST_ENV");
	if (temp != NULL) {
		strcpy(copy, temp);
	}

	return 0;
}
