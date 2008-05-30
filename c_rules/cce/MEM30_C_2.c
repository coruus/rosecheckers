#include <stdlib.h>
#include <string.h>

/* tricky good example, buff re-assigned after free */

int main(int argc, char *argv[]) {
  char *buff;
  int BUFSIZE = 50;

  buff = (char*) malloc(BUFSIZE);
  if (!buff) {
     /* handle error condition */
  }
  /* ... */
  strncpy(buff, argv[1], BUFSIZE-1);
  /* ... */
  free(buff);
  buff = (char*) malloc(BUFSIZE);
  if (!buff) {
     /* handle error condition */
  }
  return 0;
}
