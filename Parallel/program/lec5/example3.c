#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  if (creat("foo", S_IRUSR | S_IRGRP | S_IROTH) < 0) {
    printf("failed to create.\n");
    exit(0);
  }
  return 0;
}
