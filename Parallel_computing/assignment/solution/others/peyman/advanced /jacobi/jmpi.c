#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include "mpi.h"
#define N 4096
float x[N+2][N+2][2];
float help[N][N];
float back1[N],front1[N],back2[N],front2[N],middle_row[N],middle_colom[N],middle_row1[200][N];
#define myabs(a) (((a) > 0) ? (a):(-(a)))
int NN;
float a1, a2, a3, a4, a5, a6;
float MAXDIFF;
int t, t1, t2;
float  maxdiff1,localmaxdiff1[200],mymaxdiff1,localmax;
int iteration,reminder=0;

float myjacobi(float x[N+2][N+2][2],int myrank,int reminder,float help[N][N],float back1[N], float back2[N],float front1[N],float front2[N],float middle_row[N],float middle_colom[N],int *flag1) 
{
  int i, j, k=1,index,f=1,z=1;
  int Nprocess,myid;	
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  MPI_Comm_size(MPI_COMM_WORLD, &Nprocess);

  for(i=1;i<=NN;i++)
    {	
      if(myrank!=0)
	{
	  x[i][NN/Nprocess*myrank][t1]=back1[i];//printf("back....i %f%d\n",back[i],NN/Nprocess*myrank );
	}
      if(myrank!=Nprocess-1)
	{
	  x[i][(NN/Nprocess)*(myrank+1)+reminder+1][t1]=front1[i]; //printf("front....i %f\n",front[i],i );
	}
    } 
   
  maxdiff1 = -1.0;
  localmaxdiff1[myrank]=-1;
  for (i=1; i <= NN; i++) {
    for(j=NN/Nprocess*myrank+1; j<= (NN/Nprocess)*(myrank+1)+reminder; j++)     
      {	
	//printf("x[i-1][j][t1],x[i+1][j][t1],x[i][j-1][t1], x[i][j+1][t1]...i,j  %f %f %f %f %i %i \n",x[i-1][j][t1],x[i+1][j][t1],x[i][j-1][t1], x[i][j+1][t1],i,j);
	help[i][k++]=x[i][j][t] = a1*x[i-1][j][t1] + a3 * x[i+1][j][t1] +
	  a2*x[i][j-1][t1] + a4 * x[i][j+1][t1];
	//printf("help bala....i,k %f %d %d\n",help[i][k-1],i,k-1);
	//printf("x[i][j][t] - x[i][j][t1],,,,,i=j %f %d %d\n",x[i][j][t] - x[i][j][t1],i, j);
	if (myabs(x[i][j][t] - x[i][j][t1]) >localmaxdiff1[myrank])
	  {
	    localmaxdiff1[myrank]= myabs(x[i][j][t] - x[i][j][t1]);
	  }
	if(j==NN/2)
	  {	
	    middle_colom[f++]=x[i][j][t];
	    *flag1=*flag1+1;
	    //printf("middle colom %f %d %d\n",middle_colom[f-1],f-1,myrank);
	  }

	if(i==NN/2)
	  middle_row[z++]=x[NN/2][j][t];
      }//end for j
    index=k;
    k=1;
  }
  
  for(i=1;i<=NN;i++)
    {
      back1[i]=back2[i]=help[i][1];
      front1[i]=front2[i]=help[i][index-1];
    }	
  /*k=1;
    for(j=NN/Nprocess*myrank+1; j<= (NN/Nprocess)*(myrank+1)+reminder; j++)
    {
    middle_row[k++]=x[NN/2][j][t];//help[NN/2][j];
    //printf("middle row %f %d\n",middle_row[k-1],k-1);
    }*/
  //printf("flag  myrank %d %d\n",flag1,myrank);
  
  return localmaxdiff1[myrank];
}





