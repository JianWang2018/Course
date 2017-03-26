#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#define N 4096

float x[N+2][N+2][2];

#define myabs(a) (((a) > 0) ? (a):(-(a)))

int main(void) 
{
  int NN;
  float a1, a2, a3, a4, a5, a6;
  float MAXDIFF;
  int i, j;
  int t, t1, t2;
  float  maxdiff1;
  int iteration;
  FILE *fp;

  if( (fp = fopen("input.jacobi", "r+")) == NULL)
    {
      printf("No such file\n");
      exit(1);
    }  
  
  if (fp == NULL)
    {
      printf("Error Reading File\n");
    }
  fscanf(fp,"%d %f %f %f %f %f %f %f", &NN, &a1, &a2, &a3, &a4, &a5, &a6, &MAXDIFF);
  

  printf("%d %f %f %f %f %f %f %f\n", NN, a1, a2, a3, a4, a5, a6, MAXDIFF);   
  /*  a1 = a2 = a3 = a4 = 0.25; a6 = 0; a5 = 0.1;

  MAXDIFF = 0.0001;
  */

  printf("maxdiff = %13.12f\n", MAXDIFF);

  for (i=1; i<=NN+1; i++) {
    x[i][0][0] = a5*i;
    x[i][0][1] = a5*i;

    x[i][NN+1][0] = a6*i;
    x[i][NN+1][1] = a6*i;

    x[0][i][0] = 0.0;
    x[0][i][1] = 0.0;

    x[NN+1][i][0] = 0.0;
    x[NN+1][i][1] = 0.0; 
  }


  for (i=1; i<=NN; i++) 
    for (j=1; j<=NN; j++) {
      x[i][j][0] = 0.0;
      x[i][j][1] = 0.0;
    }

  printf("maxdiff=%13.12f\n", MAXDIFF);

  t = 0; t1 = 1;
  maxdiff1 = 100000.0;
  iteration = 0;

  while (maxdiff1 > MAXDIFF) {
    maxdiff1 = -1.0;
    for (i=1; i <= NN; i++) 
      for (j=1; j <=NN; j++) {
        x[i][j][t] = a1*x[i-1][j][t1] + a3 * x[i+1][j][t1] +
	  a2*x[i][j-1][t1] + a4 * x[i][j+1][t1];
	if (myabs(x[i][j][t] - x[i][j][t1]) > maxdiff1)
	  maxdiff1 = myabs(x[i][j][t] - x[i][j][t1]);
      }
    
    /*for(i=0;i<NN+2;i++){
      for(j=0;j<NN+2;j++){
	printf("%f\t",x[i][j][t]);
      }
      printf("\n");
      }*/
    t2 = t; t = t1; t1 = t2;
    printf("iteration = %d, maxdiff1 = %f, MAXDIFF = %f\n", 
           iteration++, maxdiff1, MAXDIFF);
  }
  printf("MAXDIFF = %f, maxdiff = %f\n", MAXDIFF, maxdiff1);
  
  if ((i=open("jlin.output", O_WRONLY | O_CREAT | O_TRUNC, 0600)) < 0) {
    fprintf(stderr, "Cannot open file proj3.output.\n");
    exit(0);
  }

  for (j = 0; j <= NN + 1; j++) {
    if ((t= write(i, &(x[NN/2][j][t1]),  sizeof(float))) != sizeof(float)) {
      fprintf(stderr, "write error %d %d.\n", j, t);
      exit(0);
    }
  }

  for (j = 0; j <= NN + 1; j++) {
    if ((t = write(i, &(x[j][NN/2][t1]),  sizeof(float))) != sizeof(float)) {
      fprintf(stderr, "write error. %d %d\n", j, t);
      exit(0);
    }
  }
  close(i);
  return 0;
}
