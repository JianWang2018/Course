// How to use a reference to simplify notation

#include <stdio.h>
#include <stdlib.h>


//---------------
class Grid 
{
public:
	float x[10];

	void print() {
		printf("inside grid\n");
	}
};
//---------------

class Stream {
public:
	Grid g[5];
};
//---------------

int main()
{
	Stream* str = new Stream [10];

	for (int i=0; i < 10; i++) {
		for (int j=0; j < 5; j++) {
			str[i].g[j].print();
		}
	}

	// alternatively
	for (int i=0; i < 10; i++) {
		Stream& s = str[i];   // use of a reference
		for (int j=0; j < 5; j++) {
			s.g[j].print();   // notation has been simplified
		}
		// "s" only exists between "{" and "}"
	}

	//---------------------------------
	Stream** str2 = new Stream* [5];
	for (int i=0; i < 5; i++) {
		str2[i] = new Stream();
	}

	for (int i=0; i < 10; i++) {
		Stream& s = *str2[i];
		for (int j=0; j < 5; j++) {
			s.g[j].print(); // notation remains the same
			str2[i]->g[j].print();  // more complicated and confusing
			(*str2[i]).g[j].print(); // more complicated and confusing
		}
	}

	// simplify calling methods
	// If I write a method that is called by value with a Stream argument,
	// I can change it later to call by reference, with a Stream& argument without 
	// changing ANYTHING in the method. 

	exit(0);
}
//----------------------------------------------------------------------
