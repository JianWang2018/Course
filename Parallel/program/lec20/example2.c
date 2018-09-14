// echo server
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/un.h>

#define MAXBUFSIZE 100

/* Handler for SIGCHLD */

void sig_chld(int signo)
{
  pid_t pid;
  int stat; 
  while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) 
    printf("child %d terminated.\n", pid);
  return ;
}




void process_request(int fd)
{
  char buf[MAXBUFSIZE];
  int num;


  while (num = read(fd, buf, MAXBUFSIZE)) 
  {
    if (num > 0) 
      write(fd, buf, num);
  }

}


int main(int argc, char * argv[])
{
  int sockfd, rec_sock;
  struct sigaction sa;
  struct sockaddr_un addr;

  if (argc < 2) 
  {
    fprintf(stderr,"USAGE: %s pathname.\n", argv[0]);
    exit(-1);
  }

  /* Handle SIGCHLD */
  sa.sa_handler = sig_chld;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGCHLD, &sa, NULL);

  if ((sockfd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0) 
  {
    perror("socket");
    exit(1);
  }

  unlink(argv[1]);  /* trying to remove the file */

  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_LOCAL;
  strncpy(addr.sun_path, argv[1], sizeof(addr.sun_path)-1);
  addr.sun_path[sizeof(addr.sun_path)-1] = '\0';

  if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
  {
    perror("bind");
    exit(1);
  }

  
  if (listen(sockfd, 5) < 0) 
  {
    perror("listen");
    exit(1);
  }

  for(;;)
  {
    if( (rec_sock = accept(sockfd, NULL, NULL)) < 0)
      if (errno == EINTR)
	continue;
      else 
      {
	perror("accept");
	exit(1);
      }

    if (fork() == 0) /* Child */
    {
      close(sockfd);
      process_request(rec_sock);
      close(rec_sock);
      exit(0);
    } 
    else /* Parent */
    {
      close(rec_sock); 
    }
  }

  return 0;
}


