#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>


char history[1000][512];
int curr_history = 0;

char buff[512];
int curr_pos;

int curr_cmd = 0;

void redraw_buff()
{
  int i;
  printf("\r");
  printf("<myshell:%d> ", curr_cmd);

  for (i=0; i<50; i++) printf(" ");
  printf("\r");
  buff[curr_pos] = '\0';
  printf("<myshell:%d> %s", curr_cmd, buff); fflush(0);
}

int main()
{
  char ch;
  struct termios save_termios;
  int val;
  int curr_h;

  if (tcgetattr(STDIN_FILENO, &save_termios) < 0) /* get whatever is there */
    perror("getattr");

  /* save_termios.c_lflag &= ~ICANON;*/ /* set non canonical mode */
  save_termios.c_lflag &= ~(ECHO | ICANON); /* set non canonical mode, turn of
                                               echo */
  save_termios.c_cc[VMIN] = 0;  /* one byte at a time */
  save_termios.c_cc[VTIME] = 0; /* no timer */

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &save_termios) < 0) 
    perror("setattr");
  
  /*
  fcntl(STDIN_FILENO, F_GETFL, val);
  val |= O_NONBLOCK;
  fcntl(STDIN_FILENO, F_SETFL, val);
  */

  curr_pos = 0;

  printf("<myshell:%d> ", curr_cmd);fflush(0);
  while (1) {    
    if (read(STDIN_FILENO, &ch, 1) > 0) {
      if (ch == '\n') {
	putchar(ch);
	fflush(0);
        buff[curr_pos ++] = '\0';
	printf("buf = %s\n", buff);
        strcpy(history[curr_history++], buff);
        curr_h = curr_history;
        curr_cmd++;
        printf("<myshell:%d> ", curr_cmd);fflush(0);
        curr_pos = 0;
      } else if (ch == 1) { /* ctrl-A */ 
        if (curr_pos > 0) {
	  curr_pos --;
	  redraw_buff();
	}
      } else if (ch == 16) { /* ctrl-P */
        if (curr_h > 0) {
	  strcpy(buff, history[curr_h-1]);
          curr_h--;
          curr_pos = strlen(buff);
          redraw_buff();
	}
      } else {
	buff[curr_pos ++] = ch;
        putchar(ch);fflush(0);
      }
    }
   }
}









