
#include <stdio.h>
#include <vector>
#include "tornado.h"

using namespace std;

//----------------------------------------------------------------------
// 3 constructors, 9 destructors
// 6 copy constructors


int main()
{
	Tornado t1, t2, t3;
	Tornado t4;
	vector<Tornado> tl;

	printf("------------------------------\n");
	printf("sizeof(tl)= %d\n", sizeof(tl));
	printf("after zeroth push_back, size= %d, capacity= %d\n", 
		tl.size(), tl.capacity());
	printf("sizeof(tl)= %d\n", sizeof(tl));


	printf("------------------------------\n");
	tl.push_back(t1);
	printf("sizeof(tl)= %d\n", sizeof(tl));
	printf("after first push_back, size= %d, capacity= %d\n", 
		tl.size(), tl.capacity());
	printf("sizeof(tl)= %d\n", sizeof(tl));

	printf("------------------------------\n");
	tl.push_back(t2);
	printf("sizeof(tl)= %d\n", sizeof(tl));
	printf("after second push_back, size= %d, capacity= %d\n", 
		tl.size(), tl.capacity());

	printf("------------------------------\n");
	tl.push_back(t3);
	printf("sizeof(tl)= %d\n", sizeof(tl));
	printf("after third push_back, size= %d, capacity= %d\n", 
		tl.size(), tl.capacity());

	printf("------------------------------\n");
	printf("illustrate clear\n");
	printf("tl.size() = %d\n", tl.size());
	printf("tl.capacity() = %d\n", tl.capacity());

	// An iterator is a pointer
	vector<Tornado>::iterator it = tl.begin();
	it = ++it;
	//vector<Tornado>::iterator it = tl[2];
	tl.clear();
	printf("erase\n");
	printf("tl.size() = %d\n", tl.size());
	printf("tl.capacity() = %d\n", tl.capacity());


	printf("What has happened: each time the capacity is exceeded, \n");
	printf("   the capacity is doubled\n");

	printf("\n\n");
	printf("3 destructors are for t1, t2, t3\n");
	printf("3 destructors are for the 3 elements in vector<Tornado>\n");

}
//----------------------------------------------------------------------
