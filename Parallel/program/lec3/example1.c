#include <stdio.h>
#include <unistd.h>

int main() 
{

  printf("I am here 1.\n");
  getchar();
  fork();
  printf("I am here 2\n");
  getchar();
  printf("I am here 3\n");
}
  
