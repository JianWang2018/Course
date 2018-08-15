// GaussJacobiMethod.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define  ITER  160  //iteration steps number

using namespace std;

/*Jacobi method
* input: N is the size of equation, A is a matrix of (N x N)
*        b and c is a vector of N.
* output: save the solution in x.
*/
void JacobiMethod(int N, double *A, double *b, double *x)
{
	if (N < 2)
		return;

	//using a temp vector to save already computed elements in x
	double *xTemp = (double *)malloc(N * sizeof(double));
	//marking step number 
	int nStep = 0;
	while (nStep < ITER)
	{
		++nStep;
		for (int i=0; i<N; ++i)
		{
			xTemp[i] = 0;
			for (int j=0; j<N; ++j)
			{
				if (j != i)
				{
					xTemp[i] += A[i*N+j]*x[j];
				}
			}
			xTemp[i] = (b[i] - xTemp[i])/A[i*N+i];//a temporary storage for x[i]
		}
		for (int i=0; i<N; ++i)
		{
			x[i] = xTemp[i];//reassign to x
		}
	}

	free(xTemp);
}
/*Gauss-Seidel method
* input: N is the size of equation, A is a matrix of (N x N)
*        b and c is a vector of N.
* output: save the solution in x.
*/
void GaussSeidelMethod(int N, double *A, double *b, double *x)
{
	if (N < 2)
		return;

	int nStep = 0;
	while (nStep < ITER)
	{
		++nStep;
		for (int i=0; i<N; ++i)
		{
			double fTemp = 0;
			for (int j=0; j<N; ++j)
			{
				if (j != i)
				{
					fTemp += A[i*N+j]*x[j];
				}
			}
			x[i] = (b[i] - fTemp)/A[i*N+i];
		}
	}
}
//main function
int _tmain(int argc, _TCHAR* argv[])
{
	double A[20][20] = {{0}};//Matrix A
	double b[20] = {0};// vector b
	double xJ[20] = {0};// solution x using Jacobi method
	double xG[20] = {0};// solution x using Gauss-Seidel method
	double sol[20];//exact solution

	double errJ =0;//Jacobi error
	double errG =0;//Gauss-Seidel error

	//Initialize A, b, sol
	for (int i=0; i<20; ++i)
	{
		for(int j=0; j<20; ++j)
		{
			if (j == i)
				A[i][j] = 2;
			else if (j == i-1 || j == i+1)
				A[i][j] = -1;
		}
		sol[i] = 1.0;
	}
	b[0] = 1.0;
	b[19] = 1.0;

	//compute 
	JacobiMethod(20, (double *)A, b, xJ);
	GaussSeidelMethod(20, (double *)A, b, xG);

	//calculate error
	for (int i=0; i<20; ++i)
	{
		errJ += (xJ[i]-sol[i])*(xJ[i]-sol[i]);
		errG += (xG[i]-sol[i])*(xG[i]-sol[i]);
	}
	errJ = sqrt(errJ);
	errG = sqrt(errG);

	printf("\nThe error using Jacobi method is: %0.8f\n", errJ);
	printf("\nThe error using Gauss-Seidel method is: %0.8f\n", errG);

	system("pause");

	return 0;
}


