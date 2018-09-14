/* 
 Which output is possible? 

   11111 33333  11111 33333
   22222 44444  33333 11111               
   33333 11111  22222 22222
   44444 22222  44444 44444 
*/

char buf[1024];
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  int fds[2];

  pipe(fds);
  if (fork() == 0) {
    close(fds[1]);
    printf("11111\n");
    read(fds[0], buf, 1);
    write(fds[0], buf, 1);
    printf("22222\n");
    exit(0);
  } else {
    close(fds[0]);
    printf("33333\n");
    write(fds[1], buf, 1);
    read(fds[1], buf, 1);
    printf("44444\n");
    exit(0);
  }
  return 0;
}
