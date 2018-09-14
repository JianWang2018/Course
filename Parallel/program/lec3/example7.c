#include <stdio.h>
#include <unistd.h>

void mysimplesystem(char *str)
{
  int pid, stat;

  if (fork() == 0) {
    if (execl(str, str, 0) == -1) exit(-1);
  }
  
  pid = wait(&stat);
  if (stat >> 8 == 0xff) {
    printf("command %s failed\n", str);
  } else 
    printf("command %s executed, status = %d\n", str, stat>>8);
}

int main() 
{
  char buf[100];
  char cmd[100];

  while (gets(buf)) {
    sscanf(buf, "%s", cmd);
    mysimplesystem(cmd);
  }
}
