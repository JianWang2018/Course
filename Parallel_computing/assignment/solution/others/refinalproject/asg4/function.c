#include "global.h"
#include "function.h"

int start_time;
/*********** load function **********/
void load_user(){
  int i=0;
  FILE *fd;
  char buf1[100], user1[20], pass1[20];
  
  if((fd = fopen("userdb.txt","a+")) == NULL)
    {
      printf("error in opening user database file.\n");
      exit(0);
    }
  while(fgets (buf1 , sizeof(buf1) , fd) != NULL){
    
    sscanf(buf1,"%s %s *\n",user1,pass1);
    strcpy(user_array[i++], user1);
    //printf("user %s %d\n",user_array[i-1],i-1);
    
  }
  fclose(fd);
  count_user=i;
}
/*********** check_user function **********/
int check_user(char user[]){
  int i=0;
  while(i<count_user)
    if(strcmp(user_array[i++],user)==0)
      return(1);
  return(0);
}

/*********** check_pass function **********/
int check_pass(char user[],char pass[]){
  int i=0;
  FILE *fd;
  char buf1[100], user1[20], pass1[20];
  
  if((fd = fopen("userdb.txt","a+")) == NULL)
    {
      printf("error in opening user database file.\n");
      exit(0);
    }
  while(fgets (buf1 , sizeof(buf1) , fd) != NULL){
    
  sscanf(buf1,"%s %s *\n",user1,pass1);
    //printf("username and password %s %s\n",user1,pass1);
    if(!strcmp(user1,user) && !strcmp(pass1,pass))
      {
	fclose(fd);return(1);
      }
  }
  fclose(fd);
  return(0);
}

/*********** quit function *********/
void quit(int i){
  int game;
  char buf1[500];
  /***** if the user is in a game he loses *****/

  game = find_game_playing(i);
  if(game != -1){
    if(!strcmp(user_flag[i],cur_games[game].b_user)){
      save_result(game,2);
      memset(buf1,'\0',sizeof(buf1));
      sprintf(buf1,"%s disconnected. %s wins.\n",cur_games[game].b_user,cur_games[game].w_user);
      write(client[is_online(cur_games[game].w_user)], buf1, strlen(buf1));
    }
    else{
      save_result(game,1);
      memset(buf1,'\0',sizeof(buf1));
      sprintf(buf1,"%s disconnected. %s wins.\n",cur_games[game].b_user,cur_games[game].w_user);
      write(client[is_online(cur_games[game].w_user)], buf1, strlen(buf1));
    }
    cur_games[game].move = 1;
    cur_games[game].dur = -1;  
  }


  close(client[i]);
  FD_CLR(client[i], &allset);
  client[i] = -1;
  user_flag[i]="0";
  count_unread[i]=0;
  command_counter[i]=0;
}

/*********** register function *********/
int reg(char user[], char pass[]){
  FILE *fd;
  char file_name[40];
  
  if(check_user(user)){
    //printf("User %s already registered on server.\n",user);
    return(1);
  }
  else{
    if((fd = fopen("userdb.txt","a+")) == NULL)
      {
	printf("error in opening user database file.\n");
	exit(0);
      }
    fprintf(fd,"%s %s 0 0 0 <none>\n",user,pass);
    fclose(fd);
    //here we have to update the user_array && open a mail file for user
    load_user();
    strcpy(file_name,"mail/");
    strcat(file_name,user);
    if((fd = fopen(file_name,"a+")) == NULL)
      {
	printf("error in creating user mail file.\n");
	exit(0);
      }
    fclose(fd);
  }
  return(0);
}

/*********** tell function *********/
int tell(char sender[],char user[],char msg[]){
  int i;
  char msg_tmp[500];
  for(i=0;i<MAXCONN;i++)
    if(!strcmp(user_flag[i],user)) break;
  if(i == MAXCONN) return 0;
  sprintf(msg_tmp,"%s says: %s\n",sender,msg);
  if(write(client[i],msg_tmp,strlen(msg_tmp)*sizeof(char))) return(1);
  return(0);
}

