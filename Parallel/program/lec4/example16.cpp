#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

using namespace std;

static char *str1 = "OP1\n";
static char *str2 = "AB2\n";

int main()
{
  int i=16;
  close(STDIN_FILENO);
  cin >> i;
  printf(" i = %d\n", i);
  return 0;
}
