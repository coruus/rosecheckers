#include <stdlib.h>

void EXP09_A() {
  /* assuming 32-bit pointer, 32-bit integer */
  size_t i;
  int ** triarray = calloc(100, 4);
  if (triarray == NULL) {
    /* handle error */
  }

  for (i = 0; i < 100; i++) {
    triarray[i] = calloc(i, 4);
    if (triarray[i] == NULL) {
      /* handle error */
    }
  }
}
