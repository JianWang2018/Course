/*	sqmatfc.c

	Wrapper to call Fortran SQMAT subroutine

	Robert van Engelen, 2007
*/

#include "global.h"

/* Fortran SQMULT subroutine */
void sqmult_(const double *A, const double *B, double *C, int *n);

void
sqmat_mult(const double *A, const double *B, double *C, int n)
{
  sqmult_(A, B, C, &n);
}
