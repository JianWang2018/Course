/* Department of Scientific Computing, FSU
 * Header file: RealFunction.h
 * Author: Hongzhuan Lei
 * Date: 10/20/2015 03:05PM
*/

#ifndef NM_REALFUNCTION_H_
#define NM_REALFUNCTION_H_

class RealFunction
{
public:
	RealFunction(){}//default constructor
	~RealFunction(){}//default destructor

public:
	//get root x of equation f(x)=0
	//by using the default start value x0=0.0
	//return the root or the result after reaching the given maximum iteration steps
	double getRoot();

protected:
	//function f, every subclass must overwrite it.
	virtual double f(double )=0;
	//function f', every subclass must overwrite it.
	virtual double fDerivative(double )=0;

	//Using Newton's method to get next step value
	double GetNextValue(double x0);


};

#endif