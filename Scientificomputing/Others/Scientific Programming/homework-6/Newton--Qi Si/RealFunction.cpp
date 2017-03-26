#include "RealFunction.h"


RealFunction::RealFunction(void)
{
}


RealFunction::~RealFunction(void)
{
}

double RealFunction::getRoot()
{
	double epsilon = pow(10.0, -14);
	double x0=2.0;
	for(;;){
		if(abs(fDerivative(x0)) < epsilon){
			std::cout << " Warning: denominator is too small" << std::endl;
		break;
		}
		double x1 = x0-f(x0)/fDerivative(x0);
		if(fabs(x1-x0)<0.000001) break;
		x0 = x1;
	}
	return x0;
}