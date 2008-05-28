#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

void TMP32_C_3() {
  /* ... */
  int fd;
  char temp_name[] = "/tmp/temp-XXXXXX";

  if (mktemp(temp_name) == (char*) NULL) {
    /* Handle Error */
  }
  fd = open(temp_name, O_WRONLY | O_CREAT | O_EXCL | O_TRUNC, 0600);
  if (fd == -1) {
    /* Handle Error */
  }
  /* ... */
}
