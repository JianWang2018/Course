/*  filename arrayUpdate.c            */
/*  OpenMP tutorial example           */

#include <omp.h>  /* Here's the header file for OpenMP. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>

struct timeval start, end;

int c[1000];
int d[1000];
int main( int argc, char *argv[] )
{

  int i;
#pragma omp parallel for shared(c)
  for (i=0; i<1000; i++) c[i] = 1.0;

#pragma omp parallel for shared(c, d)
  for (i=1; i<1000; i++) d[i] = c[i-1]+1.0;

#pragma omp parallel for shared(c, d)
  for (i=1; i<1000; i++) c[i] = d[i];

  for (i=0; i<1000; i++) {
    printf("c[%d] = %d\n", i, c[i]);
  }
  return 0;
}
