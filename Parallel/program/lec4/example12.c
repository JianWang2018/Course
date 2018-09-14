/* implementing  "/usr/bin/ps -ef | /usr/bin/more" */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  int fds[2];
  int child[2];
  char *argv[3];
  pipe(fds);
  if (fork()== 0) {
    close(fds[1]);
    close(STDIN_FILENO); dup(fds[0]); /* redirect standard input to fds[1] */
    argv[0] = "/bin/more";
    argv[1] = NULL;           /* check how the argv array is set */
    execv(argv[0], argv);
    exit(0);


  }
  if (fork() == 0) {
    close(fds[0]);
    close(STDOUT_FILENO); dup(fds[1]);  /* redirect standard output to fds[0] */
    argv[0] = "/bin/ps";
    argv[1] = "-e"; argv[2] = NULL;
    execv(argv[0], argv);
    exit(0);

  }

  close(fds[1]);
  wait(&child[0]);
  wait(&child[0]);  
} 
