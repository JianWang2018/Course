#include <stdio.h>
#include <stdlib.h>

char buf[100];
char cmd[100];
char na[1000][100];
int count;

int main(int argc, char *argv[])
{
  char dummy[100];
  char name[100], mon[100], host[100];
  int i;
  int date;
  FILE *fd;

  if (argc != 2) {
    printf("Usage: a.out machine\n");
    exit(0);
  }

  sprintf(cmd, "ssh -q %s who > /tmp/tmp0120", argv[1]);
  system(cmd);

  system("touch /tmp/tmp0120");

  printf("On %s:\n", argv[1]);

  if ((fd = fopen("/tmp/tmp0120", "r")) == NULL) {
    printf("Something is wrong.\n");
    exit(0);
  }
  fgets(buf, 100, fd);
  count = 0;

  while (!feof(fd)) {
    sscanf(buf, "%s %s %s %d %s (%[^)])", name, dummy, mon, &date, dummy, host);
    for (i=0; i<count; i++) if (strcmp(name, na[i]) == 0) break;
    if (i == count) {
      printf("  %12s from %20s on %s %2d\n", name, host, mon, date);
      strcpy(na[count++], name);
    }
    fgets(buf, 100, fd);
  }
  fclose(fd);
  system("rm /tmp/tmp0120");
  return 0;
} 
