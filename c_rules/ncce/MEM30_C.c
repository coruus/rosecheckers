#include <stdlib.h>
#include <string.h>

/* typical bad example, access buff after free */

int main(int argc, char *argv[]) {
  char *buff;
  const int BUFSIZE = 50;

  buff = (char*) malloc(BUFSIZE);
  if (!buff) {
     /* handle error condition */
  }
  /* ... */
  free(buff);
  /* ... */
  strncpy(buff, argv[1], BUFSIZE-1);
  return 0;
}
