#include <string.h>

int main(void) {
	const int vals[3] = {3, 4, 5};
	memset(vals, 0, sizeof(vals));
	return 0;
}
