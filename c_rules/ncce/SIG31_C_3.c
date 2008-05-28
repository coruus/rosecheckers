#include <signal.h>
#include <stdlib.h>
#include <string.h>

/* Non-compliant code, e_flag is not volatile */

char *err_msg;

sig_atomic_t e_flag = 0;

void handler(int signum) {
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

  /* main code loop */
  if (e_flag) {
    strcpy(err_msg, "SIGINT received.");
  }

  return 0;
}

