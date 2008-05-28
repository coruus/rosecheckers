#include <signal.h>
#include <stdlib.h>
#include <string.h>

/* Ambiguous: Compliant or Non-compliant code? e_flag accessed (not assigned) */

char *err_msg;

volatile sig_atomic_t e_flag = 0;
volatile sig_atomic_t e_flag_value = 1;

void handler(int signum) {
  signal(signum, handler);
  e_flag_value = e_flag;
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

