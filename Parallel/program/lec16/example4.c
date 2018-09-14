/* this is a UDP sender */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int sockfd, len;
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;
  char buf[100000];
  int count = 0;

  if (argc != 2) {
    printf("Usage : a.out port\n");
    exit(0);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons((int)atoi(argv[1]));
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror(": socket");
    exit(0);
  }

  if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)) {
    perror(": bind");
    exit(0);
  }

    recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&cliaddr, &len);
    //    sleep(3);
  while (1) {
    recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&cliaddr, &len);
    printf("count = %d, message %s from %s %d\n", count++, buf, inet_ntoa(cliaddr.sin_addr),
           cliaddr.sin_port);
  }
}






