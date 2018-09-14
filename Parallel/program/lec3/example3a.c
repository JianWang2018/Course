#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{

  if (argc < 2) {
    printf("Usage: a.out command\n");
    exit(0);
  }
  
  if(execv(argv[1], &argv[1]) == -1) {
    printf("Command execution failed.\n");
  }
}

