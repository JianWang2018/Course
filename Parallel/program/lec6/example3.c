#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static void sig_int(int signo)
{
  signal(SIGINT, sig_int);
  printf("caught SIGINT\n");
}

int main(void) 
{
  sigset_t newmask, oldmask, pendmask;
  if (signal(SIGINT, sig_int) == SIG_ERR) 
    perror("signal");
  sigemptyset(&newmask);
  sigaddset(&newmask, SIGINT);
  if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
    perror("sigprocmask");
  printf("sleep 5\n");
  sleep(5);
  printf("wakeup\n");
  if (sigpending(&pendmask) < 0)
    perror("sigpending");
  if (sigismember(&pendmask, SIGINT)) 
    printf("SIGINT pending\n");
  if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) 
    perror("sigprocmask");
  printf("SIGINT unblocked\n, sleep 5\n");
  sleep(5);
  printf("wakeup\n");
  exit(0);
}
