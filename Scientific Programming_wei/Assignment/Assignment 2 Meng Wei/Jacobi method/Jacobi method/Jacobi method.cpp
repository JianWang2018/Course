// Jacobi method.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#define MAX 20

double *Jacobi(double *A, double *b)
{
	int n;
	scanf_s("%d", &n);
	double *x, s;
	int i, j, k = 0;

	x = (double*)malloc(n*sizeof(double));
	for (i = 0; i < n; i++)
		x[i] = 0;
	while (k< MAX)
	{

		for (i = 0; i < n; i++)
		{
			s = 0;

			for (j = 0; j < n; j++)
				if (j != i)
					s = s + A[i*n + j] * x[j];

			x[i] = (b[i] - s) / A[i*n + i];
		}
		k++;
	}
	return x;


}

int main()
{
	int n;
	scanf_s("%d", &n);
	int i, j;
	double *A = (double*)malloc((n*n)*sizeof(double));
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i == j - 1)
			{
				A[i*n + j] = -1.0;
			}
			else if (i == j)
			{
				A[i*n + j] = 2.0;
			}
			else if (i == j + 1)
			{
				A[i*n + j] = -1.0;
			}
			else { A[i*n + j] = 0.0; }
		}
	}

	/*for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			printf("%lf", A[i*n + j]);
	}*/

	double *b = (double*)malloc(n*sizeof(double));
	for (i = 0; i < n; i++)
	{
		if (i == 0 || i == n - 1)
			b[i] = 1;
		else b[i] = 0;
	}

	/*for (i = 0; i < n; i++)
		printf("%lf\n", b[i]);*/

	double*x = (double*)malloc(n*sizeof(double));
	x = Jacobi(A, b);
	for (i = 0; i < n; i++)
		printf("%lf\n", x[i]);
	free(A);
	free(b);
}


