#include "SparseMatrix.h"

SparseMatrix::SparseMatrix(void){
	nrow = 0;
	ncol = 0;
	nelem = 0;
}

SparseMatrix::~SparseMatrix(void)
{
}

bool SparseMatrix :: enterSparseMx (SparseMatrix &a){
	cout << " enter the number of rows, columns, and elements " << endl;
	cin >> nrow >> ncol >> nelem;
	cout << endl;
	a.push_back(term());
	a[0].row= nrow;
	a[0].col = ncol;
	a[0].value = nelem;
	int row, col;
	double elem;
	for (int i = 1; i <= a[0].value; i++){
		cout << " please enter the " << i << "nonzero element: ";
		cin >> row >> col >> elem;
		cout << endl;
		a.push_back(term());
		a[i].row = row;
		a[i].col = col;
		a[i].value = elem;
	}
	return true;
}

SparseMatrix
	operator + (const SparseMatrix &a, const SparseMatrix &b){
		SparseMatrix c;
		int i=1, j=1, k=1;
		if (a[0].row != b[0].row || a[0].col != b[0].col){
			cout << "This two matrices cannot be added together!" << endl;
		}
		else{
		c.push_back(term());
		c[0].row = a[0].row;
		c[0].col = a[0].col;
		c[0].value = 0;
		while (i <= a[0].value && j <= b[0].value){
		if (a[i].row == b[j].row){
			if (a[i].col < b[j].col){
				c.push_back(term());
				c[k].row = a[i].row;
				c[k].col = a[i].col;
				c[k].value = a[i].value;
				k++;
				i++;
			}
			else if (a[i].col > b[j].col){
				c.push_back(term());
				c[k].row = b[j].row;
				c[k].col = b[j].col;
				c[k].value = b[j].value;
				k++;
				j++;
			}
			else {
				c.push_back(term());
				c[k].row = b[j].row;
				c[k].col = b[j].col;
				c[k].value = a[i].value + b[j].value;
				k++;
				i++;
				j++;
			}
		}
		else if(a[i].row < b[j].row){
			c.push_back(term());
			c[k].row = a[i].row;
			c[k].col = a[i].col;
			c[k].value = a[i].value;
			k++;
			i++;
		}
		else {
			c.push_back(term());
			c[k].row = b[j].row;
			c[k].col = b[j].col;
			c[k].value = b[j].value;
			k++;
			j++;
		}
	}
		while( i <= a[0].value){
			c.push_back(term());
			c[k].row = a[i].row;
			c[k].col = a[i].col;
			c[k].value = a[i].value;
			i++;
			k++;
		}
		while( j<= b[0].value){
			c.push_back(term());
			c[k].row = b[j].row;
			c[k].col = b[j].col;
			c[k].value = b[j].value;
			j++;
			k++;
		}
		}
		c[0].value = k-1;
		return c;
}
int SparseMatrix::getvalue (SparseMatrix &c, int i, int j){
	int k = 1;
	while (k <= c[0].value){
		if (c[k].row == i && c[k].col == j)
			return c[k].value;
		k++;
	}
	return 0;
}
SparseMatrix
	operator * (SparseMatrix &x, SparseMatrix &y){
		SparseMatrix c;
		int row, col, i, k;
		if (x[0].col != y[0].row)
			cout << " This two metrices cannot multiplying" << endl;
		else{
			c.push_back(term());
			c[0].row = x[0].row;
			c[0].col = y[0].col;
			c[0].value = 0;
			k = 1;
			for (row = 1; row <= x[0].row; row++){
				for (col = 1; col <= y[0].col; col++){
					double temp = 0;
					for (i = 1; i <= x[0].col; i++)
						temp = temp + x.getvalue(x,row,i)* y.getvalue(y,i,col);
					if (temp != 0){
						c.push_back(term());
						c[k].row = row;
						c[k].col = col;
						c[k].value = temp;
						k++;
					}
				}
			}
			c[0].value = k-1;
		}
		return c;
}
ostream & operator << (ostream & os, SparseMatrix &a){
	cout << "row" << "col" << "value" << endl;
	int size = a[0].value;
	for (int i = 0; i<= size; i++){
		cout << a[i].row << " " << a[i].col << " " << a[i].value << endl;
	}
	return os;
}
