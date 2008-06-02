#include <stdlib.h>
#include <string.h>

enum {NTBS_SIZE = 256};

int main(void) {
	char ntbs[NTBS_SIZE];
	char source[NTBS_SIZE];
	memset(source, 0xf, sizeof(source));
	strncpy(ntbs, source, sizeof(ntbs));
	return 0;
}
