#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
  int pid;
  int stat;

  if (argc < 2) {
    printf("Usage: a.out command\n");
    exit(0);
  }

  if (fork() == 0) {
    close(1);
    open ("mytmp", O_RDWR | O_CREAT, 0777);
    if (execv(argv[1], &argv[1]) == -1) exit(-1);
    
    printf("abcde\n");
    
  } else {
    pid = wait(&stat);
    if (stat >> 8 == 0xff) {
      printf("command failed\n");
    } else 
      printf("child pid = %d, status = %d\n", pid, stat>>8);
  }
}
