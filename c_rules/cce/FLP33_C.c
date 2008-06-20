#include <stdio.h>

int main(void) {
	short a = 533;
	int b = 6789;
	long c = 466438237;

	float d = a / 7.0f; /* d is 76.14286 */
	double e = b / 30.; /* e is 226.3 */
	double f = (double)c * 789; /* f is 368019768993.0 */

	printf("%f %f %f\n", d, e, f);
	return 0;
}
