/*      gauss.c

        Gaussian Elimination

	Compile with -Dlayout, where layout is:

	JAGBYCOL
	  Use jagged array layout (array of arrays) with heap-allocated columns
	JAGBYROW
	  Use jagged array layout (array of arrays) with heap-allocated rows
	COLMAJOR
	  Use column-major layout, similar to Fortran (w/o heap allocation)
	ROWMAJOR
	  Use row-major layout, similar to C (w/o heap allocation)
	COLARRAY
	  Use column-major layout mapped to a 1D array (w/o heap allocation)
	ROWARRAY
	  Use row-major layout mapped to a 1D array (w/o heap allocation)

        Robert van Engelen, 2007-2008, 2010, 2012
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "cputime.h"

#ifdef _OPENMP
#include <omp.h>
#endif

/* Number of test runs */
#define RUNS 4

/* Maximum matrix rank */
#define N (8192)

#if defined(JAGBYCOL)
  typedef double VECTOR[N];
  typedef double* MATRIX[N];
# define A(i,j,n) a[j][i]
#elif defined(JAGBYROW)
  typedef double VECTOR[N];
  typedef double* MATRIX[N];
# define A(i,j,n) a[i][j]
#elif defined(COLMAJOR)
  typedef double VECTOR[N];
  typedef VECTOR MATRIX[N];
# define A(i,j,n) a[j][i]
#elif defined(ROWMAJOR)
  typedef double VECTOR[N];
  typedef VECTOR MATRIX[N];
# define A(i,j,n) a[i][j]
#elif defined(COLARRAY)
  typedef double VECTOR[N];
  typedef double MATRIX[N*N];
# define A(i,j,n) a[n*j+i]
#elif defined(ROWARRAY)
  typedef double VECTOR[N];
  typedef double MATRIX[N*N];
# define A(i,j,n) a[(n+1)*i+j]
#else
# error "No matrix layout definition"
#endif

typedef int INDEX[N];

void alloc_matrix(MATRIX a, int rows, int cols);
void alloc_jagged_matrix(MATRIX a, int major, int minor);
void free_matrix(MATRIX a, int rows, int cols);

void set_matrix(MATRIX a, int n);
void print_all(MATRIX a, VECTOR x, int n);

void gauss(MATRIX a, VECTOR x, int n);

MATRIX a;
VECTOR x;

int main()
{ 
  int i, n;
  double sec;

  /* n x n matrix, where n < N */
  n = 1000;

  /* Allocate column-augmented matrix */
  alloc_matrix(a, n, n+1);

  cputime();

  for (i = 0; i < RUNS; i++)
  {
    set_matrix(a, n);
    gauss(a, x, n);
  }

  sec = cputime()/RUNS;

  /* Deallocate matrix */
  free_matrix(a, n, n+1);

  /* Verify solution */
  for (i = 0; i < n; i++)
  {
    if (fabs(x[i] - 1) > n*DBL_EPSILON)
    {
      fprintf(stderr, "Error: maximum tolerance exceeded\n");
      exit(1);
    }
  }

  printf("Solved Ax=b for %d unknowns in %g seconds on "PLATFORM"\n", n, sec);

  return 0;
}

/* Allocate jagged matrix, does nothing for non-jagged matrices */
void alloc_matrix(MATRIX a, int rows, int cols)
{
  if (rows > N || cols > N)
  {
    fprintf(stderr, "Error: matrix too big\n");
    exit(1);
  }
#if defined(JAGBYCOL)
  alloc_jagged_matrix(a, cols, rows);
#elif defined(JAGBYROW)
  alloc_jagged_matrix(a, rows, cols);
#endif
}

void alloc_jagged_matrix(MATRIX a, int major, int minor)
{
#if defined(JAGBYCOL) || defined(JAGBYROW)
  int i, j;

  for (j = 0; j < major; j++)
    a[j] = NULL;

  /* Allocate jagged array */
  for (i = 0; i < major; i++)
  {
    /* Permute to increase randomnes of memory placement */
    do j = rand() % major;
    while (a[j]);

    a[j] = (double*)malloc(sizeof(double) * minor);
  }
#endif
}

