/* this is a UDP sender */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
  int sockfd;
  struct sockaddr_in servaddr, saddr1;
  char buf, buf1;
  int len;

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
    buf = 'A';
    for (i=0; i< 1000; i++) {
      sendto(sockfd, &buf, 1, 0, (struct sockaddr *)&servaddr, 
             sizeof(servaddr));
      buf++;
      if (buf == 'Z'+1) buf = 'A';
      recvfrom(sockfd, &buf1, 1, 0, NULL, NULL);
      printf("got ack i= %d\n", i);
     }



    sleep(3);
    for (i=0; i<4; i++) {
      buf = '+';
      sendto(sockfd, &buf, 1, 0, (struct sockaddr *)&servaddr, 
             sizeof(servaddr));
      sleep(2);
    }
  }
  
 

}






