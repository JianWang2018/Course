and now array.cpp

#include <iostream>
using namespace std;
#include "array.h"

Array::Array (int sze ){ //default constructor edited
         size = (sze > 0 ? sze : 10);
         ptr = new int [ size ];
         for (int i = 0;  i < size; i++)
             ptr[ i ] = 0; //initial values
}
Array::Array (const Array & arr ): size(arr.size){
         ptr = new int [size];
         for ( int i = 0; i< size; i++)
             ptr [ i ] = arr.ptr [ i ];
}
Array::~Array(){
         delete [] ptr;
}
const Array &Array :: operator= ( const Array & right){//IMPO
         if(&right != this){ //edited self assignment test
                   if(size != right.size){//diff sized arrays
                           delete [] ptr; //reclaim space
                           size = right.size; 
                           ptr = new int [ size ]; //space created
                   }
         }
         for(int i=0; i<size; i++)
                 ptr[ i ] = right.ptr[ i ];
         return *this;     //enables cascading a=b=c       
}
bool Array::operator == ( const Array & right) const{
         if ( size != right.size )
            return false;
         for ( int i =0; i < size; i++ ){
             if ( ptr [ i ] != right.ptr[ i ] )
                return false;
         }
         return true;
 }
bool Array::operator != ( const Array & right ) const{ //edited
         return ! (*this == right);
}
const int Array::operator [] (int subscript) const{
         if(subscript >=0 && subscript < size)
            return ptr[ subscript ];      
}
int Array::getSize() const{ return size; }  
//friend functions not in .h
ostream & operator << ( ostream & output, const Array & array){
         for (int i = 0; i < array.size; i++)
             output << array.ptr[i] ; 
}
istream & operator >> ( istream & input, Array & array){
         for (int i = 0; i < array.size; i++)
             input >> array.ptr[i];
}