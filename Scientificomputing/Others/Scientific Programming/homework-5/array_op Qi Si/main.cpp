#include <iostream>
#include "NewArray.h"

using namespace std;

int main()
{
	NewArray a;
	NewArray b;
	NewArray c;
	for (int i=1; i<=10; i++)
	{
		a.push_back (i);
	}
	cout << "initial vector a is as follows:" << endl;
	a.show();
	for (int i =1; i <=20; i++)
	{
		c.push_back(i);
	}
	cout <<"using the assignment operator\n";
	a.operator=(b.operator=(c));
	cout << "now new array a is equal to\n";
	a.show();
	cout << "add 47 to the end of the array\n";
	a.push_back(47);
	a.show();

	cout << "remove the last element\n";
	a.pop_back();
	a.show();

	cout<< "remove the integer with index 4 from the array\n";
	a.remove(4);
	a.show();

	cout<< "insert 56 at the position 8\n";
	a.insert(56, 8);
	a.show();

	cout << "the capacity of the vector is: " << a.capacity() << endl; 
	cout << "the size of the vector is: " << a.size() << endl;
	
	//cout << "remove all the elements form the array\n";
	//a.Clear();

	//check the operator
	NewArray x;
	NewArray y;
	NewArray z;
	x.push_back(1);
	x.push_back(11);
	x.push_back(111);
	x.push_back(12);
	x.push_back(13);
	for (int i =1; i <=5; i++)
	{
		y.push_back(i);
	}
	z=x+y;
	cout << "the array x+y is as follows:" << endl;
	z.show();

	cout << " array x is: " << x << endl;
	cout << " array y is: " << y << endl;
	return (0);
}