/*********** shout function *********/
int shout(char sender[], char msg[]){
  int i;
  char msg_tmp[500];
  sprintf(msg_tmp,"%s says: %s\n",sender,msg);
  for(i=0;i<MAXCONN;i++)
    write(client[i],msg_tmp,strlen(msg_tmp)*sizeof(char));
  if(i == MAXCONN) return(1);
  return(0);
}

/*********** change password function *********/
int change_password(char user[], char new_pass[]){
  
  int flag=0;
  int win, lose, draw;
  char info[100]="\0";
  FILE *fd,*fd1; 
  char buf1[100]="\0", user1[20]="\0", pass1[20]="\0";
  //printf("new pass .. user %s %s\n",new_pass,user);
  win = lose = draw = 0;
  if((fd = fopen("userdb.txt","r+")) == NULL)
    {
      printf("error in opening user database file.\n");
      exit(0);
    }
   if((fd1 = fopen("out.txt","w")) == NULL)
    {
      printf("error in opening out.txt file.\n");
      exit(0);
    }
   while(fgets (buf1 , sizeof(buf1) , fd) != NULL){
     memset(info,'\0',100);
     sscanf(buf1,"%s %s %d %d %d %s\n",user1,pass1,&win,&lose,&draw,info);
     
     if(!strcmp(user1,user) ){
       strcpy(pass1,new_pass);
       flag=1;
     }
     fprintf(fd1,"%s %s %i %i %i %s\n",user1,pass1,win,lose,draw,info);
   }
   fclose(fd);fclose(fd1);
   system("cp out.txt userdb.txt");
   if(flag==1)
     return(1);
   else
     return(0);
}
/*********** list all user online function *********/
int who(char online_user[300][40]){
  int i,k=0;
  
  for (i=0; i<MAXCONN; i++) {
    if( client[i] != -1 && user_flag[i] != "0")
      strcpy(online_user[k++],user_flag[i]);
  }
  return k;
}
/*********** send an email function *********/
int mail(char sender[],char reci[],char title[]){
  FILE *fd;
  char file_name[40],buf[40],buf1[100],msg[500];
  int i,flag;
  time_t rawtime;
  struct tm * timeinfo;
  
  strcpy(file_name,"mail/");
  strcat(file_name,reci);
  if((fd = fopen(file_name,"a+")) == NULL)
    {
      printf("error in open user mail file.\n");
      exit(0);
    }
  for(i=0;i<MAXCONN;i++)
    if(!strcmp(user_flag[i],sender)) break;
  if(i == MAXCONN) return(0);

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  //printf ( "Current local time and date: %s", asctime (timeinfo) );

  fprintf(fd,"##%s\n",sender);
  fprintf(fd,"%s\n",title);
  fprintf(fd,"%s\n","0");
  fprintf(fd,"%s", asctime (timeinfo) );
			  
  memset(buf1,'\0',sizeof(buf1));
  sprintf(buf1,"Please input mail body, finishing with '.' at the beginning of a line\n");
  write(client[i], buf1, sizeof(buf1));
  read(client[i], msg, sizeof(msg));
  while(!(msg[0]=='.')){
    
    fprintf(fd,"%s",msg);
    //printf("%s",msg);
    memset(msg,'\0',sizeof(msg));
    read(client[i], msg, sizeof(msg));
  }
  fclose(fd);
  flag=0;
  for(i=0;i<MAXCONN;i++)
    if(!strcmp(user_flag[i],reci)) {flag=i;break;}
  if(i == MAXCONN) flag=-1;
  if(flag!=-1)
    {  
      sprintf(buf,"A new message just arrived.\n");
      if(!write(client[i],buf,strlen(buf))) return(0);
    }
  
  return(1);
}
/*********** list email function *********/
int list_mail(int index){
  FILE *fd;
  char file_name[40],buf1[500],buf[500],flag,sender[20],title[20],date[100];
  int i,count=0,flag_w=0;
	
  
  strcpy(file_name,"mail/");
  strcat(file_name,user_flag[index]);
  if((fd = fopen(file_name,"a+")) == NULL)
    {
      printf("error in open user mail file.\n");
      exit(0);
    }
  i=0;
  while(fgets (buf1 , sizeof(buf1) , fd) != NULL){
    if(buf1[0]=='#' && buf1[1]=='#')
      {
	sscanf(buf1,"##%s\n",sender);
	//printf("sender %s\n",sender);
	i=1;count++;flag_w=0;
      }
    else if(i==1){
      sscanf(buf1,"%s\n",title);
      //strcpy(title,buf1);
      //printf("title %s\n",title);
      i++;
    }
    else if(i==2){
      if(buf1[0]=='0')
	flag='N';
      else
	flag=' ';
      i++;
    }
    else if(i==3){
      //sscanf(buf1,"%s\n",date);
      strcpy(date,buf1);
      date[strlen(date)-1]='\0';
      //printf("date %s\n",date);
      i++;
    }
    else if(i==4 && flag_w==0){
      //printf("count flag %d %s
      sprintf(buf," %d  %c\t%s\t%s\t%s\n",count,flag,sender,title,date);
      //printf("buf %d %d\n",strlen(buf),sizeof(buf));
      write(client[index], buf, strlen(buf));
      memset(buf,'\0',sizeof(buf));
      flag_w=1;
    }
  }
  if(i==0){
    sprintf(buf,"You have no messages.\n");
    write(client[index], buf, strlen(buf));
    
  }
  fclose(fd);
  return(1);
}

