#include "global.h"

/* Use Sun High Performance library */
#ifdef USE_SUNPERF
#include <sunperf.h>
#endif

void
sqmat_mult(const double *A, const double *B, double *C, int n)
{
#ifdef USE_SUNPERF
  /* TODO: call the dgemm BLAS library */
#else
  /* can't use BLAS */
  fprintf(stderr, "Not compiled with BLAS!\n");
  exit(1);
#endif
}

