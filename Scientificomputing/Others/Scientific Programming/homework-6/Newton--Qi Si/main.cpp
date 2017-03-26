#include <iostream>
#include "MyFunction.h"
#include "SinFunction.h"
#include <cmath>

using namespace std;

int main(){
	SinFunction x;
	cout << x.getRoot() << endl;

	MyFunction y;
	cout << y.getRoot() << endl;

	return 0;
}
