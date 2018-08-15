// Simpson Rule.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <math.h>


double f(double x)
{
	return (sin(x));
}

double h(double x)
{
	return (sin(x));
}

int main()
{
	double a = 0.0;
	double b = acos(-1);
	int N = 320;
	double h = (b - a) / N;
	double s = f(a) + f(b);
	for (int i = 1; i < N; i++)
	{
		if (i % 2 != 0)
			s = s + 4 * f(a + i*h);
		else s = s + 2 * f(a + i*h);
	}
	s = h*s / 3;
	double E = (b - a)*h*h*h*h / 180;
	printf("integration of sin(x), from 0 to PI, with Simpson Rule is: %0.8f\n", s);
	printf("error boundf in Simpson Rule is: %0.8f\n", E);
	return 0;
}

