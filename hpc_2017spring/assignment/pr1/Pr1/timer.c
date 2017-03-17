#include "global.h"
#include "cputime.h"
#include "timeres.h"

/* We plan to run benchmarks for at least 0.1 seconds */
#define MINSECS (0.1)

int main()
{
  double res;

  /* Report timer resolution and timer precision for MINSECS bench runs */
  res = timeres();
  fprintf(stderr, "With MINSEC=%g timing precision is at least %d digit(s)\n", MINSECS, (int)log10(MINSECS/res));

  return 0;
}
