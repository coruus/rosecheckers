#include <stdio.h>

void FIO07_A() {
  FILE* fptr = fopen("file.ext", "r");
  if (fptr == NULL) {
    /* handle open error */
  }

  /* read data */

  rewind(fptr); // use fseek() instead!

  /* continue */
}
