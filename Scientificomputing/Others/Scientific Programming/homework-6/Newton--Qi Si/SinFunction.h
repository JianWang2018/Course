#pragma once
#include "RealFunction.h"

class SinFunction:public RealFunction
{
public:
	SinFunction(void);
	~SinFunction(void);
	double f(double);
	double fDerivative(double);
};

