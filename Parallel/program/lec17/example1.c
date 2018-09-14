#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static void sigalarm(int signo) {
  printf("Catching SIGALRM\n");
  return; 
}

int main(int argc, char *argv[])
{
  int sockfd;
  struct sockaddr_in servaddr, raddr;
  char buf[50000];
  int on = 1;

  if (argc < 2) {
    printf("Usage : a.out port\n");
    exit(0);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons((int)atoi(argv[1]));
  servaddr.sin_addr.s_addr = inet_addr("255.255.255.255");

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror(": socket");
    exit(0);
  }

  if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0) {
    perror(": setsockopt");
    exit(0);
  }
 

  strcpy(buf, "hello world!!\n");
  while (1) {
    ssize_t n;
    n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (n < 0) printf("sending broadcast message failed.\n");
    else printf("Sending %d bytes\n", n);
    sleep(1);
    /*
    signal(SIGALRM, sigalarm);
    alarm(2);
    for ( ; ; ) {
#ifdef LINUX 
      socklen_t len;
#else 
      int len;
#endif
      len = sizeof(servaddr);
      n = recvfrom(sockfd, buf, 50000, 0, (struct sockaddr *)&raddr, &len);
      if (n < 0) {
	if (errno == EINTR) break;
	else {
	  perror(": recvfrom");
	}
      } else {
	buf[n] = '\0';
	printf("from %s: %s", inet_ntoa(raddr.sin_addr), buf);
      }
    }
    */
  }
}







