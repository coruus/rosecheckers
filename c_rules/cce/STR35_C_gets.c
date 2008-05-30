#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void STR35_C_gets() {
	char buf[BUFSIZ];
	int ch = 0;
	char *p;
	
	if (fgets(buf, sizeof(buf), stdin)) {
	  /* fgets succeeds, scan for newline character */
	  p = strchr(buf, '\n');
	  if (p) {
	    *p = '\0';
	  }
	  else {
	    /* newline not found, flush stdin to end of line */
	    while (((ch = getchar()) != '\n') && !feof(stdin) && !ferror(stdin) ) {
			if(ch == EOF)
				break;
		}
	  }
	}
	else {
	  /* fgets failed, handle error */
	}
}