/*********** read a specefic email function *********/
int read_mail(int index,int msg_num){
  
  FILE *fd;
  char file_name[40],buf1[500],buf[500];
  int i=0;
	  
  strcpy(file_name,"mail/");
  strcat(file_name,user_flag[index]);
  if((fd = fopen(file_name,"r+")) == NULL)
    {
      printf("error in open user mail file.\n");
      exit(0);
    }

  while(i!=msg_num){
    fgets (buf1 , sizeof(buf1) , fd);
    if(buf1[0]=='#' && buf1[1]=='#')
      i++;
  }
  if(i==msg_num){
    sscanf(buf1,"##%s",buf1);
    sprintf(buf,"From: %s\n",buf1);
    write(client[index], buf, strlen(buf));
    memset(buf,'\0',sizeof(buf));
    fgets (buf1 , sizeof(buf1) , fd);
    sprintf(buf,"Title: %s",buf1);
    write(client[index], buf, strlen(buf));
    fgets (buf1 , sizeof(buf1) , fd);
    
    if (fseek(fd, -strlen(buf1), SEEK_CUR) == -1) {
      printf("fseek failed\n");
      exit(0);
    }
    
    fprintf(fd,"%s\n","1");
    memset(buf,'\0',sizeof(buf));
    fgets (buf1 , sizeof(buf1) , fd);
    sprintf(buf,"Time: %s\n",buf1);
    write(client[index], buf, strlen(buf));
    memset(buf,'\0',sizeof(buf));
    
    while( (fgets (buf1 , sizeof(buf1) , fd))!=NULL){
      if(buf1[0]!='#' && buf1[1]!='#')
	write(client[index], buf1, strlen(buf1));
      else 
	break;
      
    }
    
  }
  fclose(fd);
  return(1);

}
/*********** delete a specefic email function *********/
int del_mail(int index,int msg_num){

  FILE *fd,*fd1;
  char file_name[40],buf1[500],buf[500];
  int i=0;
  
  strcpy(file_name,"mail/");
  strcat(file_name,user_flag[index]);
  if((fd = fopen(file_name,"r+")) == NULL)
    {
      printf("error in open user mail file.\n");
      exit(0);
    }

  if((fd1 = fopen("mail/temp","w")) == NULL)
    {
      printf("error in open temp file.\n");
      exit(0);
    }

  while((fgets (buf1 , sizeof(buf1) , fd))!=NULL){
	
    if(buf1[0]=='#' && buf1[1]=='#')
      i++;
    if(i!=msg_num)
      fprintf(fd1,"%s",buf1);
  }
  fclose(fd);fclose(fd1);
  strcpy(buf,"cp mail/temp ");
  strcat(buf,file_name);
  system(buf);
  return(1);
}
/**** count the unread email of user to when he login show him  */
int unread_mail(int index){
	
  FILE *fd;
  char file_name[40],buf1[500];
  int i,count=0;
  
  strcpy(file_name,"mail/");
  strcat(file_name,user_flag[index]);
  if((fd = fopen(file_name,"a+")) == NULL)
    {
      printf("error in open user mail file.\n");
      exit(0);
    }
  i=0;
  while(fgets (buf1 , sizeof(buf1) , fd) != NULL){
    if(buf1[0]=='#' && buf1[1]=='#')
      i=1;
    else if(i==1)
      i++;
    else if(i==2){
      if(buf1[0]=='0')
	count++;
      i++;
    }
    else if(i==3)
      i++;
    else if(i==4 );
  }

  fclose(fd);
  return(count);
}

