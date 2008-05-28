#include <signal.h>
#include <stdlib.h>
#include <string.h>

char *err_msg;

void handler(int signum) {
  signal(signum, handler);
  strcpy(err_msg, "SIGINT encountered.");
}

int main(void) {
  signal(SIGINT, handler);

  err_msg = malloc(24);
  if (err_msg == NULL) {
    /* handle error condition */
  }
  strcpy(err_msg, "No errors yet.");

  /* main code loop */

  return 0;
}

