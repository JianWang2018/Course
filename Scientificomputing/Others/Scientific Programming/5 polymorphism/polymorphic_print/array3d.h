#ifndef _ARRAY3D_H_
#define _ARRAY3D_H_

class Array3D
{
// available to subclasses
protected:
	float* data;

// poor programming, but for expediency
public:
	int nx, ny, nz;

public:
	Array3D(int n1, int n2=1, int n3=1);
	~Array3D();
	Array3D(const Array3D& arr);
	const Array3D& operator=(const Array3D& arr);
	Array3D& operator=(Array3D& arr);
	float& operator()(int i, int j=0, int k=0);

	virtual void print();
};

#endif
