#include <limits.h>

int main(void) {
	int sl1 = 1;
	int sl2 = 0;
	int result;

	if ( (sl2 == 0) || ( (sl1 == INT_MIN) && (sl2 == -1) ) ) {
	  /* handle error condition */
	}
	sl2 = sl1 / sl2;
	if ( (sl2 == 0) || ( (sl1 == INT_MIN) && (sl2 == -1) ) ) {
	  /* handle error condition */
	}
	result = sl1 % sl2;
	return result;

}
