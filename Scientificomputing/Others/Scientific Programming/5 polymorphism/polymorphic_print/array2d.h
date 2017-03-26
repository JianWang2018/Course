#ifndef _ARRAY2D_H_
#define _ARRAY2D_H_

#include <stdio.h>
#include "Array3d.h"

class Array2D : public Array3D
{
public:
	Array2D(int n1, int n2=1);
	~Array2D();
	Array2D(const Array2D& arr);

	Array2D& operator=(Array2D& arr);
	const Array2D& operator=(const Array2D& arr);

	#if 1
	float& operator()(int i, int j=0);
	#endif

	void print();
};

#endif
