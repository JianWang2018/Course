// WriteMatrices.cpp : main project file.

#include "stdafx.h"    //Visual C++ use this file

#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)


int WriteMatrices()
{
	double A[10][10], B[10][10];

	//initialize A and B
	int i, j;
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			A[i][j] = i + j + 1.0;
			B[i][j] = 1 / (i + j + 1.0);
		}
	}

	//write to file
	FILE * f = fopen("matrices.dat", "wb");
	fwrite(A, sizeof(double), 100, f);
	fwrite(B, sizeof(double), 100, f);
	fclose(f);

	return 0;
}

int ReadMatrices()
{
	double A[10][10], B[10][10], C[10][10];

	//read from file
	FILE * f = fopen("matrices.dat", "rb");
	fread(A, sizeof(double), 100, f);
	fread(B, sizeof(double), 100, f);
	fclose(f);

	//calculate C = A*B
	int i, j, k;
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			C[i][j] = 0.0;
			for (k = 0; k < 10; k++)
				C[i][j] += A[i][k] * B[k][j];
		}
	}

	//count the average
	double avg = 0.0;
	for (i = 0; i < 10; i++)
		for (j = 0; j < 10; j++)
			avg += C[i][j];

	avg /= 100;

	//write avg to text file
	FILE * favg = fopen("avg.txt", "w");
	fprintf(favg, "%f", avg);
	fclose(favg);

	return 0;
}


int main()
{
	WriteMatrices();
	ReadMatrices();
}