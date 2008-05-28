#include <signal.h>
#include <stdlib.h>
#include <string.h>

/* Non-compliant code, e_flag is not static */

char *err_msg;

void handler(int signum) {
  volatile sig_atomic_t e_flag = 0;
  signal(signum, handler);
  e_flag = 1;
}

int main(void) {
  signal(SIGINT, handler);

  err_msg = malloc(24);
  if (err_msg == NULL) {
    /* handle error condition */
  }

  strcpy(err_msg, "No errors yet.");

  return 0;
}

