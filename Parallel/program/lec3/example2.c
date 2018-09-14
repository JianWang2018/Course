#include <stdio.h>
#include <unistd.h>

int main() 
{
  int ret;

  if ((ret = fork()) == 0) {
    printf("I am the child process, pid = %d, parent pid = %d\n", getpid(),
           getppid());
  } else {
    printf("I am the parent process, pid = %d, ret = %d\n", getpid(), ret);
  }
}
  
