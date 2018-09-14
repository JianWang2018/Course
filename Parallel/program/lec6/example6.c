#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int count;
void sig_alrm(int signo)
{ 
  printf("caught SIGALRM, count = %d\n", count);
  count++;
  if (count >= 5) exit(0);
  alarm(1);
}

int main() {
  struct sigaction act;

  act.sa_handler = sig_alrm;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;

  sigaction(SIGALRM, &act, 0);
  count = 0;
  alarm(1);
  for(;;);
}
  
