#include <signal.h>
#include <unistd.h>
#include <stdio.h>

void sig_usr(int signo) 
{
  if (signo == SIGUSR1) 
    printf("received SIGUSR1\n");
  else if (signo == SIGUSR2)
    printf("received SIGUSR2\n");
  else if (signo == SIGINT)
    printf("received SIGINT\n");
  else printf("received signo=%d\n", signo);
}

int main(void)
{
  if (signal(SIGUSR1, sig_usr) == SIG_ERR) 
    perror("signal");
  if (signal(SIGUSR2, sig_usr) == SIG_ERR) 
    perror("signal");
  if (signal(SIGINT, sig_usr) == SIG_ERR) 
    perror("signal");
  for(;;)
    pause();
}
