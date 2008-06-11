#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void TMP32_C() {
  char sfn[] = "/tmp/ed.XXXXXX";
  FILE *sfp;
  int fd = -1;

  if ((fd = mkstemp(sfn)) == -1 || (sfp = fdopen(fd, "w+")) == NULL) {
    if (fd != -1) {
      unlink(sfn);
      close(fd);
    }
    /* handle error condition */
  }

  unlink(sfn); /* unlink immediately */
  /* use temporary file */
  fclose(sfp);
  close(fd);
}
