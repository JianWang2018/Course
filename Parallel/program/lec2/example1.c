#include <stdio.h>
#include <string.h>

char *str;

char aa[1000];
int main()
{
  int ii;

  str = aa;

  printf("Hello, world. ii = %d\n", ii);
  strcpy(str, "Hello, world.");
  printf("%s\n", str);
  return 0;
}

  
