#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
  int pid;
  int stat;

  if (argc < 2) {
    printf("Usage: a.out command\n");
    exit(0);
  }

  if (fork() == 0) {
    if (execv(argv[1], &argv[1]) == -1) exit(19);
    
    printf("abcde\n");
    
  } else {
    pid = wait(&stat);
    if (stat >> 8 == 0xff) {
      printf("command failed\n");
    } else 
      printf("child pid = %d, status = %d\n", pid, stat>>8);
  }
}
