/*  filename arrayUpdate_II.c         */
/*  OpenMP tutorial example           */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* #include <omp.h>*/  /* Here's the header file for OpenMP. */

struct timeval start, end;

int main( int argc, char *argv[] )
{

  /*  Arguments required for executing argv[0]:    */
  /*  0                         -> (executable)    */
  /*  1                         -> arraySize       */

  double *x, *y;     /* the arrays                 */
  int    arraySize;  /* get from the command line  */
  int    i;

  /* Additional variables for arrayUpdate_II:      */
  int    privIndx;
  double privDbl;

  double startTime, elapsedTime; /* for timing */
  double clockZero = 0.0;
  int globalCount;
  
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
  for ( i = 0; i < arraySize; i++ )
    {
      x[i] = ( (double) i ) / ( i + 1000 );
    }

  globalCount = 0;

  /* Now begins the real work which we want to parallelize. */
  /* Mark the starting time. */
  if (gettimeofday(&start, NULL) < 0) {
    perror(": gettimeofday");
    exit(0);
  }
  /*  startTime = walltime( &clockZero ); */

  /* Here's the OpenMP pragma that parallelizes the for-loop. */
  /* This parallel construct has 3 private variables:         */
  /*   1) outer loop index "i" by default                     */
  /*   2) inner loop index "privIndx" by explicit declaration */
  /*   3) variable "privDbl" by explicit declaration          */
#pragma omp parallel for private( privIndx, privDbl )
  for ( i = 0; i < arraySize; i++ )
    {
      for ( privIndx = 0; privIndx < 16; privIndx++ )
	{
	  privDbl = ( (double) privIndx ) / 16;
	    y[i] =
	      sin( exp( cos( - exp( sin(x[i]) ) ) ) ) + cos( privDbl );
	    globalCount ++;
	}
    }

  /* Work's done. Get the elapsed wall time. */
  if (gettimeofday(&end, NULL) < 0) {
    perror(": gettimeofday");
    exit(0);
  }
  elapsedTime = (end.tv_sec-start.tv_sec)*1000.0
    + (end.tv_usec - start.tv_usec) / 1000.0;

  /*
    elapsedTime = walltime( &startTime );*/

  fprintf(stdout, "\nthe global counter should equal %d, globalCount = %d\n", 
	  arraySize * 16, globalCount);

  /* Print the wall time and terminate. */
  fprintf( stdout, "\nwall time = %.2f ms for array size = %.0e\n",
	   elapsedTime, (double) arraySize );

  fprintf( stdout, "\nProgram successfully terminated.\n\n\a" );

  return(0);
}

