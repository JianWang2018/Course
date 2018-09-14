#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static char *str1 = "OP1\n";
static char *str2 = "AB2\n";

int main()
{
  int f1;

  close(STDOUT_FILENO);  
  f1 = open ("myjunk", O_RDWR | O_CREAT | O_TRUNC, 0777);
  close(STDERR_FILENO);
  dup(f1);
  printf("print to standard output!!!, f1 = %d\n", f1); // fflush(0);
  printf("print to standard output!!!, f1 = %d\n", f1);// fflush(0);
  fprintf(stderr, "print to standard error!!!\n"); // fflush(0);
  write(f1, str1, strlen(str1)); // fflush(0);
  return 0;
}
