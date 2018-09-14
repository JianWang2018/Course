#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

extern int h_errno;

int main(int argc, char **argv)
{
  char *ptr, **pptr;
  struct hostent *hptr;
  struct servent *sptr;

  if (argc < 2) {
    printf("Usage: a.out port [protocol]\n");
    exit(0);
  }
  
  if ((sptr = getservbyport(htons((unsigned short)atoi(argv[1])), argv[2])) == 0) {
    printf("getservbyname error, h_errno = %d\n", h_errno);
    exit(0);
  }

  printf("official service name: %s, port = %d, protocol = %s\n", 
          sptr->s_name, ntohs(sptr->s_port), sptr->s_proto);
  printf("\talias:\n");
  for(pptr = sptr->s_aliases; *pptr; pptr++) {
    printf("\t\t%s\n", *pptr);
  }
  return 0;
}
