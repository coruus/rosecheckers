#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void incorrect_password(char const *user) {
  /* user names are restricted to 256 characters or less */
  static char const *msg_format 
    = "%s could not be authenticated.\n";
  size_t len = strlen(user) + sizeof(msg_format);
  char *msg = (char *) malloc(len);
  if (!msg) {
    /* handle error condition */
  }
  snprintf(msg, len, msg_format, user);
  fprintf(stderr, msg);
  free(msg);
  msg = NULL;
}
