//----------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>   // for exit(0);

#include "Grid.h"     // searches current directory
//#include <Grid.h>   // does not search current directory


int main()
{
	Grid a;
	Grid b(10,20);

	printf("size(a)= %d\n", sizeof(a));

	a.print("before");

	a.nx = 100;
	a.ny = 200;

	a.print("after");

	Grid& c = a;
	Grid* d = &b;

	// gives the size of a, which is a class instance
	printf("Grid& : sizeof(c) = %d\n", sizeof(c)); 

	// gives the size of a pointer (4 bytes on 32 bit machine)
	printf("Grid* : sizeof(d) = %d\n", sizeof(d));

	exit(0);
}

//----------------------------------------------------------------------
