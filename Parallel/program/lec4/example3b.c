#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

char buf[1000];

int main(int argc, char * argv[])
{
  int i;
  dup(0);
  close(0);
  while (fgets(buf, 1000, stdin)) {
    printf("%s", buf);
  }
  printf("back:\n");
  dup(3);
  close(3);
  while (fgets(buf, 1000, stdin)) {
    printf("%s", buf);
  }
}

    
