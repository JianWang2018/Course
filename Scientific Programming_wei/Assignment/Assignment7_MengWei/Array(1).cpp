// Array.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Array.h"


int _tmain(int argc, char * argv[])
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

	cout << a+b;
	return 0;
}



Array::Array()
{
	num = 0;
	cap = 1;
	data = new int[1];
}


Array::~Array()
{
	delete[] data;
}

Array::Array(const Array & a)
{
	num = a.num;
	cap = a.cap;
	data = new int[cap];
	memcpy(data, a.data, sizeof(int)*num);
}

void Array::check_capacity() {
	if (num >= cap) {  //not enough cap, double it
		cap *= 2;
		int * tmp = new int[cap];
		memcpy(tmp, data, sizeof(int)*num);
		delete[] data;
		data = tmp;
	}
}

void Array::push_back(int i)
{
	check_capacity();
	data[num] = i;
	num++; 
}

void Array::pop_back()
{
	num--;
	if (num < 0) num = 0;
}

void Array::remove(int i)
{
	if (i >= 0 && i < num) {
		int j;
		for (j = i; j < num-1; j++) data[j] = data[j + 1];
		num--; 
	}
}

void Array::insert(int n, int i)
{
	if (n >= 0 && n < num) {
		check_capacity();
		int j;
		for (j = num; j > n; j--) data[j] = data[j - 1];
		data[n] = i;
		num++;
	}
}


Array Array::operator+(const Array& s)
{
	if (num != s.num) {
		printf("Not the same size!\n");
		return *this; //return self.
	}
	else {
		Array tmp(s);
		int i;
		for (i = 0; i < num; i++) tmp.data[i] += data[i];
		return tmp;
	}
}

int & Array::operator[](int element)
{
	return data[element];
}

ostream& operator<<(ostream& os, Array& s)
{
	int i;
	for (i = 0; i < s.num; i++) os << s.data[i] << " ";
	return (os << endl);
}