/**** initialize a match *****/
int init_match(char b_user[20], char w_user[20], int dur, int starter){
  int i, j;
  char buf[500];
  for(i=0;i<MAXPEND;i++){
    if(!strcmp(pending_games[i].b_user,b_user) && !strcmp(pending_games[i].w_user,w_user) && pending_games[i].dur == dur){
      /* if there is a pending game with the same specifications start it. */
      for(j=0;j<MAXGAME;j++){
	if(cur_games[j].dur == -1){
	  empty_board(j);
	  strcpy(cur_games[j].b_user,b_user);
	  strcpy(cur_games[j].w_user,w_user);
	  cur_games[j].dur = dur;
	  cur_games[j].b_dur = dur;
	  cur_games[j].w_dur = dur;
	  pending_games[i].dur = -1;
	  start_time = time(NULL);
	  draw_game(j);
	  break;
	}
      }
      if(j == MAXGAME) printf("Game Max reached.\n",j);
      else break;
    }
  }

  /* if no matching games pending, then send match request */

  if(i==MAXPEND){

        
    /* store the match in pending games */
    for(j=0;j<MAXPEND;j++){
      if(pending_games[j].dur == -1){
	strcpy(pending_games[j].b_user,b_user);
	strcpy(pending_games[j].w_user,w_user);
	pending_games[j].dur = dur;
	break;
      }
    }
    if(j == MAXPEND) return(0);
    /* inform the target user of game invitation */
    memset(buf,'\0',sizeof(buf));
    if(starter == 0) {
      sprintf(buf,"%s invite you for a game <match %s w %i>.\n",pending_games[j].b_user,pending_games[j].b_user,pending_games[j].dur);
      write(client[is_online(w_user)], buf, strlen(buf));
    }
    else if(starter == 1) {
      sprintf(buf,"%s invite you for a game <match %s b %i>.\n",pending_games[j].w_user,pending_games[j].w_user,pending_games[j].dur);
      write(client[is_online(b_user)], buf, strlen(buf));
    }
  }
  return(1);
}

/***** empty the playing board ****/
void empty_board(int game){
  int i, j;
  for(i=0;i<3;i++)
    for(j=0;j<3;j++)
      cur_games[game].board[i][j] = '.';
  cur_games[j].move = 1;
  for(i=0;i<10;i++){
    cur_games[game].obs[i] = -1;
  }
}

/***** find id of an online username *****/
int is_online(char username[]){
  int i;
  for(i=0;i<MAXCONN;i++)
    if(!strcmp(user_flag[i],username)) {
      return(i);
    }
  return(-1);
}

/*** return number of current games ***/
int game(int user){
  int i,c=0;
  char *buf=malloc(500*sizeof(char)),*buf1=malloc(500*sizeof(char));
  
  for(i=0;i<MAXGAME;i++)
    if(cur_games[i].dur != -1){
      sprintf(buf1,"Game %i: %s vs %s, %i moves\n",c,user_flag[is_online(cur_games[i].b_user)],user_flag[is_online(cur_games[i].w_user)],cur_games[i].move);
      strcat(buf,buf1);
      c++;
    }
  memset(buf1,'\0',500);
  sprintf(buf1,"Total %i game(s);\n%s",c,buf);
  write(client[user], buf1, strlen(buf1));
  return(1);
}

