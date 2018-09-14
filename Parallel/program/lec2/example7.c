#include <stdlib.h>
#include <stdio.h>

int main()
{
  char name[100];
  char buf[100];

  while (fgets(buf, 100, stdin)) {
    sscanf(buf, "%s", name);
    printf("%s = %s\n", name, getenv(name));
  }
  return 0;
}
