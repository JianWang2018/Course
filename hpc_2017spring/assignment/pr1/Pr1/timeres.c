/*	timeres.c

	Robert van Engelen, 2007
*/

#include "global.h"
#include "cputime.h"
#include "timeres.h"

double
timeres(void)
{
  unsigned long i, j;
  double secs;

  /* initialize the timer */
  (void)cputime();

  i = 0;
  do
  {
    i++;
    (void)cputime();
    for (j = 0; j < i; j++)
      ;
    secs = cputime();
  } while (secs == 0.0);

  fprintf(stderr, "It took %lu iterations to generate a nonzero time on "
    "platform "PLATFORM" compiled with "COMPILER" "OFLAGS" "MFLAGS"\n", i);

  if (i == 1)
    fprintf(stderr, "Timer resolution is less than or equal to %12.9f seconds\n", secs);
  else
    fprintf(stderr, "Timer resolution is %12.9f seconds\n", secs);

  return secs;
}
