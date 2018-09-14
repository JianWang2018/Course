#include <stdio.h>
#include <string.h>

char *str;

int main()
{
  int ii;

  ii = 2;
  printf("Hello, world. ii = %d\n", ii);
  strcpy(str, "Hello, world.");
  printf("%s\n", str);
  return 0;
}

  
