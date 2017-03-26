In this project, there are 3 files :
class declaration--a head file called NewArray.h;
class definition--NewArray.cpp;
main function--main.cpp.

In NewArray.h, declare a concrete data types including the following member functions:
A default constructor: NewArray();
A copy constructor: NewArray(const NewArray &s);
An assignment operator: NewArray & operator = (const NewArray & rhs);
and
A destructor: ~NewArray(void);

with following member functions:

int push_back(int i); using cap = cap*2 to get rid of the overflow.
int pop_back();
int remove(int i);
int insert(int n, int i); which insert number "n" at the position "i".
int capacity();
int size();
int Clear();
void show();

Data members are private in the class: 
int num; int cap; int *data

In the main.cpp, we use push_back(i) to initial the object of class NewArray.
Then use all the member functions to test the class's definition,
and call show() to output the array.

In pop_back() function, create a temporary array named *temp to store all the elements
in data, then delete old data and create a new one which only have num-1 elements.