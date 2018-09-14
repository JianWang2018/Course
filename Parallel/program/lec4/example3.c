#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static char *str1 = "OP1\n";
static char *str2 = "AB2\n";

int main()
{
  close(STDOUT_FILENO);  
  /* write(STDOUT_FILENO, str1, 1); */
  printf("print to standard output!!!\n");
  fprintf(stderr, "print to standard error!!!\n");
  return 0;
}
