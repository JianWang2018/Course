#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
  int sockfd;
  socklen_t len;
  struct sockaddr_un addr1, addr2;

  if (argc != 2) 
  {
    fprintf(stderr, "Usage: %s filename\n", argv[0]);
    exit(0);
  }

  if ((sockfd = socket(AF_LOCAL,  SOCK_STREAM, 0)) == 0) 
  {
    perror("socket");
    exit(-1);
  }

  unlink(argv[1]);  /* trying to remove the file */

  memset(&addr1, 0, sizeof(addr1));
  addr1.sun_family = AF_LOCAL;
  strncpy(addr1.sun_path, argv[1], sizeof(addr1.sun_path)-1);
  addr1.sun_path[sizeof(addr1.sun_path)-1] = '\0';
  
  if (bind(sockfd, (struct sockaddr *) &addr1, sizeof(addr1)) < 0) 
  {
    perror("bind");
    exit(-1);
  }

  len = sizeof(addr2);

  if (getsockname(sockfd, (struct sockaddr *) &addr2, &len) < 0) 
  {
    perror("getsockname");
    exit(-1);
  }

  printf("Bound name = %s, len = %d\n", addr2.sun_path, len);
  return 0;
}
