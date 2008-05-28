#include <stdio.h>

void TMP32_C() {
  /* ... */
  FILE *tempfile = tmpfile();
  if (tempfile == NULL) {
    /* handle error condition */
  }
  /* ... */
}
