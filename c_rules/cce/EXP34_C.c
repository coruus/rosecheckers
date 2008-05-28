#include <string.h>
#include <stdlib.h>

const int SIZE_MAX = 50;

void EXP34_C() {
  char* input_str = "Hi, there!";
  /* ... */
  size_t size = strlen(input_str);
  if (size == SIZE_MAX) { /* test for limit of size_t */
    /* Handle Error */
  }
  char* str = (char*) malloc(size+1);
  if (str == NULL) {
    /* Handle Allocation Error */
  }
  strcpy(str, input_str);
  /* ... */
  free(str);
}
