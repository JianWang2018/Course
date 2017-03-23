//Header files
#include<cstdlib>
#include<iostream>
#include<cmath>

const double PI = std::atan(1.0)*4;
double a=0;
double b=PI;

// Define x(i)
double x(int i,int N)
{
	if(i<=N)
	{
		return a+i*(b-a)/N;
	}
	else
	{
		printf("error\n");
	}
}

// Trapezoidal rule


double Trapez(int N)
{
	int i;
	double F;
	double F0 = 0;
	for(i=0; i<= N-1; i++)
	{
		F0 += sin(x(i,N)) + sin(x(i+1,N));
	}
	F = 0.5*(b-a)*F0/(double)N;
	return F;
}

// Simpson's rule

double Simpson(int N)
{
	int i;
	double G;
	double G01 = 0;
	double G02 = 0;
	double G1;
	double G2;

	for( i = 1; i<= N-1; i++)
	{
		if( i%2 == 0)
		{
			G1 = G01 + 2*(sin(x(i,N)));
			G01 = G1;
		}
		else
		{
			G2 = G02 + 4*(sin(x(i,N)));
			G02 = G2;
		}
	}

		G= (b-a)*(sin(a)+sin(b)+G01+G02)/(3*(double)N);
		return G;

}

int main()
{
	int N[5]={20, 40, 80, 160, 320};
	double result1[5];
	double result2[5];
	double error1[5];
	double error2[5];
	int i;
	

	for(i=0; i<5; i++)
	{
		result1[i] = Trapez(N[i]);
		result2[i] = Simpson(N[i]);
		error1[i] = abs(2-result1[i]);
		error2[i] = abs(2-result2[i]);
		
		printf("When N=%d,the Trapezoidal Rule Result is %.10f; The error is %.10f\n", N[i],result1[i], error1[i]);
		printf("When N=%d,the Simpson's Rule Result is %.10f; The error is %.10f\n", N[i],result2[i], error2[i]);
	}

	double order1[4];
	double order2[4];
	for (i=0; i<4; i++)
	{
		order1[i] = log(error1[i]/error1[i+1])/log((double)2);
		order2[i] = log(error2[i]/error2[i+1])/log((double)2);
		printf("The NO. %d convergence order of the Trapezoidal Rule Result is %.10f\n", i+1, order1[i]);
		printf("The NO. %d convergence order of the Simpson's Rule Result is %.10f\n", i+1, order2[i]);
	}
	
	return 0;
}
