#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  mode_t a;

  a = umask(0);
  printf("mask = %o (oct), %x (hex), %d (dec)\n", a, a, a);

  if (creat("foo", S_IRUSR | S_IRGRP | S_IROTH) < 0) {
    printf("failed to create.\n");
    exit(0);
  }
  return 0;
}
