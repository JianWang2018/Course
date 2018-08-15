#pragma once
class Array
{
friend ostream & operator << ( ostream &, Array & );//operator <<
friend Array operator + (int Array& a, int Array& b);//operator +
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
	
	int& operator [] (int) ; //operator []
	
private:
	int num;
	int * data;
	int cap;
	void check_capacity();
};

