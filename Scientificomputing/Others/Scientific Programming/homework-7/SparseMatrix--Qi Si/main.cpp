#include <iostream>
#include <iomanip>
#include <vector>
#include "SparseMatrix.h"

using namespace std;

int main(){
	SparseMatrix A, B, C, D;
	A.enterSparseMx(A);
	B.enterSparseMx(B);
	C = A+B;
	D = A*B;

	cout << " matrix A is: " << A << endl;
	cout << " matrix B is: " << B << endl;	
	cout << " The addition of A and B is: " << C << endl;
	cout << " The multiplication of A and B is: " << D << endl;
	return 0;
}