#include <stdio.h>

void FIO07_A() {
  FILE* fptr = fopen("file.ext", "r");
  if (fptr == NULL) {
    /* handle open error */
  }

  /* read data */

  if (fseek(fptr, 0L, SEEK_SET) != 0) {
    /* handle repositioning error */
  }

  /* continue */
}
