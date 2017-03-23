/*	cputime.c

	Robert van Engelen, 2007
*/

#include "cputime.h"

#if defined(USE_TIMES)

/* Measures CPU user + system time */ 

#include <time.h>
#include <sys/times.h>
#include <unistd.h>

double
cputime(void)
{
  static double recip = 0.0;
  static long base = 0;
  struct tms buffer;
  long temp;

  times(&buffer);

  if (recip == 0.0)
    recip = 1.0 / (double)sysconf(_SC_CLK_TCK);

  temp = base;
  base = buffer.tms_utime + buffer.tms_stime;

  return (double)(base - temp) * recip;
}

#elif defined(USE_GETTIMEOFDAY)

/* Measures wall clock time */

#include <sys/time.h>

double
cputime(void)
{
  struct timeval tv;
  struct timezone tz;
  long temp_sec, temp_usec;
  static long base_sec = 0, base_usec = 0;

  gettimeofday(&tv, &tz);

  temp_sec = base_sec;
  temp_usec = base_usec;
  base_sec = tv.tv_sec;
  base_usec = tv.tv_usec;

  return (double)(base_sec - temp_sec + (base_usec - temp_usec) * 0.000001);
}

#elif defined(USE_GETRUSAGE)

/* Measures CPU user + system time */ 

#include <sys/time.h>
#include <sys/resource.h>

double
cputime(void)
{
  struct rusage buffer;
  long temp_sec, temp_usec;
  static long base_sec = 0, base_usec = 0;

  getrusage(RUSAGE_SELF, &buffer);

  temp_sec = base_sec;
  temp_usec = base_usec;
  base_sec = buffer.ru_utime.tv_sec + buffer.ru_stime.tv_sec;
  base_usec = buffer.ru_utime.tv_usec + buffer.ru_stime.tv_usec;

  return (double)(base_sec - temp_sec + (base_usec - temp_usec) * 0.000001);
}

#elif defined(USE_RDTSC)

/* Measures wall clock time in CPU ticks, but does funny things on multicore which has multiple RDTSC (need processor affinity) */

/* Added automatic calibration using gettimeofday */

#include "rdtsc.h"
#include <sys/time.h>
#include <stdio.h>

double
cputime(void)
{
  static double recip = 0.0;
  static long base = 0;
  long temp;

  if (recip == 0.0)
  {
    struct timeval tv;
    struct timezone tz;
    long base_sec, base_usec;

    temp = rdtsc();
    gettimeofday(&tv, &tz);
    base_sec = tv.tv_sec;
    base_usec = tv.tv_usec;
    do gettimeofday(&tv, &tz);
    while (1000 * (tv.tv_sec - base_sec) + (tv.tv_usec - base_usec) < 1000);
    temp = rdtsc() - temp;
    fprintf(stderr, "CPU freq. = %ld MHz\n", temp/1000);
    recip = 0.001 / (double)temp;
  }

  temp = base;
  base = rdtsc();

  return (double)(base - temp) * recip;
}

#elif defined(USE_GETHRTIME)

#include <sys/time.h>

double
cputime(void)
{
  static hrtime_t base = 0;
  hrtime_t temp;

  temp = base;
  base = gethrtime();

  return ((double)(base - temp)) * 1.0e-9;
}

#endif
