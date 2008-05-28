#include <stdio.h>

void FIO12_A() {
  FILE* file = fopen("file.ext", "r");
  char *buf = NULL;
  /* Setup file */
  setbuf(file, buf);
  /* ... */
}
