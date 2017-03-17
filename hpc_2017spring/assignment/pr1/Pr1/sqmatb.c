#include "global.h"

#define BLKSIZE (32)

static void block_mult(const double *A, const double *B, double *C, int i, int j, int k, int n);

void
sqmat_mult(const double *A, const double *B, double *C, int n)
{
  int i, j, k, n2;

  n2 = n*n;

  for (i = 0; i < n2; i++)
    C[i] = 0.0;

  for (i = 0; i < n; i += BLKSIZE)
    for (j = 0; j < n; j += BLKSIZE)
      for (k = 0; k < n; k += BLKSIZE)
        block_mult(A, B, C, i, j, k, n);
}

static void
block_mult(const double *A, const double *B, double *C, int i, int j, int k, int n)
{
  /* TODO: implement block multiply C = A*B for blocks of size BLKSIZE*BLKSIZE
     located at C(i,j), A(i,k), B(k,j)

     You will need to handle interior blocks and boundary blocks, where
     boundary blocks occur on the right edge, bottom edge, and bottom-right
     corner, as shown below:

     BBB...B
     BBB...B
     BBB...B
     ...BBB.
     ...BBB.
     ...BBB.
     BBB...B

     Make sure the interior blocks are computed as efficiently as possible by
     limiting indexing and addressing arithmetic in loops. It is usually a good
     idea to use constants (e.g. BLKSIZE) within loops and loop bounds whenever
     possible.

     Note that in C/C++ the matrix entry M(i,j) maps to M[j*n+i] where n is the
     number of rows.

     If you prefer to program this part in Fortran: implement sqmatb.f and use
     sqmatfc.c to link the Fortran SQMAT subroutine (see Makefile)
  */

}
