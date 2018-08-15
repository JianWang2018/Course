//test the Newton's method
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "SinFunction.h"
#include "MyFunction.h"

void Test()
{
	SinFunction sinFun;
	MyFunction  myFun;

	double xRootS = sinFun.getRoot();
	double xRootM = myFun.getRoot();

	printf("Using Newton's method with the starting value 0.0\n");
	printf("For the function 'f(x) = sin(x)', the root is: %0.8f \n", xRootS);
	printf("For the function 'f(x) = x+cos(x)', the root is: %0.8f \n", xRootM);

}

int _tmain(int argc, _TCHAR* argv[])
{
	Test();

	system("pause");

	return 0;
}