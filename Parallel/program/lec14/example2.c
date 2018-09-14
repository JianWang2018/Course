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
#include <string.h>

void sig_chld(int signo)
{
  pid_t pid;
  int stat;
  while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) 
    printf("child %d terminated.\n", pid);
  return ;
}

/* this routine read n bytes */
int readn (int sock, char *msgp, unsigned msgsize)
{
  int rlen;
  int temp;

  do {
      rlen = read(sock, msgp, msgsize);
  } while (rlen == -1 && errno == EINTR);
      
  if (rlen == 0) {
      /* connection closed */
      return 0;
  }


  while (rlen <msgsize) {
    do {
      temp = read(sock, (msgp+rlen), msgsize-rlen);  
    } while (temp == -1 && errno == EINTR);
    rlen += temp;
   }
      
  if (rlen == 0) {
      /* connection closed */
      return 0;
    }
  
  if (rlen < msgsize) {
    perror(": read");
    _exit(1);
  } 
  
  return !0;
}


int main(int argc, char * argv[])
{
  int sockfd, rec_sock, len;
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

  /*  sleep(100); */

  while (1) {
    /* blocking here, if no signal */
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
  
    memset(&recaddr, 0, sizeof(recaddr));
    len = sizeof(addr);
    if (getpeername(rec_sock, (struct sockaddr *)&recaddr, &len) < 0) {
      perror(": can't get name");
      _exit(1);
    }
    printf("remote machine = %s, port = %d, %d.\n", inet_ntoa(recaddr.sin_addr), recaddr.sin_port, ntohs(recaddr.sin_port));
  
    if (fork() == 0) {
      int num;
      close(sockfd);
      readn(rec_sock, (char *)&num, sizeof(num));
      printf("Receiving %d %x\n", num, num);

      while (num = read(rec_sock, buf, 100)) {
        if (num > 0) 
	  write(rec_sock, buf, num);
      }
      exit(0);
    } else {
      close(rec_sock);
    }
  }
}




