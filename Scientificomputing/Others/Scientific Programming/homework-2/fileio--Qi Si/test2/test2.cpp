/*Create two matrices, A and B, both are 10 by 10. Write a code to initialize the two
matrices by:
A[i][j] = i+j+1.0;
B[i][i] = 1/(i+j+1.0);
and write A and B into a binary file ¡°matrices.dat¡±. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
	int i ;
	int j ;
	double A[10][10];
	double B[10][10];

	FILE *pf = fopen( "matrices.dat", "wb");
	if (pf == NULL)
	{
		fputs( "File error", stderr);
		exit(1);
	}

	// initialize matrices A and B;

	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			A[i][j] = i+j+1.0;
			B[i][j] = 1/(i+j+1.0);
		}
	}

	// write A into the file;

	for (i = 0; i < 10; i++)
	{
		fwrite(A[i], sizeof(double), 10, pf);
	}

	// write B into the file;
	
	for (i = 0; i < 10; i++)
	{
		fwrite(B[i], sizeof(double), 10, pf);
	}

	fclose(pf);
	return 0;
}