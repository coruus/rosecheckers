#include <stdlib.h>
#include <string.h>

enum { WORKSPACE_SIZE = 256 };

int main(void) {
	int dest[WORKSPACE_SIZE];
	int src[WORKSPACE_SIZE];
	/* ... */
	memcpy(dest, src, 255 * sizeof(int));
	/* ... */
}
