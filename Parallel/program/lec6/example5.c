#define _POSIX_C_SOURCE 10000000L
#include <sys/types.h>
#include <signal.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void sig_usr1(int signo)
{
   printf("caught SIGUSR1\n");
}

int main(void) 
{
  sigset_t newmask, oldmask, pendmask;
  struct sigaction abc;
  int i;
  pid_t pid;

  abc.sa_handler = sig_usr1;
  sigemptyset(&abc.sa_mask);
  abc.sa_flags = 0;


  if ((pid=fork()) == 0) {
    sigaction(SIGUSR1, &abc, NULL);
    for(;;) pause();
  } else {
    abc.sa_handler = sig_usr1;
    sigaction(SIGUSR1, &abc, 0);
    sleep(1);
    for(i=0; i<5; i++) {
      kill(pid, SIGUSR1);
      sleep(1);
    }
    exit(0);
  }

}

