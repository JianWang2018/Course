now main.cpp

#include <cstdlib>
#include <iostream>
#include "array.h" // " " not <>
using namespace std;

int main(int argc, char *argv[])
{
Array a1(7),a2 (-1),a4; //changed a2
cout<<"Input "<<a1.getSize()<<" integers for Array object a1 and "<<a2.getSize()<<" integers for Array objecta2\n";
cin>>a1>>a2;
cout<<"a1 and a2 are\n";
cout<<a1<<endl<<a2;
cout<<"a1!=a2 : "<<(a1!=a2)<<endl;
cout<<"a1 ==a2: "<<(a1==a2)<<endl;
cout<<"Printing a1[5] : "<<a1[5]<<endl;
Array a3(a1); 
a4 = a3;

system("PAUSE");
return EXIT_SUCCESS;
}