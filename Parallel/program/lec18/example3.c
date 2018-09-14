#include <netdb.h>
#include <sys/utsname.h>
#include <stdio.h>
#include <stdlib.h>

extern int h_errno;

int main()
{
  struct hostent *hptr;
  struct utsname myname;

  char mychname[100];

  if (uname(&myname) < 0) {
    perror(": uname");
    exit(0);
  }

  if ((hptr = gethostbyname(myname.nodename)) == NULL) {
    printf("gethostbyname error.\n");
    exit(0);
  }

  printf("myname = %s, full name = %s, ip = %d.%d.%d.%d\n", myname.nodename, 
         hptr->h_name, (unsigned char)*hptr->h_addr_list[0],
         (unsigned char)*(hptr->h_addr_list[0]+1),
         (unsigned char)*(hptr->h_addr_list[0]+2),
         (unsigned char)*(hptr->h_addr_list[0]+3));

  if (gethostname(mychname, 100) < 0) {
    perror(": gethostname");
    exit(0);
  }

  printf("my name from gethostname = %s\n", mychname);
  return 0;
  
}





