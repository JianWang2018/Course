#pragma once

#include <iostream>
using namespace std;
class Array
{
public:
	Array();
	~Array();
	Array(const Array &);

	void push_back(int i);
	void pop_back();
	void remove(int i);
	void insert(int n, int i);

	int capacity() { return cap; };
	int size() { return num; };
	void clear() { num = 0;	};

	Array operator+(const Array& s);
	int & operator[](int element);
	friend ostream& operator<<(ostream& os, Array& s);

private:
	int num;
	int * data;
	int cap;
	void check_capacity();
};

