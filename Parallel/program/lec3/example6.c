#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main() 
{
  char * p;
  int pid;
  int status;

  p = malloc(10);

  strcpy(p, "abcd");

  if ((pid=fork()) == 0) {
    strcpy(p, "kkkkk");
    printf("child process, pid = %d, p = %lld, content(p) = %s\n", getpid(),
           (long long int)p, p);
    exit(0);
  } 
  wait(&status);
  getchar();
  printf("pid = %d, p = %lld, content(p) = %s\n", getpid(), (long long int)p, p);
}
  
