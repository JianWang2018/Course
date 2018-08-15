#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
using namespace std;

class Array{
  friend ostream & operator << ( ostream &, const Array & );
  friend istream & operator >> ( istream &, Array &);
  private:
         int size;
         int * ptr;
  public:
         Array ( int = 10 );
         Array ( const Array & ); //copy constructor
         ~Array ();
         const Array &operator=( const Array & ); 
         bool operator == ( const Array & ) const; 
         bool operator != ( const Array & ) const;
         const int operator [] (int) const; 
         int getSize() const;            
};

#endif