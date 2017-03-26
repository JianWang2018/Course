//----------------------------------------------------------------------
#ifndef _GRID_H_
#define _GRID_H_

#include <stdio.h>

class Grid 
{
public:
	int nx, ny;
	float var[100];  // array of 100 floats

public:
	Grid(); // default constructor

public:
	Grid(int dum, int ny); // constructor
	void print(char* msg);
    void setNx(int nx);
};

#endif
//----------------------------------------------------------------------
