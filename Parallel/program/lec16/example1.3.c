/* this is a UDP sender */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
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
} myframe, ackframe;

int main(int argc, char *argv[])
{
  int sockfd;
  struct sockaddr_in servaddr, saddr1;
  char buf, buf1;
  int len;
  int sendcount=0;

  if (argc < 3) {
    printf("Usage : a.out IPaddr port\n");
    exit(0);
  }

  memset(&servaddr, 0, sizeof(servaddr));  
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons((int)atoi(argv[2]));
  servaddr.sin_addr.s_addr = inet_addr(argv[1]);

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror(": socket");
    exit(0);
  }

  { 
    /* basic algorithm not reliability here */
    int i;
    int myseq;
    fd_set rset;
    struct timeval timeout;

    buf = 'A'; myseq = 0;
   
    for (i=0; i< 1000; i++) {
      /* put packet in a frame */
      myframe.size = 1;
      myframe.seq = myseq ++;
      myframe.type = DATA_FRAME;
      myframe.buf[0] = buf;
      sendto(sockfd, &myframe, sizeof(myframe)-PACKETSIZE+myframe.size, 
             0, (struct sockaddr *)&servaddr, sizeof(servaddr));
      sendcount++;
      buf++;
      if (buf == 'Z'+1) buf = 'A';

      FD_ZERO(&rset);
      FD_SET(sockfd, &rset);
      timeout.tv_sec = 0;
      timeout.tv_usec = 10;
      do {
	while (select(64, &rset, NULL, NULL, &timeout) == 0) {
        /* retransmission */
	  sendto(sockfd, &myframe, sizeof(myframe)-PACKETSIZE+myframe.size, 
		 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
	  sendcount++;
	  FD_ZERO(&rset);
	  FD_SET(sockfd, &rset);
	  timeout.tv_sec = 0;
	  timeout.tv_usec = 10;
	}

	recvfrom(sockfd, &myframe, sizeof(myframe), 0, NULL, NULL);
        if (myframe.type != ACK_FRAME) {
          printf("Something is wrong.\n");
	  exit(0);
	}
        printf("Receive ack %d\n", myframe.seq);
      } while (myframe.seq != myseq -1);
      /* my break here is the sequence number wrap around */
    }

    sleep(3);
    for (i=0; i<4; i++) {
      buf = '+';
      myframe.size = 1;
      myframe.seq = myseq ++;
      myframe.type = DATA_FRAME;
      myframe.buf[0] = buf;
      sendto(sockfd, &myframe, sizeof(myframe)-PACKETSIZE+myframe.size, 
             0, (struct sockaddr *)&servaddr, sizeof(servaddr));
      sleep(2);
    }
  }
  printf("Total send = %d\n", sendcount);
}






