#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

char buf[1000];

void echo()
{
  while (fgets(buf, 1000, stdin)) {
    printf("%s", buf);
  }
}

int main(int argc, char * argv[])
{
  int i;

  if (argc == 1) 
    echo();
  else {
    for (i=1; i<argc; i++) {
      close(0);
      open(argv[i], O_RDWR);
      echo();
    }
  }
}

    
