#include "global.h"

void
sqmat_mult(const double *A, const double *B, double *C, int n)
{
  int i, j, k, n2;

  n2 = n*n;

  for (i = 0; i < n2; i++)
    C[i] = 0.0;

  for (j = 0; j < n; j++)
    for (i = 0; i < n; i++)
      for (k = 0; k < n; k++)
        C[j*n + i] = C[j*n + i] + A[k*n + i]*B[j*n + k];
}
