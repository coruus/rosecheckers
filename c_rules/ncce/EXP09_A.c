#include <stdlib.h>

void EXP09_A() {
  size_t i;
  int** triarray = (int**) calloc(100, 4);
  if (triarray == NULL) {
    /* handle error */
  }

  for (i = 0; i < 100; i++) {
    triarray[i] = (int*) calloc(i, 4);
    if (triarray[i] == NULL) {
      /* handle error */
    }
  }
}
