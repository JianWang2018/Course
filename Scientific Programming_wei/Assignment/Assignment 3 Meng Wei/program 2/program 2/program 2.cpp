#include "stdafx.h"
#include <iostream>
#include <stdio.h> 
#include <stdlib.h>
#include <string>
#pragma warning(disable:4996)

int main()
{
	double **A, **B;
	A = new double*[10];
	B = new double*[10];
	int i, j;
	for (i = 0; i<10; i++) {
		A[i] = new double[10]();
		B[i] = new double[10]();
	}


	for (i = 0; i<10; i++) {
		for (j = 0; j<10; j++) {
			A[i][j] = i + j + 1.0;
			B[i][j] = 1 / (i + j + 1.0);
		}
	}


	FILE *pFile;
	pFile = fopen("matrices.dat", "wb");

	for (i = 0; i<10; i++) {
		for (j = 0; j<10; j++) {
			fprintf(pFile, "%f ", A[i][j]);
		}
		fprintf(pFile, "\n");
	}

	for (i = 0; i<10; i++) {
		for (j = 0; j<10; j++) {
			fprintf(pFile, "%f ", B[i][j]);
		}
		fprintf(pFile, "\n");
	}


	//Release the memory
	for (i = 0; i<10; i++) {

		delete[] A[i];
		delete[] B[i];
	}
	delete[] A;
	delete[] B;
	fclose(pFile);

}



