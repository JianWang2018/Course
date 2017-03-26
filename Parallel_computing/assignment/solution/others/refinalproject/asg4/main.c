#include "global.h"
#include "function.h"

void sig_chld(int signo)
{
  pid_t pid;
  int stat;
  while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) 
    printf("child %d terminated.\n", pid);
  return ;
}


int main(int argc, char * argv[])
{
  int sockfd, rec_sock, len, i, j, k, m, online_counter=0;
  struct sockaddr_in addr, recaddr;
  struct sigaction abc;
  
  char buf[500],buf1[500]="\0",cmd[20],arg1[500]="\0",arg2[500]="\0",arg3[500]="\0", online_user[300][40];
  fd_set rset;
  int maxfd;
  
  
  /*** Load userdb to array user***/
  
  load_user();
  
  /*** initialize game structs ***/

  for(j=0;j<MAXPEND;j++)
    pending_games[j].dur = -1;
  for(j=0;j<MAXGAME;j++){
    cur_games[j].dur = -1;
    cur_games[j].move = 1;
    for(k=0;k<3;k++)
      for(m=0;m<3;m++)
	cur_games[j].board[k][m] = '.';
    for(k=0;k<10;k++)
      cur_games[j].obs[k] = -1;
  }

  for(j=0;j<count_user;j++)
    printf("userrr %s %d\n",user_array[j],j);
  
  abc.sa_handler = sig_chld;
  sigemptyset(&abc.sa_mask);
  abc.sa_flags = 0;
  
  sigaction(SIGCHLD, &abc, NULL);
  
  if (argc < 2) {
    printf("Usage: a.out port.\n");
    exit(0);
  }
  
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror(": Can't get socket");
    exit(1);
  }
  
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_family = AF_INET;
  addr.sin_port = htons((short)atoi(argv[1]));

  if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror(": bind");
    exit(1);
  }
  
  
  len = sizeof(addr);
  if (getsockname(sockfd, (struct sockaddr *)&addr, &len) < 0)
    {
      perror(": can't get name");
      _exit(1);
    }
  
  printf("ip = %s, port = %d\n", inet_ntoa(addr.sin_addr), addr.sin_port);
  
  if (listen(sockfd, 5) < 0) {
    perror(": bind");
    exit(1);
  }
  
  for (i=0; i<MAXCONN; i++) {client[i] = -1;user_flag[i] = "0";count_unread[i]=0;command_counter[i]=0;}
  
  FD_ZERO(&allset);
  FD_SET(sockfd, &allset);
  maxfd = sockfd;
  
  while (1) {
    rset = allset;
    select(maxfd+1, &rset, NULL, NULL, NULL);
    if (FD_ISSET(sockfd, &rset)) {
      /* somebody tries to connect */
      if ((rec_sock = accept(sockfd, (struct sockaddr *)(&recaddr), &len)) < 0) {
	if (errno == EINTR)
	  continue;
	else {
	  perror(":accept error");
	  exit(1);
	}
      }
      
      if (rec_sock < 0) {
	perror(": accept");
	exit(1);
      }
      
      /* print the remote socket information */
      
      printf("remote machine = %s, port = %x, %x.\n", inet_ntoa(recaddr.sin_addr), recaddr.sin_port, ntohs(recaddr.sin_port));
		
	  write(rec_sock, "\t\t-=-= AUTHORIZED USERS ONLY =-=-\t\n",36 );	
	  write(rec_sock,"You are attempting to log into online tic-tac-toe Server.\n",58 );	
	  write(rec_sock,"Please be advised by continuing that you agree to the terms of the\nComputer Access and Usage Policy of online tic-tac-toe Server.\n\n\n",132 );	
	  write(rec_sock, "Please enter your username(guest):\n", 36);

	  //memset(buf1,'\0',500);
	  //sprintf(buf1,"Please enter your username(guest):\n");
	  //write(rec_sock, buf1, sizeof(buf1));
      
      for (i=0; i<MAXCONN; i++) {
	if (client[i] < 0) {
	  client[i] = rec_sock; 
	  FD_SET(client[i], &allset);
	  break;
	}
      }
      if (i== MAXCONN) {
	printf("too many connections.\n");
        close(rec_sock);
      }
      if (rec_sock > maxfd) maxfd = rec_sock;
    }
    
    for (i=0; i<MAXCONN; i++) {
      if (client[i] < 0) continue;
      if (FD_ISSET(client[i], &rset)) {
	int num;
	/*memset(buf1,'\0',500);
	sprintf(buf1,"Please enter your username(guest):\n");
	write(client[i], buf1, sizeof(buf1));*/

	memset(buf,'\0',500);
	num = read(client[i], buf, 500);
        if (num == 0) {


	  /* if the user is in a game he loses */
	  k = find_game_playing(i);
	  if(k != -1){
	    if(!strcmp(user_flag[i],cur_games[k].b_user)){
	      save_result(k,2);
	      memset(buf1,'\0',sizeof(buf1));
	      sprintf(buf1,"%s disconnected. %s wins.\n",cur_games[k].b_user,cur_games[k].w_user);
	      write(client[is_online(cur_games[k].w_user)], buf1, strlen(buf1));
	    }
	    else{
	      save_result(k,1);
	      memset(buf1,'\0',sizeof(buf1));
	      sprintf(buf1,"%s disconnected. %s wins.\n",cur_games[k].w_user,cur_games[k].b_user);
	      write(client[is_online(cur_games[k].b_user)], buf1, strlen(buf1));
	    }
	    cur_games[k].move = 1;
	    cur_games[k].dur = -1;
	  }


	  /* client exits */
          close(client[i]);
          FD_CLR(client[i], &allset);
          client[i] = -1;
	  user_flag[i] = "0";
	  count_unread[i] = 0;
	  command_counter[i] = 0;
	} else {
	  if(!strcmp(user_flag[i],"0")){
	    /* Get the username */
	    memset(arg1,'\0',sizeof(arg1));
	    sscanf(buf,"%s\n",arg1);
	    if(!strcmp(arg1,"guest")){
	      user_flag[i] = malloc(5*sizeof(char));
	      strcpy(user_flag[i],"guest");
	      //show all commands for guest and tell him just can use register command
	      help(i);
	      memset(buf1,'\0',sizeof(buf1));
	      sprintf(buf1,"You login as a guest. The only command that you can use is\n'register username password'\n\n");
	      write(client[i], buf1, strlen(buf1));
	      memset(buf1,'\0',sizeof(buf1));
	      sprintf(buf1,"<%s: %d>",user_flag[i],command_counter[i]++);
	      write(client[i], buf1, strlen(buf1));

	    }
	    else if(check_user(arg1)){
	      memset(buf1,'\0',sizeof(buf1));
	      sprintf(buf1,"Enter your password.\n\0");
	      write(client[i], buf1, sizeof(buf1));
	      memset(arg2,'\0',sizeof(arg2));
	      read(client[i], arg2, sizeof(arg2));
	      sscanf(arg2,"%s\n",arg2);
	      if(check_pass(arg1,arg2)){

		/* check for double login */
		for(k=0;k<count_user;k++)
		  if(!strcmp(user_flag[k],arg1)){
		    memset(buf1,'\0',sizeof(buf1));
		    sprintf(buf1,"You are logged in from another terminal.\n");
		    write(client[i], buf1, sizeof(buf1));
		    quit(i);
		  }

		user_flag[i] = malloc(strlen(arg1)*sizeof(char));
		strcpy(user_flag[i],arg1);
		memset(buf1,'\0',sizeof(buf1));
		sprintf(buf1,"You are logged in successfully.\n");
		write(client[i], buf1, sizeof(buf1));
		
		//show the all commands   
		help(i);

		count_unread[i]=unread_mail(i);
		memset(buf1,'\0',sizeof(buf1));
		if(count_unread[i]!=0)
		  sprintf(buf1,"You have %d unread messages.\n",count_unread[i]);
		else
		  sprintf(buf1,"You have no unread messages.\n");
		write(client[i], buf1, sizeof(buf1));

		memset(buf1,'\0',sizeof(buf1));
		sprintf(buf1,"<%s: %d>",user_flag[i],command_counter[i]++);
		write(client[i], buf1, strlen(buf1));
	      }
	      else{
		memset(buf1,'\0',sizeof(buf1));
		sprintf(buf1,"Your Password is not correct.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	    }
	    else {
		memset(buf1,'\0',sizeof(buf1));
		sprintf(buf1,"Your username is not correct.\n");
		write(client[i], buf1, sizeof(buf1));
	    }
	  }
	  else if(!strcmp(user_flag[i],"guest")){
	    
	    //guest client
	    /* check the command */
	    memset(arg1,'\0',sizeof(arg1));
	    memset(arg2,'\0',sizeof(arg2));
	    memset(arg3,'\0',sizeof(arg3));
	    memset(cmd,'\0',sizeof(cmd));
	    sscanf(buf,"%s %s %s %s\n",cmd,arg1,arg2,arg3);
	    
	    /* quit and exit command */
	    
	    if(!strcmp(cmd,"quit") || !strcmp(cmd,"exit")){
	      quit(i);
	    }  
	   	    
	    /* register command */
	    
	    else if(!strcmp(cmd,"register")){
	      memset(buf1,'\0',sizeof(buf1));
	      if(strlen(arg1) == 0 || strlen(arg2)==0){
		sprintf(buf1,"username or password not entered.\n\0");
		write(client[i], buf1, sizeof(buf1));
	      }
	      else if(reg(arg1,arg2)){
		sprintf(buf1,"User %s is already registered.\n\0",arg1);
		write(client[i], buf1, sizeof(buf1));
	      }
	      else {
		sprintf(buf1,"User %s registered successfully.\n\0",arg1);
		write(client[i], buf1, sizeof(buf1));
	      }
	    }//end if cmd register
	    else{
	      memset(buf1,'\0',sizeof(buf1));
	      sprintf(buf1,"You are not supposed to do this.\nYou can only use 'register username password' as a guest.\n");
	      write(client[i], buf1, strlen(buf1));
	      
	    }
	    
	    memset(buf1,'\0',sizeof(buf1));
	    sprintf(buf1,"<%s: %d>",user_flag[i],command_counter[i]++);
	    write(client[i], buf1, strlen(buf1));
	    
	  }//end if username guest 
	  else{
	    
	    memset(arg1,'\0',sizeof(arg1));
	    memset(arg2,'\0',sizeof(arg2));
	    memset(arg3,'\0',sizeof(arg3));
	    memset(cmd,'\0',sizeof(cmd));
	    // client who has logged in
	    
	    
	    /* check the command */
	    sscanf(buf,"%s %s %s %s\n",cmd,arg1,arg2,arg3);
	    
	    /* quit and exit command */
	    
	    if(!strcmp(cmd,"quit") || !strcmp(cmd,"exit")){
	      quit(i);
	    }
	    
	    /* register command */
	    
	    else if(!strcmp(cmd,"register")){
	      memset(buf1,'\0',sizeof(buf1));
	      if(strlen(arg1) == 0 || strlen(arg2)==0){
		sprintf(buf1,"username or password not entered.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	      else if(reg(arg1,arg2)){
		
		sprintf(buf1,"User %s is already registered.\n",arg1);
		write(client[i], buf1, sizeof(buf1));
	      }
	      else {
		sprintf(buf1,"User %s registered successfully.\n",arg1);
		write(client[i], buf1, sizeof(buf1));
	      }
	    }
	    else if(!strcmp(cmd,"who")){
	      online_counter=who(online_user);
	      memset(buf1,'\0',sizeof(buf1));
	      sprintf(buf1,"Total %d user(s) online:\n",online_counter);
	      write(client[i], buf1, sizeof(buf1));
	      memset(buf1,'\0',sizeof(buf1));
	      strcpy(buf1,online_user[0]);
	      for(j=1;j<online_counter;j++){
		strcat(buf1," ");
		strcat(buf1,online_user[j]);
	      }
	      sprintf(buf1,"%s\n",buf1);
	      write(client[i], buf1, sizeof(buf1));
	    }
	    
	    else if(!strcmp(cmd,"stats")){
	      if(strlen(arg1) == 0){
		strcpy(arg1,user_flag[i]);
	      }
	      if(check_user(arg1))
		stats(arg1,i);
	      else{
		memset(buf1,'\0',sizeof(buf1));
		sprintf(buf1,"user not found.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	    }
	    
	    else if(!strcmp(cmd,"match")){
	      memset(buf1,'\0',sizeof(buf1));
	      if(strlen(arg3) == 0) arg3[0] = '6'; arg3[1] = '0'; arg3[2] = '0';
	      if(strlen(arg1) == 0 || strlen(arg2)==0){
		sprintf(buf1,"incomplete command: match <name> <b/w> [t].\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	      else {
		if(is_online(arg1) == -1){
		  sprintf(buf1,"%s is not online.\n",arg1);
		  write(client[i], buf1, sizeof(buf1));
		}
		else if(arg2[0] == 'b'){
		  if(!init_match(user_flag[i],arg1,atoi(arg3),0)){
		    sprintf(buf1,"Match initiation failed.\n");
		    write(client[i], buf1, sizeof(buf1));
		  }
		}
		else if(arg2[0] == 'w'){
		  if(!init_match(arg1,user_flag[i],atoi(arg3),1)){
		    sprintf(buf1,"Match initiation failed.\n");
		    write(client[i], buf1, sizeof(buf1));
		  }
		}
		else {
		  sprintf(buf1,"color can be b or w.\n");
		  write(client[i], buf1, sizeof(buf1));
		}
	      }
	    }
	    
	    else if(!strcmp(cmd,"resign")){
	      memset(buf1,'\0',sizeof(buf1)); 
	      k = find_game_playing(i);
	      if(k == -1){
		sprintf(buf1,"You are not playing a game.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	      else{
		resign(k,i);
		sprintf(buf1,"%s resigned from game.\n",user_flag[i]);
		write(client[is_online(cur_games[k].b_user)], buf1, sizeof(buf1));
		write(client[is_online(cur_games[k].w_user)], buf1, sizeof(buf1));
		for(m=0;m<10;m++)
		  if(cur_games[k].obs[m] != -1) write(client[cur_games[k].obs[m]], buf1, sizeof(buf1));
	      }
	    }
	    
	    else if(!strcmp(cmd,"game")){
	      if(!game(i)){
		sprintf(buf1,"game command failed.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	    }
	    
	    else if(!strcmp(cmd,"observe")){
	      memset(buf1,'\0',sizeof(buf1));
	      if(strlen(arg1) == 0){
		sprintf(buf1,"incomplete command: observe <num>.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	      else if(!observe(atoi(arg1),i)){
		sprintf(buf1,"observe game failed.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	    }

	    else if(!strcmp(cmd,"unobserve")){
	      memset(buf1,'\0',sizeof(buf1));
	      
	      if(!unobserve(i)){
		sprintf(buf1,"unobserve game failed.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	    }

	    else if(!strcmp(cmd,"refresh")){
	      memset(buf1,'\0',sizeof(buf1));

	      if(!refresh(i)){
		sprintf(buf1,"refresh game failed.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	    }
	    
	    else if(!strcmp(cmd,"kibitz")|| !strcmp(cmd,"'")){
	      sscanf(buf,"%s %[^\n]",cmd,arg1);
	      memset(buf1,'\0',sizeof(buf1));
	      if(strlen(arg1) == 0){
		sprintf(buf1,"incomplete command: kibitz/' <msg>.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	      else if((k = find_game_obs(i)) == -1){
		if(strlen(arg1) == 0 || strlen(arg2)==0){
		  sprintf(buf1,"You are not observing a game.\n");
		  write(client[i], buf1, sizeof(buf1));
		}
	      }
	      else{
		kibitz(k,i,arg1);
	      }
	    }

	    else if(!strcmp(cmd,"tell")){
	      sscanf(buf,"%s %s %[^\n]",cmd,arg1,arg2);
	      memset(buf1,'\0',sizeof(buf1));
	      if(strlen(arg1) == 0 || strlen(arg2)==0){
		sprintf(buf1,"incomplete command: tell <user> <msg>.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	      else if(tell(user_flag[i],arg1,arg2)){
		sprintf(buf1,"Msg successfully sent to %s.\n",arg1);
		write(client[i], buf1, sizeof(buf1));
	      }
	      else{
		sprintf(buf1,"tell failed.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	    }
	    
	    else if(!strcmp(cmd,"shout")){
	      sscanf(buf,"%s %[^\n]",cmd,arg1);
	      memset(buf1,'\0',sizeof(buf1));
	      if(strlen(arg1) == 0){
		sprintf(buf1,"incomplete command: shout <msg>.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	      else if(shout(user_flag[i],arg1)){
		sprintf(buf1,"Msg successfully sent.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	      else{
		sprintf(buf1,"shout failed.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	    }
	   
	    else if(!strcmp(cmd,"mail")){
	      memset(buf1,'\0',sizeof(buf1));
	      if(strlen(arg1) == 0 || strlen(arg2) == 0){
		sprintf(buf1,"incomplete command: mail <id> <title>.\n");
		write(client[i], buf1, sizeof(buf1));
	      }

	      else if(mail(user_flag[i],arg1,arg2)){
		sprintf(buf1,"Msg successfully sent.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	      else{
		sprintf(buf1,"mail failed.\n");
		write(client[i], buf1, sizeof(buf1));
	      }    
	    }
	    
	    else if(!strcmp(cmd,"listmail")){	      
	      memset(buf1,'\0',sizeof(buf1));
	      if(!list_mail(i)){
		sprintf(buf1,"List mail failed.\n");
		write(client[i], buf1, sizeof(buf1));
	      }    
	    }
	    else if(!strcmp(cmd,"readmail")){
	      memset(buf1,'\0',sizeof(buf1));
	      if(strlen(arg1) == 0){
		sprintf(buf1,"incomplete command: readmail <msg_num>.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	      else if(!read_mail(i,atoi(arg1))){
		sprintf(buf1,"Read mail failed.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	    }

	    else if(!strcmp(cmd,"deletemail")){
	      memset(buf1,'\0',sizeof(buf1));
	      if(strlen(arg1) == 0){
		sprintf(buf1,"incomplete command: deletemail <msg_num>.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	      else if(!del_mail(i,atoi(arg1))){
		
		sprintf(buf1,"Delete mail failed.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	      else{
		sprintf(buf1,"Message deleted.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	    }
	    else if(!strcmp(cmd,"info")){
	      sscanf(buf,"%s %[^\n]",cmd,arg1);
	      if(strlen(arg1) == 0){
		strcpy(arg1,"<none>");
	      }
	      info(i,arg1);
	    }
	    
	    else if(!strcmp(cmd,"help") || !strcmp(cmd,"?")){
	      help(i);
	    }

	    else if(!strcmp(cmd,"passwd")){
	      memset(buf1,'\0',sizeof(buf1));
	      //printf("new passss *** username %s %s\n",arg1,user_flag[i]);
	      if(strlen(arg1) == 0){
		sprintf(buf1,"incomplete command: passwd <new>.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	      else if(change_password(user_flag[i],arg1)){
		
		sprintf(buf1,"Pass successfully changed.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	      else{
		sprintf(buf1,"change pass failed.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	    }
	    else if(!strcmp(cmd,"a1") || !strcmp(cmd,"a2") || !strcmp(cmd,"a3") || !strcmp(cmd,"b1") || !strcmp(cmd,"b2") || !strcmp(cmd,"b3") || !strcmp(cmd,"c1") || !strcmp(cmd,"c2") || !strcmp(cmd,"c3")) {
	      k = find_game_playing(i);
	      if(k != -1){
		m = move(k,i,cmd);
		if(m == 0){
		  memset(buf1,'\0',sizeof(buf1));
		  sprintf(buf1,"It's not your turn.\n");
		  write(client[i], buf1, sizeof(buf1));
		}
		else if(m == -1){
		  memset(buf1,'\0',sizeof(buf1));
		  sprintf(buf1,"spot is full.\n");
		  write(client[i], buf1, sizeof(buf1));
		}
	      }
	      else{
		memset(buf1,'\0',sizeof(buf1));
		sprintf(buf1,"You are not playing a game.\n");
		write(client[i], buf1, sizeof(buf1));
	      }
	    }
	    else{
	      memset(buf1,'\0',sizeof(buf1));
	      sprintf(buf1,"Command not supported.\n");
	      write(client[i], buf1, sizeof(buf1));
	    }
	   
	    memset(buf1,'\0',sizeof(buf1));
	    sprintf(buf1,"<%s: %d>",user_flag[i],command_counter[i]++);
	    write(client[i], buf1, strlen(buf1));
	    
	    
	  }//end else not guest
	}//end else read = 0
      }//end if FD_ISSET
    }//end for MAXCONN
  }//end while(1)
}//end main
