#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void ENV04_A(char* input) {
   pid_t pid;
   char *const args[3] = {"", input, NULL};
   char *const envs[1] = {NULL};

   if ((pid = fork()) == -1) {
      puts("fork error");
   } 
   else if (pid == 0) {
      if (execve("/usr/bin/any_exe", args, envs) == -1) {
         puts("Error executing any_exe");
    }
  }
}
