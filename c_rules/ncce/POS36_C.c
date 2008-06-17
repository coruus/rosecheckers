#include <stdlib.h>
#include <unistd.h>

int main(void) {
	/* Drop superuser privileges in incorrect order */

	if (setuid(getuid()) == -1) {
		/* handle error condition */
	}
	if (setgid(getgid()) == -1) {
		/* handle error condition */
	}

	/* It is still possible to regain group privileges due to incorrect
	 * relinquishment order */
	return 0;
}
