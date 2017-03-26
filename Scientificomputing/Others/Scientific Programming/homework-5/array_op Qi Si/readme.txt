including
main.cpp
NewArray.h
NewArray.cpp

NewArray is a user defined class with:
default constructor;
copy constuctor;
assignment operator;
member functions like push_back, pop_back, remove, insert, capacity, size, Clear, show;
operator overloading:
+, [], <<

Use non-member friend function to define the overloaded operator
+ and <<
declare as
friend NewArray operator + (const NewArray &a, const NewArray &b);
and
friend std::ostream & operator << (std::ostream & os, NewArray & s);

notice that the left operand of << must be of class ostream, so it must be a friend.
The operator + implies the addition of vectors with the same size.
The operator[] return a reference to a int. 