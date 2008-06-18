#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

/**
 * \see INT13_A
 *
 * Do not perform bitops on signed values
 */
#define UO_WRONLY ((unsigned long) O_WRONLY)
#define UO_CREAT ((unsigned long) O_CREAT)
#define UO_EXCL ((unsigned long) O_EXCL)
#define UO_TRUNC ((unsigned long) O_TRUNC)

void FIO43_C_3() {
  /* ... */
  int fd;
  char temp_name[] = "/tmp/temp-XXXXXX";

  if (mktemp(temp_name) == (char*) NULL) {
    /* Handle Error */
  }
  fd = open(temp_name, UO_WRONLY | UO_CREAT | UO_EXCL | UO_TRUNC, 0600);
  if (fd == -1) {
    /* Handle Error */
  }
  /* ... */
}
