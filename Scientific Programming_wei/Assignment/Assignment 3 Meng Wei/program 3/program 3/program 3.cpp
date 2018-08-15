// program 3.cpp : Defines the entry point for the console application.
//



#include "stdafx.h"
#include <iomanip>
#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>

int rowA = 0;
int colA = 0;
float s = 0;
float average;

using namespace std;

int main()
{
	string lineA;
	float x;
	float arrayA[20][20] = { { 0.0 } };
	double **A, **B, **C;
	A = new double*[10];
	B = new double*[10];
	C = new double*[10];
	int i, j, k;

	ifstream fileIN;
	cout << "Please Enter the data file below and press enter:" << endl;
	fileIN.open("matrices.dat");

	//Error Check
	if (fileIN.fail())
	{
		cerr << "*File you are trying to access cannot be found or opened!";
		exit(1);
	}

	//Reading the data file
	cout << "\n" << endl;
	while (fileIN.good()) {
		while (getline(fileIN, lineA))
		{
			istringstream streamA(lineA);
			colA = 0;
			while (streamA >> x) {
				arrayA[rowA][colA] = x;
				colA++;
			}
			rowA++;
		}
	}

	for (int i = 0; i < rowA; i++) {
		for (int j = 0; j < colA; j++) {
			cout << left << setw(10) << arrayA[i][j] << " ";
		}
		cout << endl;
	}

	cout << "\n" << endl;

	for (i = 0; i<10; i++) {
		A[i] = new double[10]();
		B[i] = new double[10]();
		C[i] = new double[10]();
	}


	for (i = 0; i<10; i++) {
		for (j = 0; j<10; j++) {
			A[i][j] = arrayA[i][j];
		}
	}
	for (i = 10; i<20; i++) {
		for (j = 0; j<10; j++) {
			B[i - 10][j] = arrayA[i][j];
		}
	}

	//Reading into matrix A
	cout << "Matrix A=" << endl;
	for (i = 0; i<10; i++) {
		for (j = 0; j<10; j++) {
			cout << left << setw(10) << A[i][j] << " ";
		}
		cout << endl;
	}
	cout << "\n" << endl;

	// Reading into matrix B
	cout << "Matrix B=" << endl;
	for (i = 0; i<10; i++) {
		for (j = 0; j<10; j++) {
			cout << left << setw(10) << B[i][j] << " ";
		}
		cout << endl;
	}


	//Multiplying A and B to get matrix C
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			for (k = 0; k < 10; k++) {
				s = s + A[i][k] * B[k][j];
			}
			C[i][j] = s;
			s = 0;
		}
	}
	cout << "\n" << endl;
	cout << "Matrix C=" << endl;
	for (i = 0; i<10; i++) {
		for (j = 0; j<10; j++) {
			cout << left << setw(10) << C[i][j] << " ";
		}
		cout << endl;
	}

	cout << "\n" << endl;


	//Computing the average of all elements of C
	cout << "The average of all elements of Matrix C is:" << endl;
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++) {
			s = s + C[i][j];
		}
	}
	average = s / 100;
	cout << average << endl;

	//Print out the average of all elements of C to a text file
	fstream textfile;
	textfile.open("average.txt");

		textfile << "The average of all elements of C is : " << average << endl;

		textfile.close();
	return 0;
}


