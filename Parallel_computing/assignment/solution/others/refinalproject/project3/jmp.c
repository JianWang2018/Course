#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

void wait();

#define N 4096

float x[N+2][N+2];
float y[N+2][N+2];

#define myabs(a) (((a) > 0) ? (a):(-(a)))


void getRow(float *row, int r, int NN, int t){
  int i;
  if(t == 1)
    for(i=0;i<NN+2;i++)
      row[i] = x[r][i];
  else if(t == 0)
    for(i=0;i<NN+2;i++)
      row[i] = y[r][i];
}


int main(int argc, char *argv[]) 
{
  int NN, NProc;
  float a1, a2, a3, a4, a5, a6;
  float MAXDIFF;
  char *buf[4];
  int i, j,pid[100], low, high;
  int t, t1, m, k, t2, t3;
  int fd[100][3][2];
  float  *left, *right;
  float  maxdiff1[100], maxdiffc;
  int iteration;
  FILE *fp;

  if(argc > 1)
    NProc = atoi(argv[1]);
  else NProc = 5;
  left = malloc(N * sizeof(float));
  right = malloc(N * sizeof(float));
    
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
    x[i][0] = a5*i;
    y[i][0] = a5*i;

    x[i][NN+1] = a6*i;
    y[i][NN+1] = a6*i;

    x[0][i] = 0.0;
    y[0][i] = 0.0;

    x[NN+1][i] = 0.0;
    y[NN+1][i] = 0.0; 
  }


  for (i=1; i<=NN; i++) 
    for (j=1; j<=NN; j++) {
      x[i][j] = 0.0;
      y[i][j] = 0.0;
    }

  printf("maxdiff=%13.12f\n", MAXDIFF);
  
  if ((t1=open("jmp.output", O_WRONLY | O_CREAT | O_TRUNC, 0600)) < 0) {
    fprintf(stderr, "Cannot open file jmp.output.\n");
    exit(0);
  }

  if ((t3=open("jmp.output2", O_WRONLY | O_CREAT | O_TRUNC, 0600)) < 0) {
    fprintf(stderr, "Cannot open file jmp.output2.\n");
    exit(0);
  }

  t = 0;
  for(i=0;i<NProc; i++)
    maxdiff1[i] = 100000.0;
  maxdiffc = 1000000.0;
  iteration = 0;
  for (i=0;i<NProc;i++) {
    pipe(fd[i][0]); pipe(fd[i][1]); pipe(fd[i][2]);
  }
  for (i=0; i<NProc; i++){
    if((pid[i] = fork()) == 0){
      low = (NN*i)/NProc+1; high = ((i+1)*NN)/NProc;
      while (maxdiffc > MAXDIFF) {
	
	maxdiff1[i] = maxdiffc = -1.0;
	for (j = low; j <= high; j++){
	  
	  /*write copy of current to pipe*/
	  if (j == low && j != 1 ){
	    getRow(left,j,NN,t);
	    write(fd[i-1][1][1],left,(NN+2)*sizeof(float));
	  }
	  if (j == high && j != NN ){
	    getRow(right,j,NN,t);
	    write(fd[i][0][1],right,(NN+2)*sizeof(float));
	  }
	  
	  /*get left neighbor*/
	  if(j-1 >= low || j-1 == 0){
	    getRow(left,j-1,NN,t);
	  }
	  else if (j-1 < low ){
	    read(fd[i-1][0][0],left,(NN+2)*sizeof(float));
	  }
	  
	  /*get right neighbor*/
	  if(j+1 <= high || j+1 == NN+1){
	    getRow(right,j+1,NN,t);
	  }
	  else if (j+1 > high ){
	    read(fd[i][1][0],right,(NN+2)*sizeof(float));
	  }
	  
	  for (k=1; k < NN+1; k++){
	    
	    /*calculate current cell*/  
	    if(t == 0){
	      x[j][k] = a2 * y[j][k-1] + a4 * y[j][k+1] +  a1*left[k] + a3 * right[k];
	      /*if(j == NN/2){
		if ((t2= write(t1, &(x[j][k]),  sizeof(float))) != sizeof(float)) {
		  fprintf(stderr, "write error %d %d.\n", j, t2);
		  exit(0);
		}
		}*/
	    }
	    else{
	      y[j][k] = a2 * x[j][k-1] + a4 * x[j][k+1] +  a1*left[k] + a3 * right[k];
	      if(j == NN/2){
		/*if ((t2= write(t1, &(y[j][k]),  sizeof(float))) != sizeof(float)) {
		  fprintf(stderr, "write error %d %d.\n", j, t2);
		  exit(0);
		  }*/
	      }
	    }
	    /*calculate maxdiff*/
	    if (myabs(x[j][k] - y[j][k]) > maxdiffc){
	      maxdiffc = myabs(x[j][k] - y[j][k]);
	    }
	    
	  }
	  
	  /*if(t == 0){
	    if ((t2 = write(t3, &(x[j][NN/2]),  sizeof(float))) != sizeof(float)) {
	      fprintf(stderr, "write error. %d %d\n", j, t2);
	      exit(0);
	    }
	  
	  }
	    else{
	      if ((t2 = write(t3, &(y[j][NN/2]),  sizeof(float))) != sizeof(float)) {
		fprintf(stderr, "write error. %d %d\n", j, t2);
		exit(0);
	      }
	      }*/
	}
	
	for(m=0;m<NProc-1;m++)
	  maxdiff1[m]=maxdiffc;
	write(fd[i][2][1], &maxdiff1,(NProc-1)*sizeof(float));
	for(m=0;m<NProc;m++){
	  if(i != m){
	    read(fd[m][2][0],&maxdiff1[m],sizeof(float));
	    if(maxdiff1[m] > maxdiffc) maxdiffc = maxdiff1[m];
	  }
	}
	
	/*for(m=0;m<NProc;m++){
	  write(fd[i][2][1], &maxdiffc,sizeof(float));
	  if(i==0) read(fd[NProc-1][2][0],&maxdiff1[i],sizeof(float));
	  else read(fd[i-1][2][0],&maxdiff1[i],sizeof(float));
	  if(maxdiff1[i] > maxdiffc) maxdiffc = maxdiff1[i];
	  }*/
	/*for(m=0;m<2;m++){
	  if(i==0){
	    write(fd[0][2][1],&maxdiffc,sizeof(float));
	    read(fd[NProc-1][2][0],&maxdiffc,sizeof(float));
	  }
	  else {
	    read(fd[i-1][2][0],&maxdiff1[i],sizeof(float));
	    if(maxdiff1[i] > maxdiffc) maxdiffc = maxdiff1[i];
	    write(fd[i][2][1],&maxdiffc,sizeof(float));
	  }
	  }*/
	/*if(i==0){
	  write(fd[0][2][1],&maxdiffc,sizeof(float));
	  read(fd[NProc-1][2][0],&maxdiffc,sizeof(float));
	  //write(fd[0][2][1],&maxdiffc,sizeof(float));
	}
	else{
	  read(fd[i-1][2][0],&maxdiff1[i],sizeof(float));
	  if(maxdiffc < maxdiff1[i]) maxdiffc = maxdiff1[i];
	  write(fd[i][2][1],&maxdiffc,sizeof(float));
	  if(i == NProc-1) 
	    for(m=0;m<NProc-2;m++)
	      write(fd[m][2][1],&maxdiffc,sizeof(float));
	  else read(fd[i-1][2][0],&maxdiffc,sizeof(float));
	  
	  }*/
	
	
	if(t == 0) t = 1;
	else if(t == 1) t = 0;
	
        if(i==0){
	  printf("%i,iteration = %d, maxdiff1 = %f, MAXDIFF = %f\n",i, 
		 iteration++, maxdiffc, MAXDIFF);
	}
	else iteration++;
      }
      exit(0);
    } 
  }
  for(i=0;i<NProc;i++) wait(&pid[i]);
  printf("MAXDIFF = %f, maxdiff = %f\n", MAXDIFF, maxdiffc);
  while(read(t3,buf,4) != -1)
    {
      write(t1,buf,4);
    }
close(t1);close(t3);
  return 0;
}
