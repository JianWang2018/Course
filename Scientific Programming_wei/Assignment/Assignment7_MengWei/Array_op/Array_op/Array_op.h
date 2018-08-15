#pragma once
#include <iostream>
using namespace std;

class Array
{
friend ostream & operator << ( ostream &, Array & );//operator <<
//friend Array  operator + (int Array& a, int Array& b);//operator +
public:
	Array();
	~Array();
	Array(Array &);

	void push_back(int i);
	void pop_back();
	void remove(int i);
	void insert(int n, int i);

	int capacity() { return cap; };
	int size() { return num; };
	void clear() { num = 0;	};
	


	Array operator+ (const Array& a); // Concatenation 
	int& operator [] (int) ; //operator []
	
private:
	int num;
	int * data;
	int cap;
	void check_capacity();
};


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
		for (j = i; j < num - 1; j++) data[j] = data[j + 1];
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

Array Array::operator+(const Array & a)
{
	Array temp = *this;
	if (temp.num == a.num)
	{
		for (int i = 0; i< a.num; i++)
		{
			temp.data[i] = temp.data[i] + a.data[i];
		}
	}

	return temp;
}


//define operator [] for Array Class
int& Array::operator [] (int subscript)
{
	if (subscript >= 0 && subscript < num)
		return data[subscript];
}

//define friend function operator << for Array Class
ostream & operator << (ostream & output, Array & a)
{
	for (int i = 0; i < a.size(); i++)
		return (output << a.data[i]);
}

