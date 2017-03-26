#ifndef NEWARRAY_H
#define NEWARRAY_H

#include <iostream>
#pragma once

class NewArray
{
public:
	NewArray();
	// default constructor
	NewArray(const NewArray & s);
	// copy constructor
	NewArray & operator = (const NewArray & rhs); 
	// assignment operator
	int push_back(int i);//add an integer to the end of the array
	int pop_back(void); //remove the last element
	int remove(int i); // remove the integer with index i from the array
	int insert(int n, int i); // insert an integer "num" at the position "i"
	int capacity(void); // return the capacity of the vector
	int size(void); // return the size of the vector
	int Clear(void); // remove all the elements from the array.
	void show();

	//operator overloading
	friend
		NewArray operator + (const NewArray &a, const NewArray &b);// plus elements
	int & operator [] (int index);
	friend
		std::ostream & operator << (std::ostream & os, NewArray & s); //output
	~NewArray(void);// destructor

private:
	int num; // store the number of integers in this array
	int * data; // pointer to memory block store the integers
	int cap; // the size of the memory block
	void check_capacity();
};

#endif