
#ifndef _INTEGRATE_TRAPEZOID_H_
#define _INTEGRATE_TRAPEZOID_H_

#include <stdio.h>
#include "Integrate.h"

class IntegrateTrapezoid : public Integrate
{
public:
	IntegrateTrapezoid() {;}

	// what happens if execute is removed? 
	void execute();
};

#endif
