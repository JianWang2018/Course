/*	cputime.h

	Measures elapsed time between calls

	Robert van Engelen, 2007

	cc -DUSE_X ...
	where X is

	USE_TIMES
		Gives CPU time (user + system time)
	USE_GETRUSAGE
		Gives CPU time (user + system time)
	USE_GETTIMEOFDAY
		Gives wall-clock time
	USE_RDTSC
		Gives high resolution wall-clock time
		Intel IA32 and IA64 only
		Uses the real-time RDTSC clock, which rolls over in ~ 1 sec
		Warning: be careful when using this with multicore processors,
		since these may have RDTSC clocks per core and a context switch
		to another core gives a different readout (set the processor
		affinity)
	USE_GETHRTIME
		Gives high resolution wall-clock time
		Sun Sparc only

	Usage:

	double cputime(void)
		Returns elapsed time in seconds from previous cputime() call
*/

#ifndef CPUTIME_H
#define CPUTIME_H

#ifdef __cplusplus
extern "C" {
#endif

double cputime(void);

#ifdef __cplusplus
}
#endif

#endif
