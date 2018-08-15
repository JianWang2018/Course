// ArrayClass.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ArrayClass.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

//////////////////////////Array////////////////////////////////////////////////
Array::Array()
{//default 
	InitData(0, 1);
}

Array::Array( const Array &a )
{//define an new array using existed array a
	InitData(a.m_iNum, a.m_iCapacity);
	memcpy(m_pData, a.m_pData, a.m_iCapacity*sizeof(int));
}

Array::~Array()
{
	FreeData();//free memory
}

void Array::InitData(int iNum, int iCapacity)
{//initialize number, capacity and pointer data 
	m_iNum = iNum;
	m_iCapacity = iCapacity;
	m_pData = new int[m_iCapacity];// allocate new memory
}

void Array::FreeData()
{
	delete [] m_pData;//free memory
}

Array& Array::operator=( const Array &a )
{
	if (this == &a)//check if is self
		return (*this);

	FreeData();// free old memory
	InitData(a.m_iNum, a.m_iCapacity);// initial data 
	memcpy(m_pData, a.m_pData, m_iCapacity*sizeof(int));//copy memory

	return (*this);
}

int Array::operator[]( int i ) const
{
	return m_pData[i];
}

void Array::DoubleCapacity()
{
	m_iCapacity *= 2;//double the old capacity
	int *pTemp = new int[m_iCapacity];
	memcpy(pTemp, m_pData, m_iNum*sizeof(int));//copy old data
	FreeData();//free old memory block
	m_pData = pTemp;//new memory block 
}

void Array::_insert(int num, int i)
{
	if (m_iCapacity == m_iNum)
	{//need to increase the memory block size 
		DoubleCapacity();
	}

	//move the followed elements backward
	for (unsigned int k = m_iNum; k >i; --k)
	{
		m_pData[k] = m_pData[k-1];
	}
	//set the new element at the position of i
	m_pData[i] = num; 
	//increase the number of elements
	++m_iNum;
}

void Array::push_back( int i )
{
	_insert(i, m_iNum);	//insert element "i" at the back
}

void Array::insert( int num, int i )
{
	if (i < 0 || i >= m_iNum)// not a valid index
		return;

	_insert(num, i);	
} 

void Array::pop_back()
{
	if (m_iNum >= 0 )//not empty
	{
		--m_iNum;
	}
}

void Array::remove( int i )
{
	if (i < 0 || i >= m_iNum)// not a valid index
		return;

	//move the followed elements forward
	for (unsigned int k = i; k < m_iNum; ++k)
	{
		m_pData[k] = m_pData[k+1];
	}
	//number decrease 1
	--m_iNum;
}

unsigned int Array::capacity() const
{
	return m_iCapacity;
}

unsigned int Array::size() const
{
	return m_iNum;
}

void Array::clear()
{
	m_iNum = 0;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////Test code////////////////////////////////////////////////////
void _TestOneArray(Array &arr)
{
	printf("array elements: \n");
	for (int i=0; i<arr.size(); ++i)
	{
		printf("%d   ", arr[i]);
	}
	printf("\narray size: %d     capacity: %d \n", arr.size(), arr.capacity());
	
	for (int i=0; i<20; ++i)
	{
		//test push_back() function
		arr.push_back(i);
		printf("\npush_back %d\n", i);
		printf("array size: %d     capacity: %d \n", arr.size(), arr.capacity());
	}
	printf("\narray elements: \n");
	for (int i=0; i<arr.size(); ++i)
	{
		printf("%d   ", arr[i]);
	}
	printf("\n");
	
	//test insert() function
	arr.insert(12, 2);
	printf("\ninsert value %d at position %d\n", 12, 2);
	printf("array elements: \n");
	for (int i=0; i<arr.size(); ++i)
	{
		printf("%d   ", arr[i]);
	}
	printf("\narray size: %d     capacity: %d \n", arr.size(), arr.capacity());

	//test pop_back() function
	arr.pop_back();
	printf("\nAfter pop_back()\n");
	printf("array elements: \n");
	for (int i=0; i<arr.size(); ++i)
	{
		printf("%d   ", arr[i]);
	}
	printf("\narray size: %d     capacity: %d \n", arr.size(), arr.capacity());

	//test remove() function
	arr.remove(2);
	printf("\nAfter remove element at %d\n", 2);
	printf("array elements: \n");
	for (int i=0; i<arr.size(); ++i)
	{
		printf("%d   ", arr[i]);
	}
	printf("\narray size: %d     capacity: %d \n", arr.size(), arr.capacity());

	//test clear() function
	arr.clear();
	printf("\nAfter clear() \n");
	printf("array size: %d     capacity: %d \n", arr.size(), arr.capacity());

}
//when we get out of this routine 'Test()', we can test the destruct function '~Array()'
void Test()
{
	//default constructor
	Array aFirst;
	printf("Define an empty array \n");
	_TestOneArray(aFirst);
	
	//copy constructor
	Array aSecond(aFirst);
	printf("\nDefine a new array from previous array By using copy construct\n");
	_TestOneArray(aSecond);


	Array aThird;
	//assignment operator
	aThird = aSecond;
	printf("\nDefine a new array from the previous array By using assignment operator\n");
	_TestOneArray(aThird);

}

int _tmain(int argc, _TCHAR* argv[])
{
	Test();

	system("pause");

	return 0;
}
