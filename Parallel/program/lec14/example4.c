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
    printf("child %d terminated.\n", pid);
  return ;
}

#define MAXCONN 3

int main(int argc, char * argv[])
{
  int sockfd, rec_sock, len, i;
  struct sockaddr_in addr, recaddr;
  struct sigaction abc;
  int client[MAXCONN];
  char buf[100];
  fd_set allset, rset;
  int maxfd;

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

  for (i=0; i<MAXCONN; i++) client[i] = -1;

  FD_ZERO(&allset);
  FD_SET(sockfd, &allset);
  maxfd = sockfd;

  while (1) {
    rset = allset;
    select(maxfd+1, &rset, NULL, NULL, NULL);
    if (FD_ISSET(sockfd, &rset)) {
      /* somebody tries to connect */
      if ((rec_sock = accept(sockfd, (struct sockaddr *)(&recaddr), &len)) < 0) {
	if (errno == EINTR)
	  continue;
	else {
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
   
      for (i=0; i<MAXCONN; i++) {
	if (client[i] < 0) {
	  client[i] = rec_sock; 
	  FD_SET(client[i], &allset);
	  break;
	}
      }
      if (i== MAXCONN) {
	printf("too many connections.\n");
        close(rec_sock);
      }
      if (rec_sock > maxfd) maxfd = rec_sock;
    }

    for (i=0; i<MAXCONN; i++) {
      if (client[i] < 0) continue;
      if (FD_ISSET(client[i], &rset)) {
	int num;
        num = read(client[i], buf, 100);
        if (num == 0) {
	  /* client exists */
          close(client[i]);
          FD_CLR(client[i], &allset);
          client[i] = -1;
	} else {
	  write(client[i], buf, num);
	}
      }
    }
  }
}




