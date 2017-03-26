//----------------------------------------------------------------------

#include <stdio.h>
//#include <stdlib.h>  // for exit(0);

// The include file (declarations mixed with implementation)
class Grid 
{
public:
	int nx, ny;
	float var[100];  // array of 100 floats

public:
	Grid() {
		nx = 50;
		ny = 100;
	}

public:
	Grid(int dum, int ny) {
		nx = dum;
		this->ny = ny;
	}

	void print(char* msg) {
		printf("%s\n", msg);
		printf("nx, ny= %d, %d\n", nx, ny);
	}

    void setNx(int nx) {
	}
};   // DO NOT FORGET ";"
//----------------------------------------------------------------------


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

