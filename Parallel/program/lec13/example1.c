#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void printconnerror()
{
  switch (errno) {
  case ETIMEDOUT : printf("Connection timed out.\n"); break;
  case ECONNREFUSED : printf("Connection refused.\n"); break;
  case EHOSTDOWN : printf("Host down.\n"); break;
  case EHOSTUNREACH : printf("No route to the host.\n"); break;
  case ENETUNREACH : printf("Network unreachable.\n"); break;
  default: printf("errno = %d\n", errno);
  }
}

int main(int argc, char * argv[])
{
  int sockfd, i;
  struct sockaddr_in addr;
  char buf[100];

  if (argc < 3) {
    printf("Usage: a.out ip_addr port.\n");
    exit(0);
  }

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror(": Can't get socket");
    exit(1);
  }

  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(argv[2]));
  addr.sin_addr.s_addr = inet_addr(argv[1]);

  if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    printconnerror();
    perror(": connect");
    exit(1);
  }
  printf("out of connect\n"); fflush(0);

  for (i=0; i<100; i++) buf[i] = '\0';
  read(sockfd, buf, 99);
  printf("string read = %s\n", buf);
  return 0;
}

