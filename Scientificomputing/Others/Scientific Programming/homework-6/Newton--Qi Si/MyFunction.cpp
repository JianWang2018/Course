#include "MyFunction.h"


MyFunction::MyFunction(void)
{
}


MyFunction::~MyFunction(void)
{
}

double MyFunction::f(double x)
{
	return x*x-1;
}

double MyFunction::fDerivative(double x)
{
	return 2*x;
}