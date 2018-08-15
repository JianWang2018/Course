//RealFunction.cpp
#include "stdafx.h"
#include "RealFunction.h"
#include <math.h>

#define	 ERROR_BOUNDARY		1e-9  //error accuracy
#define  ITER_STEP			1e4   //maximum number of iteration steps

//calc one step
double RealFunction::GetNextValue(double x0)
{
	return x0 - f(x0) / fDerivative(x0);
}
//get root x of equation f(x)=0
double RealFunction::getRoot()
{
	double x0 = 0.0;//start value
	if (fabs(f(x0)) < ERROR_BOUNDARY)//x0 is the root
		return x0;

	int iStep = 1;
	double x1 = GetNextValue(x0);//calc one step
	while (fabs(x1 - x0) > ERROR_BOUNDARY && iStep < ITER_STEP)
	{//stop when either getting a close solution or reaching the given maximum steps 
		x0 = x1;
		x1 = GetNextValue(x0);
		++iStep;
	}
	return x1;
}

