#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>

enum { MAXLINE = 1024 };
static jmp_buf env;

void handler(int signum) {
  signal(signum, handler);
  longjmp(env, 1);
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

/*** VULNERABILITY IF SIGINT RAISED HERE ***/

  if (buf == buf0) {
    buf = NULL;
  }
}

int main(void) {
  signal(SIGINT, handler);
  char *info1;
  char *info2;

  /* info1 and info2 are set by user input here */

  if (setjmp(env) == 0) {
    while (1) {
      /* main loop program code */
      log_message(info1, info2);
      /* more program code */
    }
  }
  else {
    log_message(info1, info2);
  }

  return 0;
}

