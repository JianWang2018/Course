// Array.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <math.h>
#include "Array_op.h"


int _tmain(int argc, char * argv[])//for testing
{
	Array a;
	a.push_back(0);
	a.push_back(1);
	a.push_back(2);
	a.push_back(3);
	a.push_back(4);
	a.remove(2);
	a.insert(3, 10);
	
	printf("a: size is %d, capacity is %d\n", a.size(), a.capacity());
	
	Array b = a;
	printf("b: size is %d, capacity is %d\n", b.size(), b.capacity());
	
	//test the operator + and << of Array
	printf("the elements of Array a+b are: \n");
	for (int i = 0; i < a.size(); i++)
	{
		cout<< (a + b)[i]<<endl;
	}

	//test the operator [] for Array a
	printf("The elements of a is: \n");
	for (int i = 0; i < a.size(); i++)
	{
		printf("%d ", a[i]);
	}
	printf("\n");
	
	return 0;
}



