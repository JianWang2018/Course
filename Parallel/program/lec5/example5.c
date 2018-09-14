#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char cmd[100];

int main(int argc, char * argv[])
{
  struct stat statbuf;

  if (argc != 2) {
    printf("Usage: a.out filename\n");
    exit(0);
  }

  if (stat(argv[1], &statbuf) < 0) {
    perror("stat");
    exit(0);
  }

  if (!(S_IRUSR & statbuf.st_mode)) {
    if (chmod(argv[1], statbuf.st_mode | S_IRUSR) < 0) {
      perror("chmod");
      exit(0);
    }
    sprintf(cmd, "cat %s", argv[1]);
    system(cmd);
    if (chmod(argv[1], statbuf.st_mode) < 0) {
      perror("chmod1");
      exit(0);
    }
  } else {
    sprintf(cmd, "cat %s", argv[1]);
    system(cmd);
  }
}

