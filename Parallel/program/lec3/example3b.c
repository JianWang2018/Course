#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
  int i;
  char *my[2];

  if (argc < 2) {
    printf("Usage: a.out command\n");
    exit(0);
  }
  
  for (i = 1; i< argc; i++) {
    my[0] = argv[i];
    my[1] = NULL;
    
    if(execv(argv[i], &my[0]) == -1) {
      printf("Command execution failed.\n");
    }
  }
}

