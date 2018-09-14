/*
 * sender.c -- multicasts "hello, world!" to a multicast group once a second
 *
 * Antony Courtney,     25/11/94
 */

#include <stdio.h>
#include <stdlib.h>

#include <strings.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/utsname.h>



#define HELLO_PORT 13
#define HELLO_GROUP "224.0.0.1"

char buf[50000];

main(int argc, char *argv[])
{
     struct sockaddr_in addr, raddr;
     int fd, cnt,nbytes;
     struct ip_mreq mreq;
     char *message="Hello, World!";

     /* create what looks like an ordinary UDP socket */
     if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
          perror("socket");
          exit(1);
     }

     /* set up destination address */
     memset(&addr,0,sizeof(addr));
     addr.sin_family=AF_INET;
     addr.sin_addr.s_addr=inet_addr(HELLO_GROUP);
     addr.sin_port=htons(HELLO_PORT);

     /* now just sendto() our destination! */
     while (1) {
       if ((nbytes=sendto(fd, message, strlen(message)+1, 0,
			 (struct sockaddr *) &addr, sizeof(addr))) < 0) {
	 perror("sendto");
	 exit(1);
       }
       printf("Sent %d bytes\n", nbytes);
       for ( ; ; ) {
#ifdef LINUX 
      socklen_t len;
#else 
      int len;
#endif
      int n;

      len = sizeof(raddr);
      n = recvfrom(fd, buf, 50000, 0, (struct sockaddr *)&raddr, &len);
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
   sleep(1);
   }
}

