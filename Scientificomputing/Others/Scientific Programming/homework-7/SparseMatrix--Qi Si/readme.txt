-Introduction:
Store the sparse matrix in a more efficient way: only store the nonzero elements. And redefine the operator + and * based on the definition.
-Design and Implementation
This code including a class SparseMatrix() and a struct term().
Each term included 3 elements: int col; int row; and double value, which store the col, row and value for each nonzero element separately.
The class SparseMatrix includeing member functions: enterSparseMx(SparseMatrix &a) and getvalue (SparseMatrix &c, int i, int j) which return the nonzero elements in matrix c.
Then we reloaded the operator +, *, <<.

For the structure of the storage of SparseMatrix a, a[0] stores the number of rows, columns and nonzero elements in the primitive matrix. In this case, we can easily judge whether these two matrices can do addition or multiplication.
And then for i=1:nelem, a[i] stores the row, column and value for the corresponding element.

Considering the operation +, firstly, we consider 
while (i <= a[0].value && j <= b[0].value)
Then compare the rows of matrices a and b, if they are equal, compare the columns , store the less one's value. If both rows and columns are equal, store a.value+b.value
if(a[i].row < b[j].row): store a[i];
else: store b[j].

Then we need to store the rest values of a and b.

Considering the operation *, use the function getvalue() to consider whether the value is zero, store the nonzero one.
-Results
Using triples to store the sparse matrix, that will saving lots of storage, and then redefine the operation + and * to calculate the addition and multiplication of two matrices.
-Conclusions
Since we didn't judge whether the addition of a[i].value and b[j].value is zero or not, the addition function may require a little more storage than just store the nonzero elements of A+B.
And we need to input the elements of the matrices in order, that is, row by row.