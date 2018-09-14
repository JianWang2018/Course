#include <stdio.h>

int main()
{
  unsigned short ii = 0x0102;

  unsigned char *ptr;

  ptr = (unsigned char *)&ii;  

  /* ii = htons(0x0102); */

  if (sizeof(ii) != 2) {
    printf("size of short = %d, can't decide.\n", sizeof(ii));
    exit(1);
  }

  if ((*ptr == 0x01) && (*(ptr+1) == 0x02)) {
    printf ("big endien.\n");
    return 0;
  }
  if ((*ptr == 0x02) && (*(ptr+1) == 0x01)) {
    printf ("little endien.\n");
    return 0;
  }

  printf("unknown.\n");
  return 0;
}
