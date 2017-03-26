// We experiment with the meaning of pointers for 1D arrays 

#include <stdio.h>
#include <stdlib.h>

int main()
{
	double a[10];

	printf("sizeof(a) = %d\n", sizeof(a)); // size is 80 = 10 doubles (a is NOT a pointer)
	printf("sizeof(&a) = %d\n", sizeof(&a)); // size is 4  (&a is a pointer)
	printf("sizeof(&a[0])= %d\n", sizeof(&a[0])); // size is 4 (pointer)

	printf("a=%ud, address(a) = %ud\n", a, &a);
	printf("address(a[0] = %ud\n", &a[0]);
	printf("address(a[1] = %ud\n", &a[1]);
	printf("address(a[1]=&a[0]+1 = %ud\n", &a[0] + 1);
	printf("address(a[1]=a+1 = %ud\n", a + 1);

	// We note that address(a) = address(a[0])
	//-----------------------------------------------

	double* b = new double [10];
	printf("sizeof(b) = %d\n", sizeof(b)); // size is 4 
	printf("sizeof(b[0]) = %d\n", sizeof(b[0])); // size is 8  (1st element of array b)
	printf("sizeof(&b[0]) = %d\n", sizeof(&b[0])); // size is 4  (address of 1st element is a pointer)
	printf("b = %ud, address(b) = %ud\n", b, &b); 
	printf("&b[0] = %ud\n", &b[0]); // size is 4  (address of 2nd element of array b)
	printf("&b[1] = %ud\n", &b[1]); // size is 4  (address of 2nd element of array b)

	delete [] b; // always delete whatever I allocated with new, preferably in the same method 

	// What have we learned? 
	// double a[10]; ==>  a = &a = &a[0];  // strange but true
	// double* b = new b [10]; // b != &b, but b = &b[0]
	// So in all cases, the name of an array is the address of the first element!
	//----------------------------------------------------------------------

	exit(0);
}
//---------------------------------------------------------------------/-
