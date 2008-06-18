#include <stdlib.h>

char *copy(size_t n, char const *str) {
  int i;
  char *p = (char *)malloc(n);
  if (p == NULL) {
    /* Handle malloc failure */
  }
  for ( i = 0; i < n; ++i ) {
    p[i] = *str++;
  }
  return p;
}

/* ... */

int main(void) {
	char str[] = "hi there";
	char *p = copy(sizeof(str), str);
	return 0;
}
