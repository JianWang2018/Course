#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

static char *str1 = "OP1\n";
static char *str2 = "AB2\n";

int main()
{
  int f1, f2;
  

  f1 = open ("mytmp", O_RDWR | O_CREAT, 0777);
  f2 = open ("mytmp", O_RDWR | O_CREAT, 0777);

  printf("f1 = %d, f2 = %d\n", f1, f2);
  write(f1, str1, strlen(str1));
  write(f2, str2, strlen(str2));

  return 0;
}