/*** draw game board for players and observers ***/
void draw_game(int id){
  int i, j;
  char *buf=malloc(500*sizeof(char)), *buf1=malloc(500*sizeof(char));
 
  sprintf(buf,"\n");
  strcat(buf1,buf);
  sprintf(buf,"Black:\t%s\tWhite:\t%s\n",cur_games[id].b_user,cur_games[id].w_user);
  strcat(buf1,buf);
  sprintf(buf,"Time:\t%i\tTime:\t%i\n",cur_games[id].b_dur,cur_games[id].w_dur);
  strcat(buf1,buf);
  sprintf(buf,"\n");
  strcat(buf1,buf);
  sprintf(buf,"   1 2 3\n");
  strcat(buf1,buf);
  sprintf(buf,"A  %c %c %c\n",cur_games[id].board[0][0],cur_games[id].board[0][1],cur_games[id].board[0][2]);
  strcat(buf1,buf);
  sprintf(buf,"B  %c %c %c\n",cur_games[id].board[1][0],cur_games[id].board[1][1],cur_games[id].board[1][2]);
  strcat(buf1,buf);
  sprintf(buf,"C  %c %c %c\n",cur_games[id].board[2][0],cur_games[id].board[2][1],cur_games[id].board[2][2]);
  strcat(buf1,buf);
  write(client[is_online(cur_games[id].b_user)], buf1, strlen(buf1));
  write(client[is_online(cur_games[id].w_user)], buf1, strlen(buf1));
  
  for(i=0;i<10;i++){
    if(cur_games[id].obs[i] != -1){
      write(client[cur_games[id].obs[i]], buf1, strlen(buf1));
    }
  }
}

/**** observe a game ****/
int observe(int id, int user){
  int i;
  if(find_game_obs(user) > -1) {return(0);}
  if(cur_games[id].dur == -1) {return(0);}
  for(i=0;i<10;i++){
    if(cur_games[id].obs[i] == -1){
      cur_games[id].obs[i] = user;
      refresh(user);
      return(1);
    }
  }
  return(0);
}

/**** unobserve a game ****/
int unobserve(int user){
  int i, j;
  j = find_game_obs(user);
  if(j == -1) return(0);
  for(i=0;i<10;i++){
    if(cur_games[j].obs[i] == user){
      cur_games[j].obs[i] = -1;
      return(1);
    }
  }
  return(0);
}

/**** find the game observing ****/
int find_game_obs(int user){
  int i, j;
  
  for(i=0;i<MAXGAME;i++){
    for(j=0;j<10;j++){
      if(cur_games[i].obs[j] == user){
	return(i);
      }
    }
  }
  return(-1);
}

/**** find the game playing ****/
int find_game_playing(int user){
  int i;
  
  for(i=0;i<MAXGAME;i++){
    if(!strcmp(user_flag[user],cur_games[i].b_user) || !strcmp(user_flag[user],cur_games[i].w_user))
      return(i);
  }
  return(-1);
}

/*** refresh the game display ***/
int refresh(int user){
  int id;
  char *buf=malloc(500*sizeof(char)), *buf1=malloc(500*sizeof(char));
  
  id = find_game_obs(user);
  if(id == -1) {
    id = find_game_playing(user);
    if(id == -1) return(0);
  }
  sprintf(buf,"\n");
  strcat(buf1,buf);
  sprintf(buf,"Black:\t%s\tWhite:\t%s\n",cur_games[id].b_user,cur_games[id].w_user);
  strcat(buf1,buf);
  sprintf(buf,"Time:\t%i\tTime:\t%i\n",cur_games[id].b_dur,cur_games[id].w_dur);
  strcat(buf1,buf);
  sprintf(buf,"\n");
  strcat(buf1,buf);
  sprintf(buf,"   1 2 3\n");
  strcat(buf1,buf);
  sprintf(buf,"A  %c %c %c\n",cur_games[id].board[0][0],cur_games[id].board[0][1],cur_games[id].board[0][2]);
  strcat(buf1,buf);
  sprintf(buf,"B  %c %c %c\n",cur_games[id].board[1][0],cur_games[id].board[1][1],cur_games[id].board[1][2]);
  strcat(buf1,buf);
  sprintf(buf,"C  %c %c %c\n",cur_games[id].board[2][0],cur_games[id].board[2][1],cur_games[id].board[2][2]);
  strcat(buf1,buf);
  write(client[user], buf1, strlen(buf1));
  return(1);
}

