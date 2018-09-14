#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sig_chld(int signo)
{
  pid_t pid;
  int stat;
  while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) 
    printf("child %d terminated.\n", (int)pid);
  return ;
}

int main(int argc, char * argv[])
{
  int sockfd, rec_sock, i;
  int num;
  unsigned int len;
  struct sockaddr_in addr, recaddr;
  struct sigaction abc;
  char buf[100];

  abc.sa_handler = sig_chld;
  sigemptyset(&abc.sa_mask);
  abc.sa_flags = 0;

  sigaction(SIGCHLD, &abc, NULL);

  if (argc < 2) {
    printf("Usage: a.out port.\n");
    exit(0);
  }

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror(": Can't get socket");
    exit(1);
  }

  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_family = AF_INET;
  addr.sin_port = htons((short)atoi(argv[1]));

  if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror(": bind");
    exit(1);
  }

  
  len = sizeof(addr);
  if (getsockname(sockfd, (struct sockaddr *)&addr, &len) < 0)
    {
      perror(": can't get name");
      _exit(1);
   }
  
  printf("ip = %s, port = %d\n", inet_ntoa(addr.sin_addr), addr.sin_port);

  if (listen(sockfd, 5) < 0) {
    perror(": bind");
    exit(1);
  }

  for (i=0; i<2; i++) { 
    if (fork() == 0) {
      while (1) {
	while ((rec_sock = accept(sockfd, (struct sockaddr *)(&recaddr), &len))
	       < 0) {
	  if (errno == EINTR) {
	    printf("errno = %d\n", errno);
	    continue; 
	  } else {
	    perror(":accept error");
	    exit(1);
	  }
	}
	
	if (rec_sock < 0) {
	  perror(": accept");
	  exit(1);
	}
	
      /* print the remote socket information */
	
	printf("remote machine = %s, port = %x, %x.\n", inet_ntoa(recaddr.sin_addr), recaddr.sin_port, ntohs(recaddr.sin_port));
      
	while ((num = read(rec_sock, buf, 100))) {
	  if (num == -1)
	    continue;
	  write(rec_sock, buf, num);
	}
	close(rec_sock);  
      }
    }
  }
  while(1) { int i; wait(&i);}
}




