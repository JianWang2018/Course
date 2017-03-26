#include "NewArray.h"
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

NewArray::NewArray()
{
	num = 0;
	cap = 1;
	data = new int[1];
	cout << "the Default constructor been called" << endl;
}
NewArray::NewArray(const NewArray & s)
{
	num=s.num;
	cap=s.cap;
	data = new int[cap];
	/*for(int i=0; i<cap; i++)
	{
		data[i]=a.data[i];
	}*/
	memcpy(data, s.data, num*sizeof(int));
	cout<<"the Copy constructor been called" << endl;
}
NewArray & NewArray::operator=(const NewArray & rhs)
{
	//check assignment to self
	if (this == &rhs) return(*this);
    delete[]data; // delete old memory
	cap = rhs.cap;
	num = rhs.num;
	data = new int[cap];
	
	for(int i=0; i<num; i++)
	{
		data[i]=rhs.data[i];
	}
	
	return(*this);
}
NewArray::~NewArray()
{
	delete [] data; // with new[], free the memory
}
// member function

void NewArray::check_capacity()
{
	if(num >= cap)
	{
		cap = cap*2;
		int *temp = new int[cap];
		memcpy(temp, data, num*sizeof(int));
		delete []data;
		data = temp;
	}
}
	
// add an integer to the end of the array
int NewArray::push_back(int i)
{
	check_capacity();
		data[num] = i;
		num = num+1;
	return (0);
}
// remove the last element
int NewArray::pop_back()
{
	if(num == 0) 
	{
		cout <<" there is no element in the array" << endl;
	}
	else
	{
	int * temp = new int[cap];
	for(int i = 0; i < num-1; i++)
	{
		temp[i] = data[i];
	}
	delete [] data;
	num--;
	/*data = new int [cap];
	for (int i=0; i < num; i++)
	{
		data[i] = temp[i];
	}*/
	data = temp;
	}
	return (0);
}
// remove the integer with index i from the array
int NewArray::remove(int i)
{
	for(int j = i-1; j < num; j++)
	{
		data[j] = data[j+1];
	}
	data[num-1] = 0;
	num--;
	return (0);
}
// insert an integer "num" at the position "i"
int NewArray::insert(int n, int i)
{
	if ( i>num ) 
	{
		cout << " the index is not exist" << endl;
		exit(1);
	}
	if (cap = num)
	{
		cap = cap*2;
		int *temp = new int[cap];
		memcpy(temp, data, num*sizeof(int));
		delete [] data;
		data = new int[cap];
		data = temp;
	}
	// copy the data to a temp array
	int *y = new int[num];
	for ( int j=0; j<num; j++)
	{
		y[j]=data[j];
	}
	for ( int k=i; k<=num; k++)
	{
		data[k]=y[k-1];
	}
	data[i-1]=n;
	delete [] y;
	num=num+1;
	return(0);
}
// return the capacity of the vector
int NewArray::capacity()
{
	return cap;
}
// return the size of the vector
int NewArray::size()
{
	return num;
}
// remove all the elements from the array
int NewArray::Clear()
{
	delete []data;
	data=new int[cap];
	num=0;
	return 0;
}
void NewArray::show()
{
	for (int i=0; i<num; i++)
	{
		cout << data[i] << " ";
	}
	cout << endl;
}

// operator overloading
NewArray
	operator + (const NewArray & a, const NewArray & b)
{
	NewArray temp;
	delete [] temp.data;
	if (a.cap > b.cap)
		temp.data = new int [a.cap];
	else temp.data = new int [b.cap];
	temp.cap = max(a.cap, b.cap);
	if(a.num == b.num)
	{temp.num = a.num;
	for( int i = 0; i < temp.num; i++)
	{
		temp.data[i] = a.data[i]+b.data[i];
	}
	}
	else printf(" Error!");
	return (temp);
}

int & NewArray::operator[](int index)
{
	return(data[index]);
}

ostream & operator << (ostream & os, NewArray & s)
{
	for (int i = 0; i < s.num; i++)
	{
		os << s.data[i] << ' ';
	}
	return os;
}