/*** make a move ***/
int move(int game, int user, char move[]){
  int row, col, i;
  char symbol;
  char buf[500];
  if(cur_games[game].move % 2) symbol = 'X';
  else symbol = 'O';

  if((!strcmp(user_flag[user],cur_games[game].b_user) && cur_games[game].move % 2 && cur_games[game].move >= 1 && cur_games[game].move <= 9) || (!strcmp(user_flag[user],cur_games[game].w_user) && !(cur_games[game].move % 2) && cur_games[game].move >= 1 && cur_games[game].move <= 9)){
    if(move[0] == 'a') row = 0;
    else if(move[0] == 'b') row = 1;
    else if(move[0] == 'c') row = 2;
    col = move[1] - '0' - 1;

    if(cur_games[game].board[row][col] == '.'){
      cur_games[game].board[row][col] = symbol;
      cur_games[game].move++;
      if(symbol == 'X') cur_games[game].b_dur -= time(NULL) - start_time;
      else cur_games[game].w_dur -= time(NULL) - start_time;
      if(cur_games[game].b_dur < 0 ) cur_games[game].b_dur = 0;
      if(cur_games[game].w_dur < 0 ) cur_games[game].w_dur = 0;
      start_time = time(NULL);
      draw_game(game);
      if(check_win(game,symbol)){
	if(symbol == 'X')
	  save_result(game,1);
	else save_result(game,2);
	memset(buf,'\0',500);
	sprintf(buf,"%s wins.\n",user_flag[user]);
	write(client[is_online(cur_games[game].w_user)], buf, strlen(buf));
	write(client[is_online(cur_games[game].b_user)], buf, strlen(buf));
	for(i=0;i<10;i++){
	  if(cur_games[game].obs[i] != -1){
	    write(client[cur_games[game].obs[i]], buf, strlen(buf));
	  }
	}
	cur_games[game].dur = -1;
	cur_games[game].move = 1;
      }
      else if(cur_games[game].move == 10){
	memset(buf,'\0',500);
	save_result(game,0);
	sprintf(buf,"The game is a draw.\n");
	write(client[is_online(cur_games[game].b_user)], buf, strlen(buf));
	write(client[is_online(cur_games[game].w_user)], buf, strlen(buf));
	for(i=0;i<10;i++){
	  if(cur_games[game].obs[i] != -1){
	    write(client[cur_games[game].obs[i]], buf, strlen(buf));
	  }
	}
	cur_games[game].dur = -1;
	cur_games[game].move = 1;
      }
      else if(check_time(game) == 1){	
	memset(buf,'\0',500);
	save_result(game,2);
	sprintf(buf,"%s wins by time.\n",cur_games[game].w_user);
	write(client[is_online(cur_games[game].w_user)], buf, strlen(buf));
	write(client[is_online(cur_games[game].b_user)], buf, strlen(buf));
	for(i=0;i<10;i++){
	  if(cur_games[game].obs[i] != -1){
	    write(client[cur_games[game].obs[i]], buf, strlen(buf));
	  }
	}
	cur_games[game].dur = -1;
	cur_games[game].move = 1;
      }
      else if(check_time(game) == 2){
	memset(buf,'\0',500);
	save_result(game,1);
	sprintf(buf,"%s wins by time.\n",cur_games[game].b_user);
	write(client[is_online(cur_games[game].w_user)], buf, strlen(buf));
	write(client[is_online(cur_games[game].b_user)], buf, strlen(buf));
	for(i=0;i<10;i++){
	  if(cur_games[game].obs[i] != -1){
	    write(client[cur_games[game].obs[i]], buf, strlen(buf));
	  }
	}
	cur_games[game].dur = -1;
	cur_games[game].move = 1;
      }
      return(1);
    }
    else return(-1);
  }
  else return(0);
}

/*** check if symbol wins ***/
int check_win(int game, char symbol){
  int row, col;
  
  for (row = 0; row < 3; row++){
    if((cur_games[game].board[row][0] == symbol) && (cur_games[game].board[row][1] == symbol) && (cur_games[game].board[row][2] == symbol))
      return 1;
  }

  for (col = 0; col < 3; col++){
    if((cur_games[game].board[0][col] == symbol) && (cur_games[game].board[1][col] == symbol) && (cur_games[game].board[2][col] == symbol))
      return 1;
  }

  if((cur_games[game].board[0][0] == symbol) && (cur_games[game].board[1][1] == symbol) && (cur_games[game].board[2][2] == symbol))
    return 1;

  if((cur_games[game].board[0][2] == symbol) && (cur_games[game].board[1][1] == symbol) && (cur_games[game].board[2][0] == symbol))
    return 1;
  return 0;
}

