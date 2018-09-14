#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
  struct stat buf;

  if (argc != 2) {
    printf("Usage: a.out filename\n");
    exit(0);
  }

  if (stat(argv[1], &buf) <0) {
    printf("stat %s failed (probably file does not exist).\n", argv[1]);
    exit(0);
  }
  if (S_ISUID & buf.st_mode) printf("Set-user-ID bit set on %s.\n", argv[1]);
  else printf("Set-user-ID bit NOT set on %s.\n", argv[1]);
  if (S_ISGID & buf.st_mode) printf("Set-group-ID bit set on %s.\n", argv[1]);
  else printf("Set-group-ID bit NOT set on %s.\n", argv[1]);

  return 0;
}
