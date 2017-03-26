#include <stdio.h>
#include <stdlib.h>
#include "timing.h"

using namespace std;
    
int main()
{
    //ofstream myfile1;
    //myfile1.open ("home1.txt");
    
	Timings ts;
	Timer t1(ts, "newtim");
 	        
    int n,steps;
    steps=20;
    
    cout << "Enter the value of n (Press 0 to quit): " ;
    cin >> n;
    
    cout<< "The value entered is : " << n<<endl;
    cout<< "Counting from 1 thru "<< n << endl;

    float count[n+1];

    for (int j=1; j<=steps; j++)
      {
       t1.start(Timer::SELF);
        for (int i=0; i < n; i++)       
          {
            count[i] = i+1;
            cout<< "count[" << i << "] = " << count[i] << endl;
           } 
       t1.end();       
       }
     ts.dumpTimings(steps,n);
     return 0;             
}

