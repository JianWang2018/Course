#pragma once
#include <iostream>

class RealFunction
{
public:
	RealFunction(void);
	~RealFunction(void);
	virtual double f(double)=0;
	virtual double fDerivative(double)=0;
	double getRoot(); //Newton's method
};

