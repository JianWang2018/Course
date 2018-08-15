//MyFunction.cpp
#include "stdafx.h"
#include "MyFunction.h"
#include <math.h>

//function f(x) in Newton's method
double MyFunction::f( double x )
{
	return x + cos(x);//using this function to test Newton's method
}
//function f'(x) in Newton's method
double MyFunction::fDerivative( double x )
{
	return 1.0 - sin(x);
}
