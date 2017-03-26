
#include <stdio.h>
#include "array2d.h"

// argument: Base class
// Without pointers/references, only Array3D::print() is accessed
void print(Array3D& arr) 
{
	arr.print();
}

//----------------------------------------------------------------------
int main()
{
	// calls 2D and 3D destructor
	Array2D arr(3,4);
	Array3D brr(3,4);

	printf("\nbefore arr.print\n");
	arr.print();
	brr.print();

	printf("\nbefore call print() method\n");
	// Base class print is called in both cases. 
	// This is called slicing. 
	// True whether references or pointers are used
	// When a pointer/reference is used, the addresses are reinterpreted
	// as pointing to differnet methods. The structure of a subclass
	// is identical to that of its parent, with additional data/methods
	// below it. 

	print(arr);
	print(brr);    //<<<<<**** Polymorphism: 2D print is accessed!!!!! <<<<*****

	printf("\nbefore base class assignment\n");
	// print from Array3D is accessed as expected
	Array3D* a3d = new Array2D(3,4);
	// Polymorphism: 2D print is accessed!!!! <<<<********
	a3d->print();

	printf("\nbefore return from main\n");
	return 0;
}
//----------------------------------------------------------------------
