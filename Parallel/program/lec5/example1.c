#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  struct stat buf;

  if (argc != 2) {
    printf("Usage: a.out filename\n");
    exit(0);
  }

  if (lstat(argv[1], &buf) <0) {
    printf("stat %s failed (probably file does not exist).\n", argv[1]);
    exit(0);
  }
  if (S_ISREG(buf.st_mode)) printf("%s is a regular file.\n", argv[1]);
  else if (S_ISDIR(buf.st_mode)) printf("%s is a directory.\n", argv[1]);
  else if (S_ISCHR(buf.st_mode)) printf("%s is a special character file.\n", 
				   argv[1]);
  else if (S_ISBLK(buf.st_mode)) printf("%s is a special block file.\n", argv[1]);
  else if (S_ISFIFO(buf.st_mode)) printf("%s is a fifo pipe.\n", argv[1]);
  else if (S_ISLNK(buf.st_mode)) printf("%s is a symbolic link.\n", argv[1]);
  else if (S_ISSOCK(buf.st_mode)) printf("%s is a socket.\n", argv[1]);
  else printf("%s is unknown type.\n", argv[1]);
  return 0;
}
