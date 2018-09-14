#include <stdio.h>
#include <stdlib.h>

FILE *fd;
char buf[256];

int main() 
{
  if ((fd = fopen("/dev/tty", "r+")) == 0) {
    printf("Cannot open /dev/tty\n");
    exit(0);
  }
  close(1);
  close(2);
  close(0);


  fprintf(fd, "TO THE SCREEN REGARDLESS OF THE I/O redirection.\n");
  fprintf(fd, "NOW only echo!!!\n");
  while (fgets(buf, 256, fd)) fprintf(fd, "%s", buf);
}
