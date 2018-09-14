/* example for sprintf and sscanf */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[])
{
  char command[100];
  char cmd[100];
  char buf[100];
  FILE *fd;

  int pid;
  char tty[20];
  int hr, min, sec;
  int n;
  char cmdd[20];
  strcpy(command, "ps");

  sprintf(cmd, "%s > tmp000", command);
  printf("executing %s\n", cmd);
  system(cmd);

  fd = fopen("tmp000", "r");
  fgets(buf, 100, fd);  /* skip the first line */

  fgets(buf, 100, fd);
  while(!feof(fd)) {
    sscanf(buf, "%d %[^/]/%d %d:%d:%d %s", &pid, tty, &n, &hr, &min, &sec, cmdd);
    printf("pid = %d, tty = %s, n = %d, min=%d, sec = %d, hr = %d, cmd = %s\n", pid, tty, n, min, sec, hr, cmdd);
    fgets(buf, 100, fd);
  }
  return 0;
}

  
  
  
