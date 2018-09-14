#include <stdio.h>
#include <stdlib.h>

int main()
{
  unsigned short ii = 0x0102;

  unsigned char *ptr;

  ptr = (unsigned char *)&ii;  

  if (sizeof(ii) != 2) {
    printf("size of short = %d, can't decide.\n", sizeof(ii));
    exit(1);
  }

  printf("ii = %d\n", ii);

  if ((*ptr == 0x01) && (*(ptr+1) == 0x02)) {
    printf ("big endien, *ptr == 0x01, *(ptr+1) == 0x02\n");
    return 0;
  }
  if ((*ptr == 0x02) && (*(ptr+1) == 0x01)) {
    printf ("little endien, *ptr == 0x02, *(ptr+1) = 0x01.\n");
    return 0;
  }

  printf("unknown.\n");
  return 0;
}
