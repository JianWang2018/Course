#include <stdio.h>
#include <omp.h>

int 
main(int argc, char *argv[])
{

	int i, n, x;
	x = 0;
	n = 1000;

	if (argc > 1) n = atoi(argv[1]);	
	#pragma omp parallel shared(x, n) private (i) 
	{
	    for (i = 0; i < n; ++i) x = x + 1;

      	}  /* end of parallel section */
	fprintf(stderr, "%d\n", x);
}

