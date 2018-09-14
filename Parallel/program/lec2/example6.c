#include <stdio.h>
extern char **environ;

int main()
{
  int i;
  i = 0;
  while (environ[i]) 
    printf("%s\n", environ[i++]);
  return 0;
}
