#include <signal.h>

volatile sig_atomic_t sig1 = 0;
volatile sig_atomic_t sig2 = 0;

void sig1_handler(int signum) {
  sig1 = 1;
}

void sig2_handler(int signum) {
  sig2 = 1;
}

int main(void) {
  signal(SIGUSR1, sig1_handler);
  signal(SIGUSR2, SIG_IGN);

  while (sig1 == 0) {
    /* do nothing or give up CPU for a while */
  }

  signal(SIGUSR2, sig2_handler);
  while (sig2 == 0) {
    /* do nothing or give up CPU for a while */
  }

  /* ... */

  return 0;
}
