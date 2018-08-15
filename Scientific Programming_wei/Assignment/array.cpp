
# include<iostream>
# include<conio.h>
# include<stdlib.h>
# include<math.h>
# include<array>

using namespace std;

class Array
{
public:
	int num;                                           // to store number of intergers in an Array
	int *data;                                         // pointer to memory block store the integers 
	int ary[100];
	int cap;                                           // the size of the memory block
	Array();                                           // default constructor
	~Array();                                          // default destructor    
	Array( const Array &obj);                          // copy constructor
	Array& operator = (const Array &opr);                 // assignment operator 
	void push_back(int i);
	void pop_back();
	void remove(int i);
	void insert(int num1,int i);
	int capacity(void);
	int size(void) const;
	void clear(void);
//////////////////////////////////////////////////////////////////////////
	int operator[](int index) const
	{
		//check index if valid or not
		return data[index];
	}

	int& operator[](int index)
	{
		//check index if valid or not
		return data[index];
	}

	Array& operator+(const Array& ap)
	{
		if (size() != ap.size())
			throw invalid_argument("size diff");

		Array *C = new Array;
		*C = ap;
		for (int i=0; i<size(); ++i)
		{
			(*C)[i] = (*this)[i] + ap[i];
		}

		return *C;

	}

	friend ostream &operator<<( ostream &output,const Array &D )
	{ 
		for (int i=0; i<D.size(); ++i)
		{
			output<<D[i]<<" ";
		}
		return output;            
	}
//////////////////////////////////////////////////////////////////////////

};

Array::Array()
{
	num=0;                                                         // initialize size
	cap=1;                                                         // initialize capacity
	data = 	new int [cap];                                         // assigning capacity with pointer
	//cout<<"Constructor created with an empty vector";
}
Array::Array( const Array &obj)
{
	num=obj.num;                                                   // object accessing the num from class
	cap=obj.cap;                                                   // object accessing the cap from class
	data=new int [cap];
	for (int j =0; j<cap-1; ++j)      // sicne this is copy , we need to copy whole array even no element is there, since in order to maintain the same array
	{
		data[j] = obj.data[j];		                           // object accessing the data pointer from class
	}
}
Array& Array::operator=( const Array &opr)
{
	num=opr.num;                                                   // object accessing the num from class
	cap=opr.cap;                                                   // object accessing the cap from class
	data=new int [cap];
	for (int j =0; j<cap-1; ++j)      // sicne this is copy , we need to copy whole array even no element is there, since in order to maintain the same array
	{
		data[j] = opr.data[j];		                           // object accessing the data pointer from class
	}
	return (*this);
}
Array::~Array() 
{
	delete[] data;                                             // call of destructor for array pointer
}
void Array::push_back(int i)
{
	num=num+1;                                                      // increament for last element plugging
	if(num<=cap)                                                    // object accessing the num from classcheck with size and capacity of array
	{
		data[num-1]=i;                                              // plugging the last number
	}
	else
	{
		cap=cap*2;                                                  // increamenting capacity 
		int *temp = new int [cap];                                  // new temp to copy older data before creating new one with new capacity
		for (int j =0; j<num-1; ++j)
		{
			temp[j] = data[j];		                                // data copying element wise
		}
		delete[] data;                                              // delete older array
		data=new int [cap];                                         // defining new data with new capacity
		for(int k=0;k<num-1;k++)
		{
			data[k]=temp[k];                                        // copying from temp to new data
		}
		delete[] temp;                                              // delete temp
		data[num-1]=i;                                              // new last element , plugging with i
	}

}
void Array::pop_back()
{
	num=num-1;                                                      //just decrement size by 1 so last element is not in existence
}
void Array::remove(int i)
{
	if(i>num-1 || i<0)                                              // check for procided index is valid and within range or not
	{
		for(int k=i;k<num-1;k++)
		{
			data[k]=data[k+1];                                          //only shifting every element behind 
		}
		num=num-1;                                                      // hence one vacant place at last position hence decreased
	}
	else
	{
		cout<<"enter index less than size and it should be positive";   // error message for exception handling
	}
}
void Array::insert(int num1, int i)
{
	if(i>num-1 || i<0)                                                         // checking for valid index
	{
		cout<<"enter index less than size and it should be positive";         // message for exception handling     

	}
	else
	{
		num=num+1;                                                           // creating room for another new element to insert at i'th position
		if(num>cap)
		{
			int *temp = new int [cap];                                        //  temp to copy data
			for (int j =0; j<num-1; ++j)
			{
				temp[j] = data[j];		                                      // copying to temp from old data
			}

			delete[] data;                                                    //  delete old data
			data=new int [cap];                                               // new data with capacity
			for(int k=0;k<num-1;k++)
			{
				data[k]=temp[k];                                               // copying back from temp to data
			}
			delete[] temp;                                                     // delete temp
			data[num-1]=i;                                                     // new last element assignment
		}
		for(int k=num-1;k>=i;k--)
		{		
			if(k==i)
			{
				data[k]=num1;                                              // at last of this decrementing for loop new element would get inserted here
			}   
			else
			{
				data[k]=data[k-1];                                         // till then a shifting from one box toward the next box
			}
		}
	}
}

int Array::capacity()
{
	return cap;                                                                 //  return the capacity of array
}
int Array::size() const
{
	return num;                                                                  // return size of array
}
void Array::clear()
{
	num=0;                                                                      // plug num i.e. size as zero hence no element inside

}


int main()
{
	Array a1;
	//a1.push_back(5);                                                   // multiple push back required or can insert in for loops
	for (int i=0; i<5; ++i)
	{
		a1.push_back(i+1);
	}
	cout<<"Array a1:"<<endl;
	cout<<a1<<endl;
	Array a2(a1);
	cout<<"Array a2:"<<endl;
	for (int i=0; i<a2.size(); ++i)
	{
		cout<<a2[i]<<" ";
	}
	Array a3;
	a3 = a2 + a1;
	cout<<endl<<"Array a3:"<<endl;
	cout<<a3<<endl;

// 	a1.pop_back();
// 	a1.remove(1);
// 	a1.insert(2,8);
// 	a1.capacity();
// 	a1.size();
// 	a1.clear();

	system("pause");
	return 0;
}

