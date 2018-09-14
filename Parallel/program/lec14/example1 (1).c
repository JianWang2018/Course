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
  addr.sin_port = htons((short)atoi(argv[2]));
  addr.sin_addr.s_addr = inet_addr(argv[1]);

  if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror(": connect");
    exit(1);
  }
  printf("out of connect.\n"); fflush(0);
  { int ii;
  ii = 100;
  printf("ii= %d %x\n", ii, ii);
  write(sockfd, &ii , sizeof(int));
  }

  while (fgets(buf, 100, stdin)) {
    write(sockfd, buf, strlen(buf+1));
    if (read(sockfd, buf, 100)==0) {
      printf("server closed the connection.\n");
      exit(0);
    };
    printf("Server response : %s\n", buf);
  }
}




