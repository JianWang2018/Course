// Array.cpp : Defines the entry point for the console application.
//

#pragma once
#include "stdafx.h"
#include <math.h>
#include <iostream>
#include "TemplateClassArray.h"
using namespace std;




int _tmain(int argc, char * argv[])//for testing
{
	Array<int> a;//int type
	//Array<float> a;//float type
	//Array<double> a;//double type
	a.push_back(0.5);
	a.push_back(1.5);
	a.push_back(2.5);
	a.push_back(3.5);
	a.push_back(4.5);
	a.remove(2);
	a.insert(3, 10.5);

	cout <<"a: size is: "<< a.size()<<" " << "a: capacity is: "<<a.capacity() << endl;

	Array<int> b = a;
	//Array<float> b = a;
	//Array<double> b = a;
	cout <<"b: size is: "<< b.size() << " " << "b: capacity is: " <<b.capacity() << endl;

	//test the operator + and << of Array
	cout << "the elements of Array a+b are: \n";
	for (int i = 0; i < a.size(); i++)
	{
		cout << (a + b)[i] << endl;
	}

	//test the operator [] for Array a
	cout << "the elements of Array a are: \n";
	for (int i = 0; i < a.size(); i++)
	{
		cout << a[i] << endl;
	}

	return 0;
}