#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

char cmd[100];

int main(int argc, char * argv[])
{
  struct stat statbuf;
  char c;

  if (argc != 2) {
    printf("Usage: a.out filename\n");
    exit(0);
  }

  if (stat(argv[1], &statbuf) < 0) {
    perror("stat");
    exit(0);
  }

  if (!(S_IWUSR & statbuf.st_mode)) {
    printf("a.out: remove write-protected regular file `%s'?", argv[1]);
    c = getchar();
    if ((c == 'n') || (c == 'N')) {
      printf("File not removed.\n");
      exit(0);
    }

    if (chmod(argv[1], statbuf.st_mode | S_IWUSR) < 0) {
      perror("chmod");
      exit(0);
    }
    sprintf(cmd, "rm %s", argv[1]);
    system(cmd);
    printf("File %s removed.\n", argv[1]);
  }
}
