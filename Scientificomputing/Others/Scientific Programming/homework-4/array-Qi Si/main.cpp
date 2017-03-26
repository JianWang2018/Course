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
	//call copy constructor
	NewArray x(a);
	cout << "the vector is as follows:" << endl;
	x.show();
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
	
	cout << "remove all the elements form the array\n";
	a.Clear();
	//a.show();

	return (0);
}