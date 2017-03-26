#include <stdio.h>
#include <stdlib.h>

int main()
{

      float a = 3.45;
	  printf("original a: %f\n", a);

	  float& b = a;   // reference (used like a variable, but really a "kind of" pointer)
	  b = 45.;
	  printf("changed a= %f, b= %f\n", a, b);

	  float c = -14.;
	  printf("\n\n original c: %f\n", c);
	  b = c;    // reference is NOT changed. It is actually a that is changed.
	            // reference b still points to a (contrary to Java!!)

	  printf("a,b,c= %f, %f, %f\n", a, b, c);

	  b = 17.;

	  printf("a,b,c= %f, %f, %f\n", a, b, c);
	  printf("a is changed, c is unchanged\n");

	  printf("-----------------\n");
	  printf("(address of b) &b = %u\n", &b);

	  //printf("*b = %d\n", (int) *b);
	  //printf("&&b = %u\n", &&b); // INVALID

	  exit(0);
}



