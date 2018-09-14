/*
 * system call API hello world
 */

#include <unistd.h>
#include <string.h>

main()
{
  char buf[1000];
  int i;

  write (STDOUT_FILENO, "hello world! please input: \n", 
	 strlen("hello world! please input: \n"));
  
  i = read (0, buf, 1000);

  buf[i] = '\0';
  
  write(1, "the input is: ", 14);
  write(1, buf, strlen(buf));
}
