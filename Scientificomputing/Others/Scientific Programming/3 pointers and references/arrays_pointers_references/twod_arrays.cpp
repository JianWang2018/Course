
// We experiments with the meaning of pointers for 2D arrays

#include <stdio.h>
#include <stdlib.h>

class Grid 
{
public:
	double x[102];
};
//-----------------------------

int main()
{
	double a[10][4];

	for (int j=0; j < 10; j++) {
	for (int i=0; i < 4; i++) {
		a[j][i] = 100*j + i;   // Just by looking at a[j][i], I can figure out i and j
	}}

	printf("sizeof(a) = %d\n", sizeof(a)); // size is 80 = 10 doubles (a is NOT a pointer)
	printf("sizeof(&a) = %d\n", sizeof(&a)); // size is 4  (&a is a pointer)
	printf("sizeof(&a[0])= %d\n", sizeof(&a[0])); // size is 4 (pointer)
	printf("sizeof(&a[0][0])= %d\n", sizeof(&a[0][0])); // size is 4 (pointer)

	printf("a=%u, address(a) = %u\n", a, &a);
	printf("&a[0]= %u, &a[0][0]= %u\n", &a[0], &a[0][0]);

	// CONCLUSION
	// a = &a = &a[0] = &a[0][0]      // Interesting, no? 

	printf("&a[0][0]= %u, &a[0][1]= %u\n", &a[0][0], &a[0][1]);
	printf("&a[0][0]= %u, &a[1][0]= %u\n", &a[0][0], &a[1][0]);
	printf("&a[0][1]-&a[0][0]= %d\n", &a[0][1]-&a[0][0]);    // difference i 1
	printf("&a[1][0]-&a[1][0]= %d\n", &a[1][0]-&a[0][0]);    // difference is 4 (CONSEQUENCE?)

	// CONCLUSION
	// successive in memory: a[0][0], a[0][1], a[0][2], a[0][3], a[1][0], a[1][1], ...
	// second index varies the fastest. Same in Java. Different in Fortran. 

	printf("sizeof(a)= %d\n", sizeof(a));  // 320 = 10*4 doubles = 10*4*8 = 320 bytes

	printf("*a[2] = %f\n", *a[2]); // *b[2]= 200 = b[2][0]
	printf("*(a[2]+1) = %f\n", *(a[2]+1)); // *(b[2]+1)= 201 = b[2][1] 
	printf("*(a[2]-2) = %f\n", *(a[2]-2)); // *(b[2]+1)= 103 = b[1][3] 
	printf("*(&a[1][3]+2) = %f\n", *(&a[1][3]+2)); // *(b[2]+1)= 103 = b[1][3] 

	// CONCUSION: 40 doubles are consecutive in memory

	// For more proof
	printf("&a[9][3]-&a[0][0]= %d\n", &a[9][3]-&a[0][0]); // returns 39 = 4*10 - 1, which is expected

	//----------------------------------------------------------------------
	// Dynamic allocation

	//double** b = new double* [10]; // ALLOWED
	double** b;
	b = new double* [10]; // create an array of 10 (double*)

	//b[0] = 4.56 // ILLEGAL since b[0] is double*, and 4.56 is double

	for (int i=0; i < 10; i++) {
		b[i] = new double [4];
	}

	for (int j=0; j < 10; j++) {
	for (int i=0; i < 4; i++) {
		b[j][i] = 100*j + i;   // Just by looking at b[j][i], I can figure out i and j
	}}

	printf("b = %u\n", b);
	printf("&b[0] = %u\n", &b[0]);
	printf("b[0] = %u\n", b[0]);
	printf("&b[0][0] = %u\n", &b[0][0]);

	// CONCLUSION: 
	// -   b = &b[0]
	// -   b != b[0] != &b[0][0]
	// ADVICE: do not count on this behavior. If you want the address of the first element of an array, 
	//    ALWAYS do    &a[0]  or &a[0][0]

	printf("*b[2] = %f\n", *b[2]); // *b[2]= 200 = b[2][0]
	printf("*(b[2]+1) = %f\n", *(b[2]+1)); // *(b[2]+1)= 201 = b[2][1] 
	printf("*(b[2]-2) = %f\n", *(b[2]-2)); // *(b[2]+1)= 103 = b[1][3] 
	printf("*(&b[1][3]+2) = %f\n", *(&b[1][3]+2)); // *(b[2]+1)= 103 = b[1][3] 

	printf("&b[9][3]-&b[0][0]= %d\n", &b[9][3]-&b[0][0]); // returns 48 > 4*10 which is not expected

	// Break down the count
	printf("--- How is memory used for a double** array? ---\n");
	for (int j=0; j < 5; j++) {
		for (int i=0; i < 4; i++) {
			printf("&b[%d][%d]= %u\n", j, i, &b[j][i]);
		}
		printf("......\n");
	}
	printf("---------\n");
	// CONCLUSION: Skip of 8+8 bytes when going from &a[3][0] to &a[1][0]

	// NOW TRY with LARGER STRUCTURE
	Grid** g = new Grid* [5];
	for (int i=0; i < 5; i++) {
		g[i] = new Grid [4];
	}
	printf("-------------------------------\n");
	printf("sizeof(Grid) = %d\n", sizeof(Grid));
	printf("sizeof(g[0][0]) = %d\n", sizeof(g[0][0]));

	for (int j=0; j < 5; j++) {
		for (int i=0; i < 4; i++) {
			printf("&g[%d][%d]= %u\n", j, i, &g[j][i]);
		}
		printf("......\n");
	}
	// CONCLUSION: skip of sizeof(Grid)+8 bytes when going from g[0][3] to g[1][0]
	// So when dealing with pointers, array elements are NOT continuous in memory

	printf("\n\nsizeof(Grid) = %d\n", sizeof(Grid));
	printf("&g[1][0] = %d\n", &g[1][0]);
	printf("&g[0][3] = %d\n", &g[0][3]);
	printf("&g[1][0]-&g[0][3]= %d\n", (int) &g[1][0] - (int) &g[0][3]);

	// In fact (VERY IMPORTANT),  there is no reason why the skip factor should be exactly 
	// the size of one (4 bytes) or two (8 bytes) pointer widths. 
	// g[0] and g[1] and g[2] could in principle point in completely different areas of memory. 
	// REMEMBER THIS!


	#if 0
	printf("address(a[1] = %u\n", &a[1]);
	printf("address(a[1]=&a[0]+1 = %u\n", &a[0] + 1);
	printf("address(a[1]=a+1 = %u\n", a + 1);

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
	#endif

	exit(0);
}
//---------------------------------------------------------------------/-
