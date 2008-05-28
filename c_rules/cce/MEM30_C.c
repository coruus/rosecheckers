
#include <stdlib.h>
#include <string.h>

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
  return 0;
}
