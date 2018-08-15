/*Description: Array definition header file
* Author: Hongzhuan LEI
* Institution: Department of Scientific Computing, FSU 
* Date: 10/02/2015 11:58:20 
*/
#ifndef ARRAYCLASS_DEFINITION_SC_FSU_HL
#define ARRAYCLASS_DEFINITION_SC_FSU_HL


class Array
{
public:
	Array();						//default constructor
	Array(const Array &a);			//copy constructor
	~Array();						//destructor

	Array& operator=(const Array &a);//assignment operation
	int	   operator[](int i) const;		 //get element at position "i"

public:
	void push_back(int i);			//add an element to the end of the array
	void pop_back();				//remove the last element
	void remove(int i);				//remove the element with index i from the array
	void insert(int num, int i);	//insert an integer"num" at the position "i"
	unsigned int  capacity() const;	//return the capacity of the array
	unsigned int  size() const;		//return the size of the array
	void clear();					//remove all elements from the array

protected:
	void InitData(int iNum, int iCapacity);//initialize number, capacity and pointer data 
	void FreeData();				//free memory
	void DoubleCapacity();			//double the capacity
	void _insert(int num, int i);	//insert element "num" at position of "i"

private:
	unsigned int m_iNum;			//the number of integers
	int *m_pData;					//pointer to memory block store the integers
	unsigned int m_iCapacity;		//the size of the memory block

};



#endif