#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

main(int argc, char *argv[])
{
  int i;
  struct stat buf;

  if (argc != 2) {
    printf("Usage: a.out filename\n");
    exit(0);
  }

  if (stat(argv[1], &buf) <0) {
    printf("stat %s failed\n", argv[1]);
    exit(0);
  }
  if (S_ISREG(buf.st_mode)) printf("-");
  else if (S_ISDIR(buf.st_mode)) printf("d");
  else if (S_ISCHR(buf.st_mode)) printf("c", 
				   argv[1]);
  else if (S_ISBLK(buf.st_mode)) printf("b");
  else if (S_ISFIFO(buf.st_mode)) printf("f");
  else if (S_ISLNK(buf.st_mode)) printf("l");
  else if (S_ISSOCK(buf.st_mode)) printf("s");
  else printf("u");
  
  if (S_IRUSR & buf.st_mode) printf("r");
  else printf("-");
  if (S_IWUSR & buf.st_mode) printf("w");
  else printf("-");
  if (S_IXUSR & buf.st_mode) printf("x");
  else printf("-");

  if (S_IRGRP & buf.st_mode) printf("r");
  else printf("-");
  if (S_IWGRP & buf.st_mode) printf("w");
  else printf("-");
  if (S_IXGRP & buf.st_mode) printf("x");
  else printf("-");
  
  if (S_IROTH & buf.st_mode) printf("r");
  else printf("-");
  if (S_IWOTH & buf.st_mode) printf("w");
  else printf("-");
  if (S_IXOTH & buf.st_mode) printf("x");
  else printf("-");

  printf("  ");
  printf("%d", buf.st_nlink);
  printf(" ");
  printf("%d", buf.st_uid);
  printf(" ");
  printf("%d", buf.st_gid);

  printf("%13d ", buf.st_size);

  printf("%s", ctime(&(buf.st_mtime))+4);
  printf(" %s\n", argv[1]);
}
