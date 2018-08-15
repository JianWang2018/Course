
g++ -c integrate.cpp 
g++ -c integrate_simpson.cpp 
g++ -c integrate_trapezoid.cpp 
g++ -c main.cpp
g++ -o a.out main.cpp integrate_simpson.cpp \
   integrate_trapezoid.cpp integrate.cpp
