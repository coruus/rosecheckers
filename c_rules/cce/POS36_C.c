#include <stdlib.h>
#include <unistd.h>

int main(void) {
	/* Drop superuser privileges in correct order */

	if (setgid(getgid()) == -1) {
		/* handle error condition */
	}
	if (setuid(getuid()) == -1) {
		/* handle error condition */
	}

	return 0;
}
