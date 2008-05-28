#include <stdio.h>
#include <stdlib.h>

void ENV04_A() {
  char* input = "parameters";
  char cmd[25];
  snprintf( cmd, 25, "any_exe '%s'", input);
  system( cmd);
}