/* Deallocate jagged matrix, does nothing for non-jagged matrices */
void free_matrix(MATRIX a, int rows, int cols)
{
#if defined(JAGBYCOL)
  int i;
  for (i = 0; i < cols; i++)
  { 
    free(a[i]);
    a[i] = NULL;
  }
#elif defined(JAGBYROW)
  int i;
  for (i = 0; i < rows; i++)
  { 
    free(a[i]);
    a[i] = NULL;
  }
#endif
}

/* Initialize column-augmented matrix with a pattern */
void set_matrix(MATRIX a, int n)
{
  int i, j;

  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
    {
      if (i == j)
        A(i,j,n) = 1;
      else if (i == j-1 || i == j+1)
        A(i,j,n) = 2;
      else if ((i == n-1 && j == 0) || (i == 0 && j == n-1))
        A(i,j,n) = 2;
      else
        A(i,j,n) = 0;
    }
    A(i,n,n) = 5;
  }
}

/* Print column-augmented matrix a and vector x for debugging */
void print_all(MATRIX a, VECTOR x, int n)
{
  int i, j;

  /* Matrix must be small to print properly */
  if (n > 10)
    return;

  printf("\n");
  for (j = 0; j < n; j++)
    printf("__A[%1d]", j);
  printf("|___B[]|___X[]\n");
  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
      printf("%6.2g", A(i,j,n));
    printf("|%6.2g|%6.2g\n", A(i,n,n), x[i]);
  }
}

/* Gaussian elimination of column-augmented matrix a and unknown x */
void gauss(MATRIX a, VECTOR x, int n)
{
  int i, j, k, maxloc;
  double maxval;
  INDEX idx;

  /* Init row index array */
  for (i = 0; i < n; i++)
    idx[i] = -1;

  for (k = 0; k < n; k++)
  {
    /* Find pivot element in k'th column */
    maxval = 0;
    maxloc = -1;
    for (i = 0; i < n; i++)
    {
      if (idx[i] == -1 && maxval < fabs(A(i,k,n)))
      {
        maxval = fabs(A(i,k,n));
        maxloc = i;
      }
    }

    /* Singular? */
    if (maxval < DBL_EPSILON)
    {
      fprintf(stderr, "Singular matrix\n");
      exit(1);
    }

    /* Relabel row of the k'th pivot element */
    idx[maxloc] = k;

    /* Reduce the rows, except pivot row and previous rows */
    for (i = 0; i < n; i++)
    {
      if (idx[i] == -1)
      {
	double fac = A(i,k,n)/A(maxloc,k,n);
        for (j = k; j < n+1; j++)
          A(i,j,n) = A(i,j,n) - fac * A(maxloc,j,n);
      }
    }
  }

  /* Row exchanges for A[][] and b[] */
#ifdef JAGBYROW
  {
    MATRIX tmp;

    /* Simply exchange the pointers to the rows, no data movement needed! */
    for (i = 0; i < n; i++)
      tmp[idx[i]] = a[i];
    for (i = 0; i < n; i++)
      a[i] = tmp[i];
  }
#else
  /* Iterate per column to exchange elements */
  for (j = 0; j < n+1; j++)
  {
    VECTOR tmp;

    for (i = 0; i < n; i++)
      tmp[idx[i]] = A(i,j,n);
    for (i = 0; i < n; i++)
      A(i,j,n) = tmp[i];
  }
#endif

  /* Solve x by backsubstitution */
  /* To parallelize, we need to rewrite the loop order so i-loop is innermost */
  /* Hint: see "Synchronous Shared Memory Parallel Examples" slide 26:
       DO j = n,1,-1
         X(j) = A(j,n+1) / A(j,j)
         A(1:j-1,n+1) = A(1:j-1,n+1) - A(1:j-1,j)*X(j)
       ENDDO
     and rewrite A(1:j-1,n+1) = A(1:j-1,n+1) - A(1:j-1,j)*X(j) as a parallel
     loop (note that Fortran indexing is 1..n+1, while we need 0..n)
  */
  for (i = n-1; i >= 0; i--)
  {
    /* Note: b[i] is stored in A(i,n,n) */
    double sum = A(i,n,n);

    for (j = i+1; j < n; j++)
      sum = sum - A(i,j,n) * x[j];

    x[i] = sum/A(i,i,n);
  }
}
