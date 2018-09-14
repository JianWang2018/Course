#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char * argv[])
{
  int sockfd, rec_sock, len;
  struct sockaddr_in addr, recaddr;

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
  addr.sin_port = htons(atoi(argv[1]));

  if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror(": bind");
    exit(1);
  }

  
  len = sizeof(addr);
  if (getsockname(sockfd, (struct sockaddr *)&addr, (socklen_t *)&len) < 0)
    {
      perror(": can't get name");
      _exit(1);
   }
  
  printf("ip = %s, port = %d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

  if (listen(sockfd, 5) < 0) {
    perror(": bind");
    exit(1);
  }

  rec_sock = accept(sockfd, (struct sockaddr *)(&recaddr), (socklen_t *)&len);

  if (rec_sock < 0) {
    perror(": accept");
    exit(1);
  }

  /* print the remote socket information */

  printf("remote machine = %s, port = %x, %x.\n", inet_ntoa(recaddr.sin_addr), recaddr.sin_port, ntohs(recaddr.sin_port));
  
  memset(&recaddr, 0, sizeof(recaddr));
  len = sizeof(addr);
  if (getpeername(rec_sock, (struct sockaddr *)&recaddr, (socklen_t *) &len) < 0)
    {
      perror(": can't get name");
      _exit(1);
    }

  printf("remote machine = %s, port = %d, %d.\n", inet_ntoa(recaddr.sin_addr), recaddr.sin_port, ntohs(recaddr.sin_port));
  

  write(rec_sock, "hi, there", 10);
  sleep(20);
  exit(1);
}




