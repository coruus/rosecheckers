int main(void) {
	int ar[20];
	int *ip;

	for (ip = &ar[0]; ip < &ar[21]; ip++) {
	  *ip = 0;
	}
	return 0;
}
