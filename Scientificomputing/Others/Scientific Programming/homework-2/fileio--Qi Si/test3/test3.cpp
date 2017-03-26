/* Write a code to read ¡°matrices.dat¡± into matrices A and B, multiply A and B to get
matrix C. Print out the average of all elements of C to a text file. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
	//read the file
	FILE *rf= fopen("matrices.dat", "rb");
	//check the file
	if (rf == NULL) 
	{
		fputs("File error", stderr);
		exit(1);
	}

	double A[10][10];
	double B[10][10];
	double C[10][10];
	int i, j, k;

	// read the file in the beginning;
	fseek(rf, 0, SEEK_SET);

	for (i = 0; i < 10; i++)
	{
		fread(A[i], sizeof(double), 10, rf);
	}
	
	for (i = 0; i < 10; i++)
	{
		fread(B[i], sizeof(double), 10, rf);
	}

	// multiply A and B to get C;

	for (i=0; i<10; i++)
	{
		for(j=0; j<10; j++)
		{
			C[i][j] = 0.0;
			for (k=0; k<10; k++)
			{
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}

	// compute the average of all elements of C;

	double aver = 0;
	for (i=0; i<10; i++)
	{
		for(j=0; j<10; j++)
		{
			aver = aver + C[i][j];
		}
	}
	aver = aver/100;


	FILE *wf = fopen("multiplication.txt", "w");
	if (rf == NULL) 
	{
		fputs("File error", stderr);
		exit(1);
	}
	fprintf(wf,"the average number of C is %f",aver);



	fclose(rf);
	fclose(wf);

	return 0;
}