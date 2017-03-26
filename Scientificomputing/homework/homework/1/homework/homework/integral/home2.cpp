#include <iostream>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

using namespace std;

int main()
{
   FILE *myfile1;
   myfile1 = fopen ("home1.txt", "w");  
   
   double f1,f2,f3,t,dt,a,b,p,sol;
   int T,n,i;
   struct timeval start1, end1,start2, end2;
   double mtime, starttime, endtime; 

   t=-3;
   T=10;
   sol=0;

   cout << "Enter the number of interval: " ;
   cin >> n;   

   dt=(double)(T-t)/(double)n;
    cout << "dt= "<<dt<<endl;   
        
    cout<< "The value entered is : " << n<<endl;
    cout<< "Integral from -3 to 10 with  "<< n <<"  intervals."<< endl;
    fprintf (myfile1, "n\t\tIntegral Value\t\tTime(ms)\t\t\n");  

    gettimeofday(&start2, NULL); 
    for (i=1;i<=n;i++)
      {
        t=t+dt;
        gettimeofday(&start1, NULL); 
        a=t-dt;
        b=t;
        f1=1/(sin(2*a) + 2)-(2*a*cos(2*a))/pow((sin(2*a) + 2),2);
        f2=1/(sin(a+b) + 2)-((a+b)*cos(a+b))/pow((sin(a+b) + 2),2);
        f3=1/(sin(2*b) + 2)-(2*b*cos(2*b))/pow((sin(2*b) + 2),2);
        p=(b-a)/6*(f1+4*f2+f3);
        sol=sol+p;

        gettimeofday(&end1, NULL);        
        starttime  = (start1.tv_sec*1000)+ (start1.tv_usec/1000);
        endtime = (end1.tv_sec*1000) + (end1.tv_usec/1000);
        mtime = endtime-starttime;

        fprintf (myfile1, "%6.3f-%6.3f\t\t%6.4f\t\t\t%6.4f\n",a,b,sol,mtime);     
       }
     gettimeofday(&end2, NULL);  
     starttime  = (start2.tv_sec*1000)+ (start2.tv_usec/1000);
     endtime = (end2.tv_sec*1000) + (end2.tv_usec/1000);
     mtime = endtime-starttime;  
     fprintf (myfile1, "The total value is %.4f.",mtime);  ;  
     return 0;             
}

