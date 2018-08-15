#pragma once
class Array
{
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
private:
	int num;
	int * data;
	int cap;
	void check_capacity();
};

