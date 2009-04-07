#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  /* ... */
  char * prog_name = (char*) malloc(strlen(argv[0])+1);
  if (prog_name != NULL) {
    strcpy(prog_name, argv[0]);
  }
  else {
    /* Couldn't get the memory - recover */
  }
  /* ... */
  return 0;
}
