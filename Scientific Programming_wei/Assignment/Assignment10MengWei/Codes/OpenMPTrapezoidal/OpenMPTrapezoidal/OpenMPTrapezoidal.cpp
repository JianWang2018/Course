// OpenMPTrapezoidal.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

//define the integrand f(x)=sin(x)
double f(double x)
{
	return (sin(x));
}
//compute f''(x) in order to get error bound
double g(double x)
{
	return (-sin(x));
}

int main()
{
	double a = 0.0;
	double b = acos(-1);
	int i;
	double wtime;
	int N = 100000000;
	double h = (b - a) / N;
	double s;
	double E;
	s = 0.5*h*(f(a) + f(b));
	wtime = omp_get_wtime();
# pragma omp parallel num_threads(8) shared (a, b, N, h, E) private ( i )
	
# pragma omp for reduction ( + : s )
   
	for ( i = 1; i < N; i++)
	{
		s = s + h*f(a + i*h);
	}
    E = (b - a)*h*h / 12;
	wtime = omp_get_wtime() - wtime;// the time for Trapezoidal rule

	printf("%12f\n", wtime);
	printf("integration of sin(x), from 0 to PI, with Trapezoidal Rule is: %0.8f\n", s);
	printf("error bound in Trapezoidal Rule is: %0.8f\n", E);
	return 0;
}

