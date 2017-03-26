#include <stdio.h>
#include "array3d.h"

Array3D::Array3D(int n1, int n2, int n3)
{
	nx = n1;
	ny = n2;
	nz = n3;
	data = new float [n1*n2*n3];

	for (int i=0; i < n1*n2*n3; i++) {
		data[i] = (float) i;
	}

	printf("Array3D constructor\n");
}
//----------------------------------------------------------------------
Array3D::Array3D(const Array3D& arr)
{
	//if (this == &arr) return;
	this->nx = arr.nx;
	this->ny = arr.ny;
	this->nz = arr.nz;
	this->data = new float [nx*ny*nz];
	printf("inside array3d copy constructor\n");
}
//----------------------------------------------------------------------
Array3D& Array3D::operator=(Array3D& arr) 
{
	//if (this == &arr) return;
	this->nx = arr.nx;
	this->ny = arr.ny;
	this->nz = arr.nz;
	printf("inside array3d operator=\n");
	for (int i=0; i < nx*ny*nz; i++) {
		data[i] = arr.data[i];
	}
}
//----------------------------------------------------------------------
// this CANNOT BE const
const Array3D& Array3D::operator=(const Array3D& arr)
{
	//if (this == &arr) return;
	this->nx = arr.nx;
	this->ny = arr.ny;
	this->nz = arr.nz;
	printf("inside const array3d operator=\n");
	for (int i=0; i < nx*ny*nz; i++) {
		data[i] = arr.data[i];
	}
}
//----------------------------------------------------------------------
Array3D::~Array3D()
{
	printf("inside Array3D destructor, data= %ld\n", (long) data);
	if (data) delete [] data;
	data = 0;
	printf("exit Array3D destructor\n");
}
//----------------------------------------------------------------------
float& Array3D::operator()(int i, int j, int k)
{
	printf("Array3D::in operator()\n");
	return data[i+nx*(j+ny*k)];
}
//----------------------------------------------------------------------
void Array3D::print()
{
	// use nx,ny,nz from Array2D.h
	printf("Array3D::print, nb_el= %d\n", nx*ny*nz);
}
//----------------------------------------------------------------------
