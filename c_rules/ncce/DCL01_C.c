#include <stdlib.h>
#include <string.h>

char msg[100];
/* ... */
void report_error(char const *error_msg) {
	char msg[80];
	/* ... */
	/* Assume error_msg isn't too long */
	strncpy(msg, error_msg, sizeof(msg));
	/* ... */
	return;
}
int main(void) {
	char error_msg[80];
	/* Ensure error_msg isn't too long */
	if (strlen(error_msg) >= sizeof(msg)) {
		error_msg[sizeof(error_msg) - 1] = '\0';
	}
	report_error(error_msg); /* good */
 /* ... */
}
