#include <iostream>
#include <vector>
using namespace std;

//triple struct
struct term {
	int col;//the column of nonzero elements
	int row;//the row of nonzero elements
	double value;//the value of nonzero elements
	term(int i, int j, double val)
	{
		row = i;
		col = j;
		value = val;
	}
};

//the declaration of Class SparseMatrix
class SparseMatrix
{
	friend SparseMatrix operator +(const SparseMatrix&m1, const SparseMatrix&m2);//add two sparse matrix together
	friend SparseMatrix operator *(const SparseMatrix&m1, const SparseMatrix&m2);//sparse matrix times sparse matrix
public:
	SparseMatrix(int s1, int s2); //default constructor
	SparseMatrix(const SparseMatrix &);//copy constructor
	SparseMatrix& operator=(const SparseMatrix &);//assignment operator
	vector<term> data;
	int nrow, ncol;    //the number of row and column of Sparse Matrix
	double get(int i, int j);//get the i-th row, j-th column element
	void set(int i, int j, double val);//change the i-th row, j-th column element by value val
};
