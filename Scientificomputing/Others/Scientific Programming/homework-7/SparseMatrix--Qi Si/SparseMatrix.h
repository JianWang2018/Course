#include <iostream>
#include <vector>
#pragma once

using namespace std;

// Triples
struct term{
	int col;
	int row;
	double value;
};

class SparseMatrix : public vector<term>{
public:
	SparseMatrix(void);
	bool enterSparseMx (SparseMatrix &a); //construct a sparse matrix
	friend
		SparseMatrix operator + (const SparseMatrix &a, const SparseMatrix &b);
	friend
		SparseMatrix operator * (SparseMatrix &x, SparseMatrix &y);
	int getvalue (SparseMatrix &c, int i, int j);
	friend
		ostream & operator << (ostream & os, SparseMatrix &a);
	~SparseMatrix (void); //destructor
private:
	int ncol; //the number of column
	int nrow; //the number of row
	int nelem; //the number of elements in the sparse matrix
};