/********* check_time ***********/
int check_time(int game){
  if(cur_games[game].b_dur <= 0)
    return(1);
  else if(cur_games[game].w_dur <= 0)
    return(2);
  return(0);
}

/********* save result of game in file ********/
void save_result(int game, int res){

  int win, lose, draw;
  char info[100]="\0";
  FILE *fd,*fd1; 
  char buf1[100]="\0", user1[20]="\0", pass1[20]="\0";
  win = lose = draw = 0;

  if((fd = fopen("userdb.txt","r+")) == NULL)
    {
      printf("error in opening user database file.\n");
      exit(0);
    }
   if((fd1 = fopen("tmpresult","w")) == NULL)
    {
      printf("error in opening out.txt file.\n");
      exit(0);
    }
   while(fgets (buf1 , sizeof(buf1) , fd) != NULL){
     memset(info,'\0',100);
     sscanf(buf1,"%s %s %d %d %d %s\n",user1,pass1,&win,&lose,&draw,info);
     
     if(!strcmp(user1,cur_games[game].b_user) ){
       if(res == 0) draw++;
       else if(res == 1) win++;
       else if(res == 2) lose++;
     }
     if(!strcmp(user1,cur_games[game].w_user) ){
       if(res == 0) draw++;
       else if(res == 2) win++;
       else if(res == 1) lose++;
     }
     fprintf(fd1,"%s %s %i %i %i %s\n",user1,pass1,win,lose,draw,info);
   }
   fclose(fd);fclose(fd1);
   system("cp tmpresult userdb.txt");
}
/********* kibitz and ' commands **********/
void kibitz(int game, int user, char msg[]){
  int i;
  char buf[500];
  memset(buf,'\0',500);
  sprintf(buf,"Kibitz* %s: %s\n",user_flag[user],msg);
  
  for(i=0;i<10;i++){
    if(cur_games[game].obs[i] != -1) write(client[cur_games[game].obs[i]],buf,strlen(buf));
  }
}


/********* show the stats of users**********/
void stats(char target[], int user){
  int i, win, lose, draw;
  float rate;
  char buf[500], buf1[500], user1[20], pass1[20], status[10], info[100]="\0";
  FILE *fd;
  
  strcpy(status,"online");
  rate = 0.0;
  if((fd = fopen("userdb.txt","a+")) == NULL)
    {
      printf("error in opening user database file.\n");
      exit(0);
    }
  while(fgets (buf1 , sizeof(buf1) , fd) != NULL){
    sscanf(buf1,"%s %s %d %d %d %[^\n]",user1,pass1,&win,&lose,&draw,info);    
    if(!strcmp(user1,target)) break;
  }
  fclose(fd);
  memset(buf,'\0',500);
  if(win + lose + draw) rate = (float)(win * 2 + draw)/(float)(win + lose + draw);
  if(is_online(target) == -1) strcpy(status,"offline");
  sprintf(buf,"User: %s\nInfo: %s\nRating: %f\nWins: %i, Loses: %i, Draws: %i\n%s is currently %s.\n",target,info,rate,win,lose,draw,target,status);
  write(client[user],buf,strlen(buf));
}

/********* Resign from a game **********/
void resign(int game, int user){
  if(user_flag[user] == cur_games[game].b_user)
    save_result(game,2);
  else save_result(game,1);
  cur_games[game].move = 1;
  cur_games[game].dur = -1;  
}

