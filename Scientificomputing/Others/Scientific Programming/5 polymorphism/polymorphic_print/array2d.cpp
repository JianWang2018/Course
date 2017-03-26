#include <stdio.h>
#include "array3d.h"
#include "array2d.h"

Array2D::Array2D(int n1, int n2) : Array3D(n1, n2, 1)
{
	printf("Array2D constructor\n");
}
//----------------------------------------------------------------------
Array2D::~Array2D()
{
// really needed?
	printf("inside Array2D destructor, data= %ld\n", (long) data);

	// delete data only if nonzero
	if (data) delete [] data;

	// prevent data from being deleted twice
	data = 0;
	printf("exit Array2D destructor\n");
}
//----------------------------------------------------------------------
Array2D::Array2D(const Array2D& arr) : Array3D(arr)
{
	//if (this == &arr) return;
	this->nx = arr.nx;
	this->ny = arr.ny;
	this->nz = arr.nz;
	this->data = new float [nx*ny*nz];
	printf("inside array2d copy constructor\n");
	for (int i=0; i < nx*ny*nz; i++) {
		data[i] = arr.data[i];
	}
}
//----------------------------------------------------------------------
Array2D& Array2D::operator=(Array2D& arr) 
{
	//if (this == &arr) return;
	this->nx = arr.nx;
	this->ny = arr.ny;
	printf("inside array2d operator=\n");
	for (int i=0; i < nx*ny; i++) {
		data[i] = arr.data[i];
	}
}
//----------------------------------------------------------------------
const Array2D& Array2D::operator=(const Array2D& arr)
{
	//if (this == &arr) return;
	this->nx = arr.nx;
	this->ny = arr.ny;
	printf("inside const array2d operator=\n");
	for (int i=0; i < nx*ny; i++) {
		data[i] = arr.data[i];
	}
}
//----------------------------------------------------------------------
#if 1
float& Array2D::operator()(int i, int j)
{
	printf("Array2D::in operator()\n");
	return data[i+nx*j]; // more efficient
}
#endif
//----------------------------------------------------------------------
void Array2D::print()
{
	// use nx,ny,nz from Array2D.h
	printf("Array2D::print, nb_el= %d\n", nx*ny*nz);
}
//----------------------------------------------------------------------
