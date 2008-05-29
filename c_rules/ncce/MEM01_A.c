#include <stdlib.h>

int main(void) {
	int message_type = 0;
	int value_1 = 0;
	int value_2 = 0;

	char *message = malloc(1);
	if(!message) return 1;

	if (message_type == value_1) {
		/* Process message type 1 */
		free(message);
	}
	/* ...*/
	if (message_type == value_2) {
		/* Process message type 2 */
		free(message);
	}
	return 0;
}
