/* this is a UDP sender */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FRAME 1
#define ACK_FRAME 2
#define PACKETSIZE 10000

struct frame {
  int seq;
  int type;
  int size;
  char buf[PACKETSIZE];
} myframe;

int main(int argc, char *argv[])
{
  int sockfd, len;
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;
  int count = 0;
  FILE *fd;
  int last_seq;

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

  if ((fd = fopen("outputfile", "w")) == NULL) {
    printf("can't open outputfile\n");
    exit(0);
  }

  last_seq = -1;
  while (1) {
    memset(&cliaddr, 0, sizeof(servaddr));
    len = sizeof (cliaddr);
    recvfrom(sockfd, &myframe, sizeof(myframe), 0, (struct sockaddr *)&cliaddr, &len);
    if (last_seq < myframe.seq) {
      /* good frame */
      if ((myframe.buf[0] < 'A') || (myframe.buf[0] > 'Z')) exit(0);
      if (myframe.buf[0]=='Z') fprintf(fd, "%c\n", myframe.buf[0]);
      else fprintf(fd, "%c", myframe.buf[0]);
      //      printf("message %c, seg = %d\n", myframe.buf[0], myframe.seq);
      last_seq = myframe.seq;

      myframe.size = 0;
      myframe.type = ACK_FRAME;
      /* myframe.seq is acked */
      sendto(sockfd, &myframe, sizeof(myframe)-PACKETSIZE+myframe.size, 
         0, (struct sockaddr *)&cliaddr, len);
    } else {
      printf("duplicate packet %d, dropped\n", myframe.seq); 
      /* must resend, otherwise the sender may keep resending a same packet */
      myframe.size = 0;
      myframe.type = ACK_FRAME;
      /* myframe.seq is acked */
      sendto(sockfd, &myframe, sizeof(myframe)-PACKETSIZE+myframe.size, 
         0, (struct sockaddr *)&cliaddr, len);
    }
  }
}






