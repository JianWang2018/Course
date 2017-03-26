OPT=-DSUBOPER
echo "Array3D"
g++ -c Array3D.cpp
echo "Array2D"
g++ -c Array2D.cpp ${OPT}
echo "main"
g++ -c main.cpp  
g++ -o a.out main.o Array3D.o Array2D.o
