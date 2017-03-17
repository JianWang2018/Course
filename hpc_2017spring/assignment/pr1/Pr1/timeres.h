/*	timeres.h

	Measure timer resolution.

	Robert van Engelen, 2007

	Usage:
		double timeres(void)
		Returns the cputime() timer resolution in seconds
		Prints report to stderr
*/

#ifndef TIMERES_H
#define TIMERES_H

#ifdef __cplusplus
extern "C" {
#endif

double timeres(void);

#ifdef __cplusplus
}
#endif

#endif
