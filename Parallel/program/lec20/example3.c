// unix domain socket sender
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>

#define BUFLEN 100

/* this routine read n bytes. It returns *0* if unable to read msgsize chars. */

static int readn (int sock, char *msgp, unsigned msgsize)
{
  int rlen, temp;

  rlen = 0;
  while(rlen < msgsize)
  {
    temp = read(sock, msgp+rlen, msgsize-rlen);
    if(temp<0)
      if(errno == EINTR)
	continue;
      else
	return 0;
    else if(temp == 0)
      break; /* End of file */

    rlen += temp;
  }
  
  
  return rlen==msgsize;
}


int main(int argc, char * argv[])
{
  int sockfd;
  struct sockaddr_un addr;
  char buf1[BUFLEN], buf2[BUFLEN];

  if (argc != 2) 
  {
    fprintf(stderr,"USAGE: %s pathname.\n", argv[0]);
    exit(-1);
  }

  if ((sockfd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0) 
  {
    perror("socket");
    exit(1);
  }

  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_LOCAL;
  strncpy(addr.sun_path, argv[1], sizeof(addr.sun_path)-1);
  addr.sun_path[sizeof(addr.sun_path)-1] = '\0';

  if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
  {
    perror("connect");
    exit(1);
  }

  while (fgets(buf1, BUFLEN, stdin)) /* Until EOF */
  {
    write(sockfd, buf1, strlen(buf1)+1);
    if (readn(sockfd, buf2, strlen(buf1)+1) == 0) 
    {
      fprintf(stderr, "Server closed connection.\n");
      exit(0);
    }
    printf("Server response : %s\n", buf2);
  }

  return 0;
}




