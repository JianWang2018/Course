
#ifndef _INTEGRATE_SIMPSON_H_
#define _INTEGRATE_SIMPSON_H_

#include <stdio.h>
#include "Integrate.h"

// Check out error message without public (applied to Integrate)
// error: ‘Integrate’ is an inaccessible base of ‘IntegrateSimpson’
class IntegrateSimpson : public Integrate
{
public:
	IntegrateSimpson() {;}
	void execute();
};

#endif
