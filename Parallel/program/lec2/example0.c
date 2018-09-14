#include <stdio.h>
#include <stdlib.h>

int main()
{
  int i, j;
  char a, b;

  printf("a = %c, b = %c\n", ({a='a';b='b';}), b);

  {typeof(i) a = b;}

  printf("a = %c, b = %c\n", a, b);  
  exit(1);
}

