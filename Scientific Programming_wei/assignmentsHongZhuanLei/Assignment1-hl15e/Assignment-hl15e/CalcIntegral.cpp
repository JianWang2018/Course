// CalcIntegral.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.1415926
#define RANGA  0
#define RANGB  PI
#define TRUEVALUE	2

double Func(double x)
{
	return  sin(x);
}
//Trapezoidal Rule
double CalcT(int nInterval)
{
	double fResult = 0.0;
	if (nInterval < 1)
		return fResult;

	for (int i = 0; i <= nInterval; ++i)
	{
		double fInterval = RANGA + i*(RANGB - RANGA) / (double)nInterval;
		int nFactor = 1;
		if (i != 0 && i != nInterval)
			nFactor = 2;

		fResult += nFactor * Func(fInterval);
	}

	fResult = fResult * (RANGB - RANGA) / (2.0*nInterval);
	return fResult;
}
//Simpson Rule
double CalcS(int nInterval)
{
	double fResult = 0.0;
	if (nInterval < 1 || nInterval % 2 != 0)
		return fResult;

	for (int i = 0; i <= nInterval; ++i)
	{
		double fInterval = RANGA + i*(RANGB - RANGA) / (double)nInterval;
		int nFactor = 1;
		if (i != 0 && i != nInterval && (i % 2 == 0))
			nFactor = 2;
		else if (i != 0 && i != nInterval && (i % 2 == 1))
			nFactor = 4;

		fResult += nFactor * Func(fInterval);
	}

	fResult = fResult * (RANGB - RANGA) / (3.0*nInterval);
	return fResult;
}
//
int main()
{
	double a = 0.0, b = PI;
	int Num[5] = { 20, 40, 80, 160, 320 };
	double TResult[5] = { 0.0 }, TError[5] = { 0.0 }, TOrder[5] = { 0.0 };
	double SResult[5] = { 0.0 }, SError[5] = { 0.0 }, SOrder[5] = { 0.0 };

	for (int ix = 0; ix < 5; ++ix)
	{
		TResult[ix] = CalcT(Num[ix]);
		TError[ix] = fabs(TResult[ix] - TRUEVALUE);

		SResult[ix] = CalcS(Num[ix]);
		SError[ix] = fabs(SResult[ix] - TRUEVALUE);
	}
	for (int ix = 0; ix < 4; ++ix)
	{
		TOrder[ix] = log(TError[ix] / TError[ix+1]) / log(2.0);
		SOrder[ix] = log(SError[ix] / SError[ix+1]) / log(2.0);
	}
	//print 
	printf("The Result Report:\n");
	printf("Number   TResult    TError    TOrder        SResult    SError    SOrder\n");
	for (int ix = 0; ix < 5; ++ix)
	{
		printf(" %3d   %0.8f %0.8f %0.8f    %0.8f %0.8f %0.8f\n", Num[ix],
			TResult[ix], TError[ix], TOrder[ix], SResult[ix], SError[ix], SOrder[ix]);
	}

	system("pause");

    return 0;
}