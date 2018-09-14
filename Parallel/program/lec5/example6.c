#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <utime.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int i;
  struct stat statbuf;
  struct utimbuf timebuf;
  
  if (argc != 3) {
    printf("Usage: a.out file1 file2\n");
    exit(1);
  }
  
  if (stat(argv[2], &statbuf) == -1) {
    printf("File %s not there.", argv[2]);
    exit(2);
  }

  if (stat(argv[1], &statbuf) == -1) {
    printf("File %s not there.", argv[1]);
    exit(2);
  }

  timebuf.actime = statbuf.st_atime;
  timebuf.modtime = statbuf.st_mtime;
  
  if (utime(argv[2], &timebuf) < 0) {
    printf("utime failed.\n");
    exit(3);
  }
}
