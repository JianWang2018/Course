/* Department of Scientific Computing, FSU
 * Header file: SinFunction.h
 * Author: Hongzhuan Lei
 * Date: 10/20/2015 03:11PM
*/

#ifndef NM_SINFUNCTION_H_
#define NM_SINFUNCTION_H_

#include "RealFunction.h"

class SinFunction : public RealFunction
{
public:
	SinFunction(){}//default constructor
	~SinFunction(){}//default destructor

protected:
	//overwrite this virtual function by using sin function.
	virtual double f(double x);
	//overwrite this virtual function by using cos function.
	virtual double fDerivative(double x);


};

#endif