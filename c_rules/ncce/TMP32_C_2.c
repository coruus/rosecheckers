#include <stdio.h>

void TMP32_C_2() {
  char* temp_file_name = NULL;
  FILE* t_file;
  /* ... */
  if (tmpnam(temp_file_name)) {
    /* temp_file_name may refer to an existing file */
    t_file = fopen(temp_file_name, "wb+");
    if (!t_file) {
      /* Handle Error */
    }
  }
  /* ... */
}

