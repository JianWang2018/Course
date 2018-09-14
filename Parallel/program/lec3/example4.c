#include <stdio.h>
#include <stdlib.h>
int main()
{
  int stat;
  int pid;

  if (fork() == 0) {
    printf("I am the child process, pid = %d\n", getpid());
    exit(10);
  } else {
    pid = wait(&stat);
    getchar();
    printf("child pid = %d, status = %d\n", pid, stat>>8);
  }
}
