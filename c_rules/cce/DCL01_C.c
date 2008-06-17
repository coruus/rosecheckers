#include <stdlib.h>
#include <string.h>

char system_msg[100];
/* ... */
void report_error(char const *error_msg) {
	char default_msg[80];
	/* ... */
	/* Assume error_msg isn't too long */
	if (error_msg)
		strncpy(system_msg, error_msg, sizeof(system_msg));
	else
		strncpy(system_msg, default_msg, sizeof(system_msg));
	return;
}
int main(void) {
	char error_msg[80];
	/* Ensure error_msg isn't too long */
	if (strlen(error_msg) >= sizeof(system_msg)) {
		error_msg[sizeof(error_msg) - 1] = '\0';
	}
	report_error(error_msg); /* good */
 /* ... */
}
