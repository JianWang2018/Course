#include <stdio.h>
#include <stdlib.h>

/*
	int a; // primitive variable
	int* b; // pointer
	printf("address(a) = %u\n", &a);
	printf("address(b) = %u\n", b);
	printf("value(b)  = %d\n", *b);
*/

int main()
{

      float a = 3.45;
	  printf("original a: %f\n", a);

	  float* b = &a;   // & is address
	  (*b) = 45.;
	  printf("changed a= %f, *b= %f\n", a, *b);

	  float c = -14.;
	  b = &c;      // I change the variable "b" points to
	  (*b) = 17.;

	  printf("a= %f, c= %f\n", a, c);
	  printf("a did not change, c changed!!!\n");
	   
	  // difference between signed and unsigned
	  printf("address b = %d, %u\n", (int) b, (unsigned int) b);
	  printf("address b = %d, %u\n", b, b);
	  printf("address b = %x, %x\n", (int) b, (unsigned int) b);

	  printf("value(b) = %f\n", (*b));

	  exit(0);
}
