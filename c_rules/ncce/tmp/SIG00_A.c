#include <signal.h>

volatile sig_atomic_t sig1 = 0;
volatile sig_atomic_t sig2 = 0;

void handler(int signum) {
  if (sig1) {
     sig2 = 1;
  }
  if (signum == SIGUSR1) {
    sig1 = 1;
  }
}

int main(void) {
  signal(SIGUSR1, handler);
  signal(SIGUSR2, handler);

  while (sig2 == 0) {
    /* do nothing or give up CPU for a while */
  }

  /* ... */

  return 0;
}
