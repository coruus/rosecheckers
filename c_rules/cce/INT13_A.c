#include <stdlib.h>
#include <stdio.h>

int main(void) {
	int rc = 0;
	unsigned int stringify = 0x80000000;
	char buf[sizeof("256")];
	rc = snprintf(buf, sizeof(buf), "%u", stringify >> 24);
	if (rc == -1 || rc >= sizeof(buf)) /* handle error */ ;
	return 0;
}
