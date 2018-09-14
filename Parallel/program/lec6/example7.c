#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int count;
void sig_alrm(int signo)
{ 
  printf("caught SIGALRM, count = %d\n", count);
  count++;
  if (count >= 5) exit(0);
  alarm(1);
}

int main() {
  int i;
  struct sigaction act;
  char ch[100];

  ch[0] = 0;
  act.sa_handler = sig_alrm;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;

  sigaction(SIGALRM, &act, 0);
  count = 0;
  alarm(1);
  for(i=0;;) {
    size_t tt;
    tt=read(STDIN_FILENO, ch, 5);
    ch[5] = '\0';
    printf("i=%d, ch = %s, tt= %d\n", i++, ch, tt);
  } 
}
  
