#include "stdio.h"
#include "stdlib.h"
#include <omp.h> 
#define n 1000000000
unsigned char s[n];
int main()
{
	int i;
	for(i=0; i < n; i++) 
		s[i] = n*n % 256;
	
	int num[256] = {0};
	for (i = 0; i < n; i++)
	{
		num[s[i]] ++;
	}
	for (i = 0; i<256; i++)
		printf("the char %d freq is %f\n", i, num[i]/(double)n);
}