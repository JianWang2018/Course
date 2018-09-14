#include <errno.h>
#include <signal.h>
#include <stdio.h>

void print_mask()
{
  sigset_t sigset;
  int errno_save;
  
  errno_save = errno;
  if (sigprocmask(0, NULL, &sigset) < 0)
    perror("Sigprocmask");
  printf("sigmask = ");
  if (sigismember(&sigset, SIGINT)) printf("SIGINT ");
  if (sigismember(&sigset, SIGQUIT)) printf("SIGQUIT ");
  if (sigismember(&sigset, SIGUSR1)) printf("SIGUSR1 ");
  if (sigismember(&sigset, SIGALRM)) printf("SIGALARM ");
  if (sigismember(&sigset, SIGABRT)) printf("SIGABRT ");
  if (sigismember(&sigset, SIGCHLD)) printf("SIGCHLD ");
  if (sigismember(&sigset, SIGHUP)) printf("SIGHUP ");
  if (sigismember(&sigset, SIGTERM)) printf("SIGTERM ");
  printf("\n");
  errno = errno_save;
}

int main() {
  print_mask();
  for (;;) pause();
}



