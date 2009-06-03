/*
 * Copyright (c) 2007 Carnegie Mellon University.
 * All rights reserved.

 * Permission to use this software and its documentation for any purpose is hereby granted,
 * provided that the above copyright notice appear and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that the name of CMU not
 * be used in advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.
 *
 * CMU DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL CMU BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, RISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "util.h"
#include "cpp.util.h"

void SIG00();
void SIG01();
void SIG30();
void SIG31();
void SIG33();
void SIG34();

void SIG() {
  SIG00();
  SIG01();
  SIG30();
  SIG31();
  SIG33();
  SIG34();
}

volatile sig_atomic_t SIG00_sig1 = 0;
volatile sig_atomic_t SIG00_sig2 = 0;

void SIG00_handler(int signum) {
  if (signum == SIGUSR1) {
    SIG00_sig1 = 1;
  }
  else if (SIG00_sig1) {
    SIG00_sig2 = 1;
  }
}

void SIG00() {
  if (signal(SIGUSR1, SIG00_handler) == SIG_ERR) {
    /* handle error */
  }
  if (signal(SIGUSR2, SIG00_handler) == SIG_ERR) {
    /* handler error */
  }

  while (SIG00_sig2 == 0) {
    /* do nothing or give up CPU for a while */
  }

  /* ... */

  return;
}

void SIG01_handler(int signum) {
  if (signal(signum, SIG01_handler) == SIG_ERR) {
    
  }
  /* handle signal */
}

void SIG01() {
  if (signal(SIGUSR1, SIG01_handler) == SIG_ERR) {}
}

enum { SIG30_MAXLINE = 1024 };
char *SIG30_info = NULL;

void SIG30_log_message(void) {
  /* something */
}

void SIG30_handler(int signum) {
  SIG30_log_message();
  free(SIG30_info);
  SIG30_info = NULL;
}

void SIG30(void) {
  if (signal(SIGINT, SIG30_handler) == SIG_ERR) {}
  SIG30_info = (char*)malloc(SIG30_MAXLINE);
  if (SIG30_info == NULL) {
    /* Handle Error */
  }

  while (1) {
    /* main loop program code */

    SIG30_log_message();

    /* more program code */
  }
}


char *SIG31_err_msg;
enum { SIG31_MAX_MSG_SIZE = 24 };

void SIG31_handler(int signum) {
  strcpy(SIG31_err_msg, "SIGINT encountered.");
}

void SIG31() {
  if (signal(SIGINT, SIG31_handler) == SIG_ERR) {}

  SIG31_err_msg = (char *)malloc(SIG31_MAX_MSG_SIZE);
  if (SIG31_err_msg == NULL) {
    /* handle error condition */
  }
  strcpy(SIG31_err_msg, "No errors yet.");

  /* main code loop */

  return;
}

void SIG33_log_msg(int signum) {
  /* log error message in some asynchronous-safe manner */
}

void SIG33_handler(int signum) {
  /* do some handling specific to SIGINT */
  if (raise(SIGUSR1)!=0) {}
}

void SIG33() {
  if (signal(SIGUSR1, SIG33_log_msg) != SIG_ERR) {}
  if (signal(SIGINT, SIG33_handler) != SIG_ERR) {}

  /* program code */
  if (raise(SIGINT) != 0) {}
  /* more code */

  return;
}

void SIG34_handler(int signum) {
  if (signal(signum, SIG34_handler) == SIG_ERR) {}
  /* handle signal */
}

void SIG34() {
  if (signal(SIGUSR1, SIG34_handler) == SIG_ERR) {}
}


