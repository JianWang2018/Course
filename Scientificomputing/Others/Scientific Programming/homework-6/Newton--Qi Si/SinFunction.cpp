#include "SinFunction.h"
#include <cmath>


SinFunction::SinFunction(void)
{
}


SinFunction::~SinFunction(void)
{
}

double SinFunction::f(double x)
{
	return sin(x);
}

double SinFunction::fDerivative(double x)
{
	return cos(x);
}