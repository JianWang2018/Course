/*	rdtsc.h

	RDTSC asm code

	Robert van Engelen, 2007
*/

#ifndef RDTSC_H
#define RDTSC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

__inline__ uint64_t rdtsc() {
  uint32_t lo, hi;
  __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
  return (uint64_t)hi << 32 | lo;
}

#ifdef __cplusplus
}
#endif

#endif
