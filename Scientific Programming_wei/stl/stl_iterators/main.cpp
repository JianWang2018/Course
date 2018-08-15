
#include <stdio.h>
#include <list>
#include "tornado.h"

using namespace std;

//----------------------------------------------------------------------
// 3 constructors, 9 destructors
// 6 copy constructors


int main()
{
	Tornado** to = new Tornado* [3];
	for (int i=0; i < 3; i++) {
		to[i] = new Tornado(3.14*i+2.1); // wind speed = 3.14*i
	}

	list<Tornado> tl;

	//printf("it->getSpeed() = %f\n", t2.getSpeed()); exit(0);

	printf("list has no concept of capacity\n");
	for (int i=0; i < 3; i++) {
		printf("------------------------------\n");

		tl.push_back(*to[i]);  // to[i] is a Tornado*, so *to[i] is a Tornado

		printf("after push_back %d, size= %d\n", i+1, tl.size());
		printf("sizeof(tl)= %d\n", sizeof(tl));
	}

	// Destroy original objects (they are still in the list, hopefully)
	for (int i=0; i < 3; i++) {
		delete to[i];
	}
	delete [] to;

	// printf("tl[2].getSpeed() = %f\n", tl[2].getSpeed()); // ILLEGAL use of tl[2]

	printf("------------------------------\n");
	// An iterator is a pointer so use "->" to access methods
	list<Tornado>::iterator it = tl.begin();
	it = it ++;           // ILLEGAL
	//it = tl.begin() + 4;  // ILLEGAL

	printf("it->getSpeed() = %f\n", (*it).getSpeed());
	Tornado& tt = *tl.begin();
	printf("tt.getSpeed() = %f\n", tt.getSpeed());

	printf("------------------------------\n");
	printf("illustrate clear\n");
	printf("tl.size() = %d\n", tl.size());

	//tl.clear();
	printf("erase\n");
	printf("tl.size() = %d\n", tl.size());

	printf("\n\n");
	printf("destructors are for t1, t2, t3\n");
	printf("3 destructors are for the 3 elements in list<Tornado>\n");
}
//----------------------------------------------------------------------
