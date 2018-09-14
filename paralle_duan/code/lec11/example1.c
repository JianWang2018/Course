/*  filename arrayUpdate.c            */
/*  OpenMP tutorial example           */

#include <omp.h>  /* Here's the header file for OpenMP. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>

struct timeval start, end;

int main( int argc, char *argv[] )
{

  /*  Arguments required for executing argv[0]:    */
  /*  0                         -> (executable)    */
  /*  1                         -> arraySize       */

  double *x, *y;     /* the arrays                 */
  int    arraySize;  /* get from the command line  */
  int    i;

  double startTime, elapsedTime; /* for timing */
  double clockZero = 0.0;

  /* Check the command line arguments. */
  if (argc != 2)
    {
      fprintf( stdout, "usage:  %s  arraySize\n", argv[0] );
      exit(-1);
    }

  arraySize = (int) atof( argv[1] );
  
  /* Allocate memory for the arrays. */
  x = (double *) malloc( (size_t) (  arraySize * sizeof(double) ) );
  y = (double *) malloc( (size_t) (  arraySize * sizeof(double) ) );

  /* Initialize x with some junk. */
#pragma omp parallel for
  for ( i = 0; i < arraySize; i++ )
    {
      x[i] = ( (double) i ) / ( i + 1000 );
    }

  /* Now begins the real work which we want to parallelize. */
  /* Mark the starting time. */
  if (gettimeofday(&start, NULL) < 0) {
    perror(": gettimeofday");
    exit(0);
  }


  /* Here's the OpenMP pragma that parallelizes the for-loop. */
#pragma omp parallel for
  for ( i = 0; i < arraySize; i++ )
    {
      y[i] = sin( exp( cos( - exp( sin(x[i]) ) ) ) );
    }

  /* Work's done. Get the elapsed wall time. */
  if (gettimeofday(&end, NULL) < 0) {
    perror(": gettimeofday");
    exit(0);
  }

  /* Print the wall time and terminate. */
  fprintf( stdout, "\nwall time = %.2f ms for array size = %.0e\n",
	   (end.tv_sec-start.tv_sec)*1000.0
         + (end.tv_usec - start.tv_usec) / 1000.0, (double) arraySize );

  fprintf( stdout, "\nProgram successfully terminated.\n\n\a" );

  return(0);
}
