#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

void myfind(char *filename, char *dir)
{
  DIR *d;
  struct dirent *w;
  struct stat buf;

  char n[2000];

  //  printf("%s in %s\n", filename, dir);

  if ((d = opendir(dir)) == NULL) {
    printf("This is not supposed to happen.\n");
    exit(0);
  }

  while ((w = readdir(d)) != NULL) {
    sprintf(n, "%s/%s", dir, w->d_name);
    if (strcmp(w->d_name, filename) == 0) {
      printf("%s\n", n);
    }
    if ((strcmp(w->d_name, ".") == 0) ||
        (strcmp(w->d_name, "..")==0)) continue;

    if (lstat(n, &buf) < 0) {
      fprintf(stderr, "Cannot stat %s\n", n); 
      continue;
    }
    if (S_ISDIR(buf.st_mode)) {
      myfind(filename, n);
    }
  }
  closedir(d);
}

int main(int argc, char *argv[])
{
  struct stat buf;

  if (argc != 3) {
    printf("Usage: a.out filename directory\n");
    exit(0);
  }

  /* finding all occurance of filename under directory */

  if (lstat(argv[2], &buf) <0) {
    printf("stat %s failed (probably file does not exist).\n", argv[2]);
    exit(0);
  }

  if (!S_ISDIR(buf.st_mode)) {
    printf("%s is not a directory.\n", argv[2]);
    exit(0);
  }

  myfind (argv[1], argv[2]);
  return 0;
}
