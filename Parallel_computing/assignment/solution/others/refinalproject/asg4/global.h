/* global.h */

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>



#define MAXCONN 30
#define MAXPEND 100
#define MAXGAME 15

int client[MAXCONN];
char *user_flag[MAXCONN];
char user_array[300][40];
int count_user, count_unread[40];
int command_counter[MAXCONN];
fd_set allset;
struct game{
  char b_user[20];
  char w_user[20];
  int b_dur;
  int w_dur;
  int dur;
  int obs[10];
  char board[3][3];
  int move;
};
struct game cur_games[MAXGAME];
struct game pending_games[MAXPEND]; 
