#include <stdio.h>

int main(void) {
	short a = 533;
	int b = 6789;
	long c = 466438237;

	float d = a / 7; /* d is 76.0 */
	double e = b / 30; /* e is 226.0 */
	double f = c * 789; /*  f may be negative due to overflow */

	printf("%f %f %f\n", d, e, f);
	return 0;
}
