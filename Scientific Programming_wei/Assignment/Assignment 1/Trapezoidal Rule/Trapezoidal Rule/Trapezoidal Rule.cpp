// Trapezoidal Rule.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <math.h>

double f(double x)
{
	return (sin(x));
}
double g(double x)
{
	return (-sin(x));
}
int main()
{
	double a = 0.0;
	double b =acos(-1);
	int N = 20;
	double h = (b - a) / N;
	double s = 0.5*h*(f(a) + f(b));
	for (int i = 1; i < N; i++)
	{
		s = s + h*f(a + i*h);
	}
	double E = (b - a)*h*h / 12;
	printf("integration of sin(x), from 0 to PI, with Trapezoidal Rule is: %0.8f\n", s);
	printf("error bound in Trapezoidal Rule is: %0.8f\n", E);
    return 0;
}

