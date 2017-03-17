/*	bench.c

	Benchmark wrapper for sqmat_mult()

	Robert van Engelen, 2007-2008
*/

#include "global.h"
#include "cputime.h"
#include "timeres.h"

/* Number of floating point ops per matrix entry */
#define FLOP (2)

/* Minimum number of runs to time */
#define MINRUNS (2)

/* Minimum time required to run: double number of runs until min time is met */
/* Watch out for timer rollover! */
#define MINSECS (0.1)

/* Maximum dimension of square matrix */
#define MAXDIMS (1000)

/* The routine to benchmark */
extern void sqmat_mult(const double *A, const double *B, double *C, int n);

/* Square matrix dimension test sizes, include power of 2 +/- 1 */
int sqmat_test_size[] = { 10, 15, 16, 17, 20, 30, 31, 32, 33, 40, 50, 60, 63, 64, 65, 70, 80, 90, 100, 127, 128, 129, 200, 255, 256, 257, 300, 400, 500, 511, 512, 513, 600, 700, 800, 900, 1000 };

#define NUMSIZES (sizeof(sqmat_test_size)/sizeof(int))

static double A[MAXDIMS * MAXDIMS],
              B[MAXDIMS * MAXDIMS],
              C[MAXDIMS * MAXDIMS];

void sqmat_rand(double *A, int n);
double sqmat_mflops(const double *A, const double *B, double *C, int n);
int sqmat_check(const double *A, const double *B, const double *C, int n);

int
main()
{
  int i;
  double res;
  time_t clock = time(NULL);

  /* Report timer resolution and determine MFlop/s precision */
  res = timeres();
  fprintf(stderr, "Timing precision is at least %d digit(s)\n", (int)log10(MINSECS/res));

  sqmat_rand(A, MAXDIMS);
  sqmat_rand(B, MAXDIMS);
  
  printf("# Timestamp = %s"
         "# Platform  = "PLATFORM"\n"
         "# Compiler  = "COMPILER"\n"
         "# Options   = "OFLAGS" "MFLAGS"\n\n"
         "# Dim    MFLOPS\n", ctime(&clock));

  for (i = 0; i < NUMSIZES; i++)
  {
    double mflops;
    int n = sqmat_test_size[i];

    mflops = sqmat_mflops(A, B, C, n);

    if (!sqmat_check(A, B, C, n))
      exit(1);

    printf("%5d %9.2f\n", n, mflops);
  }

  return 0;
}

void
sqmat_rand(double *A, int n)
{
  int i;
  int n2 = n*n;

  for (i = 0; i < n2; i++)
    A[i] = drand48();
}

int
sqmat_check(const double *A, const double *B, const double *C, int n)
{
  int sample, n2per20;

  n2per20 = n*n / 20; /* check 5% of entries */

  for (sample = 1; sample < n2per20; sample++)
  {
    int i, j, k;
    double prod, errb = 0.0, ddot = 0.0;

    i = rand() % n;
    j = rand() % n;

    for (k = 0; k < n; k++)
    {
      prod = A[k*n + i] * B[j*n + k];
      ddot += prod;
      errb += fabs(prod);
    }

    if (fabs(ddot - C[j*n + i]) > 3.0 * n * n * n * errb * DBL_EPSILON)
    {
      fprintf(stderr, "Max error tolerance exceeded at [%d,%d]\n", i, j);
      return 0;
    }
  }

  return 1;
}

double
sqmat_mflops(const double *A, const double *B, double *C, int n)
{
  int runs;
  double secs, mflops;

  runs = MINRUNS;
  for (;;)
  {
    int i;

    (void)cputime();

    for (i = 0; i < runs; i++)
      sqmat_mult(A, B, C, n);

    secs = cputime();

    if (secs >= MINSECS)
      break;

    runs *= 2;
  }

  mflops = (double)FLOP * (double)runs * (double)n * (double)n * (double)n / (1.e06 * secs);

  return mflops;
}

