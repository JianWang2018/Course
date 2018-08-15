/* Department of Scientific Computing, FSU
 * Header file: MyFunction.h
 * Author: Hongzhuan Lei
 * Date: 10/20/2015 03:21PM
*/

#ifndef NM_MYFUNCTION_H_
#define NM_MYFUNCTION_H_

#include "RealFunction.h"

class MyFunction : public RealFunction
{
public:
	MyFunction(){}//default constructor
	~MyFunction(){}//default destructor

protected:
	//overwrite this virtual function by using user defined function.
	virtual double f(double x);
	//overwrite this virtual function by using user defined function.
	virtual double fDerivative(double x);


};

#endif