#include <netdb.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

extern int h_errno;

int main(int argc, char **argv)
{
  char *ptr, **pptr;
  struct hostent *hptr;
  struct timeval start, end;

  if (argc < 2) {
    printf("Usage: a.out hostname\n");
    exit(0);
  }

  if (gettimeofday(&start, NULL) < 0) {
    perror(": gettimeofday");
    exit(0);
  }
  
  if ((hptr = gethostbyname(argv[1])) == 0) {
    printf("gethostbyname error, h_errno = %d\n", h_errno);
    exit(0);
  }

  if (gettimeofday(&end, NULL) < 0) {
    perror(": gettimeofday");
    exit(0);
  }
  
  printf("Gethostbyname takes %10.5f ms\n", (end.tv_sec-start.tv_sec)*1000.0
	 + (end.tv_usec - start.tv_usec) / 1000.0);

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
