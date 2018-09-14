#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int mypid = 10;

int mygetpid()
{
  return mypid;
}

int main(int argc, char *argv[])
{
  int i;

  if (strcmp(argv[1], "1") == 0) {
    for (i=0; i<100000000; i++) mygetpid();
    printf("100000000 mygetpid\n");
  } else {
    for (i=0; i<100000000; i++) getpid();
    printf("100000000 getpid\n");
  }
  return 0;
}
