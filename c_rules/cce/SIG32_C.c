#include <signal.h>
#include <stdlib.h>

enum { MAXLINE = 1024 };
volatile sig_atomic_t eflag = 0;

void handler(int signum) {
  signal(signum, handler);
  eflag = 1;
}

void log_message(char *info1, char *info2) {
  static char *buf = NULL;

  char buf0[MAXLINE];

  if (buf == NULL) {
    buf = buf0;
  }

  /* try to fit a message into buf, else
     re-allocate it on the heap, then
     log the message. */

  if (buf == buf0) {
    buf = NULL;
  }
}

int main(void) {
  signal(SIGINT, handler);
  char *info1;
  char *info2;

  /* info1 and info2 are set
     by user input here */

  while (!eflag) {
    /* main loop program code */

    log_message(info1, info2);

    /* more program code */
  }

  log_message(info1, info2);

  return 0;
}

