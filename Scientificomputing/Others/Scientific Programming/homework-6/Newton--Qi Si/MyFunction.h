#pragma once
#include <cmath>
#include "RealFunction.h"

class MyFunction:public RealFunction
{
public:
	MyFunction(void);
	~MyFunction(void);
	double f(double);
	double fDerivative(double);
};

