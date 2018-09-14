#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main()
{
  char ch;
  struct termios save_termios;
  int val;

  if (tcgetattr(STDIN_FILENO, &save_termios) < 0) /* get whatever is there */
    perror("getattr");

  save_termios.c_lflag &= ~ICANON; /* set non canonical mode */
  
  save_termios.c_cc[VMIN] = 0;  
  save_termios.c_cc[VTIME] = 0; 

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &save_termios) < 0) 
    perror("setattr");
  
  while (1) {
    printf("."); fflush(0);sleep(1);
    if (read(STDIN_FILENO, &ch, 1) > 0) {
      printf("!!%c,%d@@\n", ch, ch);
    }
  }
}









