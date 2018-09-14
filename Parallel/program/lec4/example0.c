#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
  int i;
  int f1, f2;
  
  for (i=0;;i++) {
    if ((f1 = open ("mytmp", O_RDWR | O_CREAT, 0777)) == -1) break;
  }
  printf("number of files open (file descriptor table size) = %d\n", i);
  return 0;
}
