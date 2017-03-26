// What kind of arithmetic operations are possible on pointers? 

#include <stdio.h>
#include <stdlib.h>

class Grid {
	// What is default scope?
	double x[100]; 
	double y[100];

public:
	Grid() {
		printf("Grid constructor, sizeof(Grid)= %d\n", sizeof(Grid));
	}
};

//----------------------------------------------------------------------
class Stream {
	double f[10];
	Grid g[5];

public:
	Stream() {
		printf("Stream constructor, sizeof(Stream)= %d\n", sizeof(Stream));
	}
};
//----------------------------------------------------------------------

int main()
{
	Grid g; // note the sizes
	Stream s; // note the sizes

	Stream s3[3];  
	printf("s3= %u\n", s3);      // %u : unsigned int
	printf("&s3= %u\n", &s3);
	printf("&s3[0]= %u\n", &s3[0]);
	printf("&s3[1]= %u\n", &s3[1]);

	// subtract two pointers, and get the number of objects (not the number of bytes)
	printf("&s3[1]-&s3[0]= %u\n", &s3[1]-&s3[0]); // subtract two pointers

	Grid g3[3];
	//printf("&g3[2]-&s3[1] = %u\n", &g3[2] - &s3[1]); // NOT ALLOWED!! Since different types
	// &g3[2] = g3+2;
	// &s3[1] = s3+1;
	// &g3[2]-&s3[1] = (g3+2) - (s3+1) = g3 - s3 + 1  (can get confusing!!!)

	//printf("&g3[2]+&g3[1] = %u\n", &g3[2] + &g3[1]); // Cannot add pointers!!!

	// CONCLUSIONS: 
	// -  Can only add integers to pointers
	// -  Can only subract pointers of the same time, which equals number of elements between the two pointers


	exit(0);
}
//----------------------------------------------------------------------
