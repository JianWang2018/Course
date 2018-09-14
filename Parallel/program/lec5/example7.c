#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
  int fd;
  if ((fd = creat("hole001", S_IRUSR | S_IWUSR)) == -1) {
    printf("File creation failed.\n");
    exit(1);
  }

  if (write(fd, "abcde", 5) == -1) {
    printf("write failed\n");
    exit(2);
  }

  if (lseek(fd, 40, SEEK_END) == -1) {
    printf("lseek failed\n");
    exit(3);
  }
  write(fd, "end", 3);
}
