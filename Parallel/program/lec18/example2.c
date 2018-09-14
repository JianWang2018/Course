#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

extern int h_errno;

int main(int argc, char **argv)
{
  char *ptr, **pptr;
  struct hostent *hptr;
  struct in_addr addr;

  if (argc < 2) {
    printf("Usage: a.out ipaddr\n");
    exit(0);
  }
  
  inet_aton(argv[1], &addr);
  if ((hptr = gethostbyaddr((char *)&addr, 4, AF_INET)) == 0) {
    printf("gethostbyaddr error, h_errno = %d\n", h_errno);
    exit(0);
  }

  printf("Official hostname: %s\n", hptr->h_name);
  for(pptr = hptr->h_aliases; *pptr; pptr++) 
    printf("\talias: %s\n", *pptr);

  switch(hptr->h_addrtype) {
  case AF_INET:
    for (pptr = hptr->h_addr_list; *pptr; pptr++) {
      printf("\taddress: %d.%d.%d.%d\n", (unsigned char)**pptr, 
             (unsigned char)*(*pptr+1), (unsigned char)*(*pptr+2), 
             (unsigned char)*(*pptr+3));
      /*      printf("\taddress: %s\n", inet_ntoa(*pptr)); */
    }
    break;
  default:
    printf("unknown address type\n");
  }
  return 0;
}
