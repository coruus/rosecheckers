#include <stdio.h>

int main(void) {
	short a = 533;
	int b = 6789;
	long c = 466438237;

	float d = (float) a;
	double e = (double) b;
	double f = (double) c;

	d /= 7; /* d is 76.14286 */
	e /= 30; /* e is 226.3 */
	f *= 789; /* f is 368019768993.0 */
	return 0;
}