int main(int argc, char * argv[])
{
  int i, j, k, z, reminder1=0,f,flag=0;
  int Nprocess, myid, rank;
  FILE *fp;
  char buff[200];
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  MPI_Comm_size(MPI_COMM_WORLD, &Nprocess);



  if((fp = fopen("input.jacobi", "r")) == NULL)
    {
      printf( "Unable to open proj3.input.\n");
      MPI_Finalize(); 
      exit(0);
    }
  fgets (buff, sizeof buff, fp); 
  sscanf(buff,"%d %f %f %f %f %f %f %f", &NN, &a1, &a2, &a3, &a4, &a5, &a6, &MAXDIFF);
  if(myid==0)
   printf("%d %f %f %f %f %f %f %f\n", NN, a1, a2, a3, a4, a5, a6, MAXDIFF);  


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
  if(myid==0)
  printf("maxdiff=%13.12f\n", MAXDIFF);
  
  t = 0; t1 = 1;
  maxdiff1 = 100000.0;
  iteration = 0;
  reminder=NN%Nprocess;
	
  f=0;

  while(maxdiff1>MAXDIFF)
    {	 
      maxdiff1 = -1.0;
      
      if(myid==Nprocess-1)
	{   mymaxdiff1=myjacobi(x,myid,reminder,help,back1,back2,front1,front2,middle_row,middle_colom,&flag);
	  //printf("local******i %f %d\n",mymaxdiff1,myid);
	  //printf("mymaxdiff1 %f\n",mymaxdiff1);
	}
      else
	{	
	  mymaxdiff1=myjacobi(x,myid,0,help,back1,back2,front1,front2,middle_row,middle_colom,&flag);
	  //printf("local******i %f %d\n",mymaxdiff1,myid);
	  //printf("flag,,,Myrank %d %d\n",flag,i);
	}  
      // MPI_Barrier(MPI_COMM_WORLD);
      // MPI_Reduce(&mymaxdiff1, &maxdiff1, 1, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);
      //MPI_Bcast(&maxdiff1, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);			
      
      //printf("here myid=%d\n",myid);
		
      if(myid==0 && Nprocess!=1){
	MPI_Recv(&front1, NN, MPI_FLOAT, myid+1, 1, MPI_COMM_WORLD, &status);
	MPI_Send(&front2, NN, MPI_FLOAT, myid+1, 1, MPI_COMM_WORLD);
      }
      else if(myid==Nprocess-1 && Nprocess!=1){
	MPI_Send(&back2, NN, MPI_FLOAT, myid-1, 1, MPI_COMM_WORLD);
	MPI_Recv(&back1, NN, MPI_FLOAT, myid-1, 1, MPI_COMM_WORLD, &status);
      }
      else if(myid%2==0 &&  Nprocess!=1){
	
	MPI_Recv(&front1, NN, MPI_FLOAT, myid+1, 1, MPI_COMM_WORLD, &status);
	MPI_Send(&front2, NN, MPI_FLOAT, myid+1, 1, MPI_COMM_WORLD);
	MPI_Recv(&back1, NN, MPI_FLOAT, myid-1, 1, MPI_COMM_WORLD, &status);
	MPI_Send(&back2, NN, MPI_FLOAT, myid-1, 1, MPI_COMM_WORLD);
      }
      else if(myid%2==1){
	MPI_Send(&back2, NN, MPI_FLOAT, myid-1, 1, MPI_COMM_WORLD);
	MPI_Recv(&back1, NN, MPI_FLOAT, myid-1, 1, MPI_COMM_WORLD, &status);
	MPI_Send(&front2, NN, MPI_FLOAT, myid+1, 1, MPI_COMM_WORLD);
	MPI_Recv(&front1, NN, MPI_FLOAT, myid+1, 1, MPI_COMM_WORLD, &status);
      }
      
      //MPI_Barrier(MPI_COMM_WORLD);
      MPI_Reduce(&mymaxdiff1, &maxdiff1, 1, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);
      //printf("maxdiff of worker %d is %f\n", myid, mymaxdiff1);  
      MPI_Bcast(&maxdiff1, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
      t2 = t; t = t1; t1 = t2;
      if(myid==0)
	printf("iteration = %d, maxdiff1 = %f, MAXDIFF = %f \n",iteration++, maxdiff1, MAXDIFF);
      
    }//end while
  //printf("hereee myid=%d\n",myid);


  /***Process zero(0) receive middle column and middle row from others process**/

  if(flag!=0 && myid!=0)
    {
      MPI_Send(&middle_colom, NN, MPI_FLOAT, 0, 1, MPI_COMM_WORLD);
      //printf("here1 myid=%d\n",myid);
    }//if flag!=0
  
  else if(myid==0 && flag==0)
    {
      MPI_Recv(&middle_colom, NN, MPI_FLOAT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);	
      //printf("here2 myid=%d\n",myid);
      for (j = 1; j <= NN; j++) 
	x[j][NN/2][t1]=middle_colom[j];
    }
  /******Using barrier to be sure process first receive middle column*****/
  MPI_Barrier(MPI_COMM_WORLD);
  
  if(myid!=0)
    {      
      MPI_Send(&middle_row, NN, MPI_FLOAT, 0, 1, MPI_COMM_WORLD);
    }
  else
    {
      for (k = 1; k <Nprocess; k++){
	MPI_Recv(&middle_row1[k], NN, MPI_FLOAT, k, 1, MPI_COMM_WORLD, &status);
	//printf("here2 myid=%d\n",myid);
      }	
      
      /****process0 has received middle row and coloumn and now have to write them into file****/
      j=1;
      if(Nprocess!=1){
	for (k = 0; k <Nprocess; k++){
	  if(k==Nprocess-1) reminder1=reminder;
	  for(z=1; z<= (NN/Nprocess)+reminder1; z++)
	    x[NN/2][j++][t1]=middle_row1[k][z];	
	}//end for
      }//end if Nprocess!=1
      if ((i=open("mpi.output", O_WRONLY | O_CREAT | O_TRUNC, 0600)) < 0) {
	fprintf(stderr, "Cannot open file proj3_process.output.\n");
	printf("MAXDIFF = %f, maxdiff = %f\n", MAXDIFF, maxdiff1);
      }
      
      for (j = 0; j <= NN + 1; j++) {
	//printf("middle row %f %d\n",x[NN/2][j][t1],j);
	if ((t= write(i, &(x[NN/2][j][t1]),  sizeof(float))) != sizeof(float)) {
	  fprintf(stderr, "write error %d %d.\n", j, t);
	  MPI_Finalize(); 
	  exit(0);
	}
      }
      
      for (j = 0; j <= NN + 1; j++) {
	//printf("middle colom%f %d\n",x[j][NN/2][t1],j);
	if ((t = write(i, &(x[j][NN/2][t1]),  sizeof(float))) != sizeof(float)) {
	  fprintf(stderr, "write error. %d %d\n", j, t);
	  MPI_Finalize(); 
	  exit(0);
	} 
      }
      close(i);
      
    }//end else process0
  
  //printf("here myid=%d\n",myid);
  

  MPI_Finalize(); 
  exit(0);
}
