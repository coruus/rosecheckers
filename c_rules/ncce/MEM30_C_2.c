#include <stdlib.h>
#include <string.h>

/* tricky bad example, buff accessed via assignment */

char* foo() {
  char *buff;
  char *buff2;
  const int BUFSIZE = 50;

  buff = (char*) malloc(BUFSIZE);
  if (!buff) {
     /* handle error condition */
  }
  /* ... */
  free(buff);
  /* ... */
  buff2 = buff; // bad, accesses freed ptr
  return buff2;
}