/********* info **********/
void info(int user, char msg[]){
  int win, lose, draw;
  char info[100]="\0";
  FILE *fd,*fd1; 
  char buf1[100]="\0", user1[20]="\0", pass1[20]="\0";
  win = lose = draw = 0;

  if((fd = fopen("userdb.txt","r+")) == NULL)
    {
      printf("error in opening user database file.\n");
      exit(0);
    }
   if((fd1 = fopen("tmpinfo","w")) == NULL){
     printf("error in opening out.txt file.\n");
     exit(0);
   }
   while(fgets (buf1 , sizeof(buf1) , fd) != NULL){
     memset(info,'\0',100);
     sscanf(buf1,"%s %s %d %d %d %s\n",user1,pass1,&win,&lose,&draw,info);
     
     if(!strcmp(user1,user_flag[user]) ){
       strcpy(info,msg);
     }
     
     fprintf(fd1,"%s %s %i %i %i %s\n",user1,pass1,win,lose,draw,info);
   }
   fclose(fd);fclose(fd1);
   system("cp tmpinfo userdb.txt");
}
/******help and ? function*******/
void help(int index){
  char buf[500];
  
  memset(buf,'\0',500);
  sprintf(buf,"\tCommands supported:\n");
  write(client[index],buf,strlen(buf));
  memset(buf,'\0',500);
  sprintf(buf,"\t?                       # print this message\n");
  write(client[index],buf,strlen(buf));
  memset(buf,'\0',500);
  sprintf(buf,"\t' <msg>                 # Comment on a game\n");
  write(client[index],buf,strlen(buf));
  memset(buf,'\0',500);
  sprintf(buf,"\tdeletemail <msg_num>    # Delete the particular mail\n");
  write(client[index],buf,strlen(buf));
  memset(buf,'\0',500);
  sprintf(buf,"\texit                    # quit the system\n");
  write(client[index],buf,strlen(buf));
  memset(buf,'\0',500);
  sprintf(buf,"\tgame                    # list all current games\n");
  write(client[index],buf,strlen(buf));
  memset(buf,'\0',500);
  sprintf(buf,"\thelp                    # print this message\n");
  write(client[index],buf,strlen(buf));
  memset(buf,'\0',500);
  sprintf(buf,"\tinfo <msg>              # change your information to <msg>\n");
  write(client[index],buf,strlen(buf));
  memset(buf,'\0',500);
  sprintf(buf,"\tkibitz <msg>            # Comment on a game\n");
  write(client[index],buf,strlen(buf));
  memset(buf,'\0',500);
  sprintf(buf,"\tlistmail                # List the header of the mails\n");
  write(client[index],buf,strlen(buf));
  memset(buf,'\0',500);
  sprintf(buf,"\tmail <id> <title>       # Send id a mail\n");
  write(client[index],buf,strlen(buf));
  memset(buf,'\0',500);
  sprintf(buf,"\tmatch <name> <b|w> t    # Trying to start a game\n");
  write(client[index],buf,strlen(buf));
  memset(buf,'\0',500);
  sprintf(buf,"\tobserve <game_num>      # Observe a game\n");
  write(client[index],buf,strlen(buf));
  memset(buf,'\0',500);
  sprintf(buf,"\tpasswd <new>            # change password\n");
  write(client[index],buf,strlen(buf));
  memset(buf,'\0',500);
  sprintf(buf,"\tquit                    # quit the system\n");
  write(client[index],buf,strlen(buf));
  memset(buf,'\0',500);
  sprintf(buf,"\treadmail <msg_num>      # Read the particular mail\n");
  write(client[index],buf,strlen(buf));
  memset(buf,'\0',500);
  sprintf(buf,"\tresign                  # Resign a game\n");
  write(client[index],buf,strlen(buf));
  memset(buf,'\0',500);
  sprintf(buf,"\trefresh                 # Refresh a game\n");
  write(client[index],buf,strlen(buf));
  memset(buf,'\0',500);
  sprintf(buf,"\tshout <msg>             # shout <msg> to every one online\n");
  write(client[index],buf,strlen(buf));
  memset(buf,'\0',500);
  sprintf(buf,"\tstats [name]            # Display user information\n");
  write(client[index],buf,strlen(buf));
  memset(buf,'\0',500);
  sprintf(buf,"\ttell <name> <msg>       # tell user <name> message\n");
  write(client[index],buf,strlen(buf));
  memset(buf,'\0',500);
  sprintf(buf,"\tunobserve               # Unobserve a game\n");
  write(client[index],buf,strlen(buf));
  memset(buf,'\0',500);
  sprintf(buf,"\twho                     # List all online users\n");
  write(client[index],buf,strlen(buf));
}
