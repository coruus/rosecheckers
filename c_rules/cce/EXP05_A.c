#include <stdlib.h>

void remove_spaces(char *str, size_t slen) {
   char *p = str;
   size_t i;
   for (i = 0; i < slen && str[i]; i++) {
      if (str[i] != ' ') *p++ = str[i];
   }
   *p = '\0';
}
