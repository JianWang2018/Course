
#include <stdio.h>
#include <vector>
#include "tornado.h"

using namespace std;

//----------------------------------------------------------------------
// 3 constructors, 3 destructors


int main()
{
	Tornado t1, t2, t3;
	Tornado *t4;
	vector<Tornado*> tl;

	printf("------------------------------\n");
	tl.push_back(&t1);
	printf("sizeof(tl)= %d\n", sizeof(tl));
	printf("after first push_back, size= %d, capacity= %d\n", 
		tl.size(), tl.capacity());
	printf("sizeof(tl)= %d\n", sizeof(tl));

	printf("------------------------------\n");
	tl.push_back(&t2);
	printf("sizeof(tl)= %d\n", sizeof(tl));
	printf("after second push_back, size= %d, capacity= %d\n", 
		tl.size(), tl.capacity());

	printf("------------------------------\n");
	tl.push_back(&t3);
	printf("sizeof(tl)= %d\n", sizeof(tl));
	printf("after third push_back, size= %d, capacity= %d\n", 
		tl.size(), tl.capacity());

	printf("What has happened: each time the capacity is exceeded, \n");
	printf("   the capacity is doubled\n");

	printf("\n\n");
	printf("3 destructors are for t1, t2, t3\n");
	printf("3 destructors are for the 3 elements in vector<Tornado>\n");

	tl.push_back(t4);
}
//----------------------------------------------------------------------
