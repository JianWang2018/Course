#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

int main()
{
  time_t t;

  fprintf(stderr, "Time by Date :\n  ");
  system("date");

  fprintf(stderr, "Time by the time/ctime system calls :\n  ");
  t = time(NULL);
  fprintf(stderr, "%s\n", ctime(&t));
  return 0;
}
