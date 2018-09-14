#include <stdio.h>
#include <string.h>

/* char buf[1024]; */
int main(void)
{
  char *s, buf[1024];
  int fds[2];
  int i;

  s = "hello world\n";

  for (i=0; i< 1024; i++) buf[i] = '\0';
  pipe(fds);
  close(fds[0]);
  printf("1\n");
  write(fds[1], s, strlen(s));
  printf("2\n");
  i = read(fds[0], buf, strlen(s));
  printf("3\n");
  printf("fds[0] = %d, fds[1] = %d, buf = %s, i = %d\n", fds[0], 
	 fds[1], buf, i);
  return 0;
}
