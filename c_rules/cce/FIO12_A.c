#include <stdio.h>

void FIO12_A() {
  FILE* file = fopen("filename", "r");
  char *buf = NULL;
  /* Setup file */
  if (setvbuf(file, buf, buf ? _IOFBF : _IONBF, BUFSIZ) != 0) {
    /* Handle error */
  }
  /* ... */
}
