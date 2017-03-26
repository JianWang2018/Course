#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#define N 4096

float *x, *y;
float *dev_x;
float *dev_y;
#define myabs(a) (((a) > 0) ? (a):(-(a)))

__global__ void matrixadd(float* x, float* y, int NN, float a1, float a2, float a3, float a4, float *diff)
{
  int  i = blockIdx.x * blockDim.x + threadIdx.x;  
  int  index = (NN+2)*(i+1);
  
  diff[i] = -1.0;
  for(int k=1; k <= NN; k++){
    x[index + k] = a2 * y[index + k - 1] + a4 * y[index + k + 1] + a1 * y[index + k - NN - 2] + a3 * y[index + k + NN + 2];
    if(myabs(x[index + k] - y[index + k]) > diff[i])
      diff[i] = myabs(x[index+k] - y[index+k]);
  }
  //__syncthreads();
}


int main(void) 
{
  int NN;
  float a1, a2, a3, a4, a5, a6;
  float MAXDIFF;
  int i, j;
  int t, t1, t2;
  float  maxdiff1;
  float *mydiff, *dev_mydiff;
  int iteration;
  FILE *fp;

  if((fp = fopen("input.jacobi","r+")) == NULL){
	printf("File not found.\n");
	exit(1);
}
  
  fscanf(fp,"%d %f %f %f %f %f %f %f", &NN, &a1, &a2, &a3, &a4, &a5, &a6, &MAXDIFF);
  

  printf("%d %f %f %f %f %f %f %f\n", NN, a1, a2, a3, a4, a5, a6, MAXDIFF);   
  /*  a1 = a2 = a3 = a4 = 0.25; a6 = 0; a5 = 0.1;

  MAXDIFF = 0.0001;
  */

  /* 1. allocate host memory */
  x = (float*)malloc( (NN+2)*(NN+2)*sizeof(float) );
  y = (float*)malloc( (NN+2)*(NN+2)*sizeof(float) );


  printf("maxdiff = %13.12f\n", MAXDIFF);

  for (i=1; i<=NN+1; i++) {
    x[i] = a5*i;
    y[i] = a5*i;

    x[i*(NN+2)] = 0.0;
    y[i*(NN+2)] = 0.0;

    x[i+(NN+1)*(NN+2)] = a6*i;
    y[i+(NN+1)*(NN+2)] = a6*i;

    x[NN+1+i*(NN+2)] = 0.0;
    y[NN+1+i*(NN+2)] = 0.0;
  }
  
  for (i=1; i<=NN; i++) 
    for (j=1; j<=NN; j++) {
      x[i+j*NN] = 0.0;
      y[i+j*NN] = 0.0;
    }

  printf("maxdiff=%13.12f\n", MAXDIFF);
  
  t = 0; t1 = 1;
  maxdiff1 = 100000.0;
  iteration = 0;
  mydiff = (float*) malloc( NN*sizeof(float) );
  
  int blockSize = 16;
 
  cudaMalloc( &dev_x, (NN+2)*(NN+2)*sizeof(float) );
  cudaMalloc( &dev_y, (NN+2)*(NN+2)*sizeof(float) ); 
  cudaMalloc( &dev_mydiff, NN*sizeof(float) );

  cudaMemcpy( dev_x, x, (NN+2)*(NN+2)*sizeof(float), cudaMemcpyHostToDevice );
  cudaMemcpy( dev_y, y, (NN+2)*(NN+2)*sizeof(float), cudaMemcpyHostToDevice );

  while (maxdiff1 > MAXDIFF) {
    maxdiff1 = -1.0;
   
      
  cudaMemcpy( dev_mydiff, mydiff, NN*sizeof(float), cudaMemcpyHostToDevice );
   	       
   if (t == 0) matrixadd<<<(NN/blockSize), blockSize>>>(dev_x, dev_y, NN, a1, a2, a3, a4, dev_mydiff);
   else if(t == 1) matrixadd<<<(NN/blockSize), blockSize>>>(dev_y, dev_x, NN, a1, a2, a3, a4, dev_mydiff);

    cudaMemcpy( mydiff, dev_mydiff, NN*sizeof(float), cudaMemcpyDeviceToHost );
   
    for(i = 0; i < NN; i++){
      //for(j = 0; j < NN; j++){
	if(maxdiff1 < mydiff[i]) 
	  maxdiff1 = mydiff[i];
	mydiff[i] = -1;
	//}
    }

    t2 = t; t = t1; t1 = t2;
    printf("iteration = %d, maxdiff1 = %f, MAXDIFF = %f\n", 
           iteration++, maxdiff1, MAXDIFF);
  }
  printf("MAXDIFF = %f, maxdiff = %f\n", MAXDIFF, maxdiff1);
      cudaMemcpy( x, dev_x, (NN+2)*(NN+2)*sizeof(float), cudaMemcpyDeviceToHost );
  cudaMemcpy( y, dev_y, (NN+2)*(NN+2)*sizeof(float), cudaMemcpyDeviceToHost );  

  if ((fp = fopen("cuda.output", "w+")) < 0) {
    fprintf(stderr, "Cannot open file proj3.output.\n");
    exit(0);
  }
  if(t == 1){
  for (j = 0; j <= NN + 1; j++) {
    if ((t = fprintf(fp, "%.10f\t", x[NN/2+j*(NN+2)])) < 0) {
      fprintf(stderr, "write error %d %d.\n", j, t);
      exit(0);
    }
  }

  for (j = 0; j <= NN + 1; j++) {
    if ((t = fprintf(fp, "%.10f\t", x[j+(NN/2)*(NN+2)])) < 0) {
      fprintf(stderr, "write error. %d %d\n", j, t);
      exit(0);
    }
  }
}
else{
  for (j = 0; j <= NN + 1; j++) {
    if ((t = fprintf(fp, "%.10f\t", y[NN/2+j*(NN+2)])) < 0) {
      fprintf(stderr, "write error %d %d.\n", j, t);
      exit(0);
   }
  }

  for (j = 0; j <= NN + 1; j++) {
    if ((t = fprintf(fp, "%.10f\t", y[j+(NN/2)*(NN+2)])) < 0) {
      fprintf(stderr, "write error. %d %d\n", j, t);
      exit(0);
    }
  }
}
  close(i);  
  free(mydiff);
  free(x);
  free(y); 
  cudaFree(dev_x);
  cudaFree(dev_y);
  cudaFree(dev_mydiff);
  return 0;
}
