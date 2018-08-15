//SinFunction.cpp
#include "stdafx.h"
#include "SinFunction.h"
#include <math.h>

//function f(x) in Newton's method
double SinFunction::f( double x )
{
	return sin(x);
}
//function f'(x) in Newton's method
double SinFunction::fDerivative( double x )
{
	return cos(x);
}
