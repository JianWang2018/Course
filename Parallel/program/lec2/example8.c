#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main()
{
  int i;

  i = 0;
  while (environ[i]) 
    printf("%s\n", environ[i++]);

  setenv("MYNEW", "/home/faculty", 1);

  printf("After adding a varaible\n\n");

  printf("HOME=%s\n", getenv("HOME"));
  printf("MYNEW=%s\n", getenv("MYNEW"));
  return 0;
}
