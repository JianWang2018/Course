// Array.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <math.h>
#include "Array.h"


int _tmain(int argc, char * argv[])//for testing
{
	Array a;
	a.push_back(0);
	a.push_back(1);
	a.push_back(2);
	a.push_back(3);
	a.push_back(4);
	a.remove(2);
	a.insert(3, 10);
	
	printf("a: size is %d, capacity is %d\n", a.size(), a.capacity());

	Array b = a;
	printf("b: size is %d, capacity is %d\n", b.size(), b.capacity());
	
	//test the operator + of Array
	Array c= +(a,b);
	for(int i=0;i<c.size;i++)
	{
		printf("the elements of Array c are: %d\n", c.data[i]);
	}
	
	
	//test the operator [] for Array a
	pritf("The second element of Array a is: %d/n", a[](1));
	
	
	//test the operator << for Array b
	<< ( ostream & output, Array & b);
	
	
	return 0;
}



Array::Array() //default constructor
{
	num = 0;
	cap = 1;
	data = new int[1];
}


Array::~Array()//default destructor
{
	delete[] data;
}

Array::Array(Array & a)//copy constructor
{
	num = a.num;
	cap = a.cap;
	data = new int[cap];
	memcpy(data, a.data, sizeof(int)*num);
}

void Array::check_capacity() //define check_capacity function
{
	if (num >= cap) {  //not enough cap, double it
		cap *= 2;
		int * tmp = new int[cap];
		memcpy(tmp, data, sizeof(int)*num);
		delete[] data;
		data = tmp;
	}
}

void Array::push_back(int i)//define push_back function
{
	check_capacity();
	data[num] = i;
	num++; 
}

void Array::pop_back()//define pop_back function
{
	num--;
	if (num < 0) num = 0;
}

void Array::remove(int i)//define remove function
{
	if (i >= 0 && i < num) {
		int j;
		for (j = i; j < num-1; j++) data[j] = data[j + 1];
		num--; 
	}
}

void Array::insert(int n, int i)//define insert function
{
	if (n >= 0 && n < num) {
		check_capacity();
		int j;
		for (j = num; j > n; j--) data[j] = data[j - 1];
		data[n] = i;
		num++;
	}
}

//define operator + for Array Class
Array operator+(int Array& a, int Array& b)
{
	Array temp;
	delete[] temp.data;
	temp.data= new int[a.size];
	if(a.size==b.size)
	{
		for(int i=0; i< a.size; i++)
		{
			temp.data[i]=a.data[i]+b.data[i];
		}
		}
		return temp;
}

//define operator [] for Array Class
int& Array::operator [] (int subscript) 
{
         if(subscript >=0 && subscript < a.size)
            return data[ subscript ];      
}

//define friend function operator << for Array Class
ostream & operator << ( ostream & output, Array & a)
{
         for (int i = 0; i < a.size; i++)
            return (output << a.data[i]) ; 
}
