
#include <stdlib.h>

void EXP09_A() {
  size_t i;
  int **triarray = (int**) calloc(100, sizeof(int *));

  if (!triarray) {
    /* handle error */
  }

  for (i = 0; i < 100; i++) {
    triarray[i] = (int*) calloc(i, sizeof(int));
    if (!triarray[i]) {
      /* handle error */
    }
  }
}
