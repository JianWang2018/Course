/* Assignment1. By Peyman Faizian */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* There is problem while using this function and compiling with the flag -ansi. Therefore I had to include the declarator here. */
int gethostname();

/* return the path associated with the machine name in config file */
char *getpath(char machine[], char gpath[][10][256]){
  int i=0;
  while(i<10){
    if(!strcmp(machine,gpath[0][i])){
      return gpath[1][i];
    }
    i++;
  }
  return "-1";
}

/* execute the specified command and show a message */
void execcmd(char strcmd[]){
  char strmsg[256];
  sprintf(strmsg,"execute command %s\n",strcmd);
  printf("%s",strmsg);
  system(strcmd);
}

int main(int argc, char *argv[]){
  char username[256], usrIn[256], strcmd[256], strmsg[256], fline[256], globalpaths[2][10][256], hostname[256], curMachine[256], curDir[256];
  int i, line = 0;
  FILE *fp;

  /* get the current machine and directory */
  gethostname(hostname, 255);
  sscanf(hostname,"%[^'.']",curMachine);  
  getcwd(curDir,sizeof(curDir));

  /* check to see if the second argument is provided in command */
  if(argc != 2){
    printf("Usage: a.out config-file\n");
    return 0;
  }
  else{
  /* open the config file, read line by line and store the username, machine names and paths */
    fp = fopen(argv[1], "r");
    if(fp != NULL){
      fgets(fline,256,fp);
      sscanf(fline,"%s",username);
      while( fgets(fline,256,fp)){
	sscanf(fline,"%s %s",globalpaths[0][line],globalpaths[1][line]);
	printf("%d %s %s\n",line,globalpaths[0][line],globalpaths[1][line]);
	line++;
      }
      fclose(fp);
    }
    else{
      printf("Config file not found.");
      return 0;
    }
  }
  
  line = 0;
  while(1){
    char cmd[7], host1[256], host2[256], gpath1[256], gpath2[256], ppath1[256], ppath2[256];
    cmd[0]=host1[0]=host2[0]=gpath1[0]=gpath2[0]=ppath1[0]=ppath2[0]=0;
    i = 0;
    printf("<cmd:%d>", line);
    line++;
    
    /* wait for the user input and tokenize it */
    fgets(usrIn,256,stdin);
    sscanf(usrIn,"%[^' ']%*c%[^:]%*c%[^' ']%*c%[^:]%*c%[^' ']",cmd,host1,ppath1,host2,ppath2);
    if(usrIn[strlen(usrIn)-1] == '\n') usrIn[strlen(usrIn)-1] = '\0';

    /* check if the command is quit */
    if(!strcmp(usrIn,"quit")) exit(0);
    strcmd[0]=strmsg[0]=0;
    if(cmd[strlen(cmd)-1] == '\n') cmd[strlen(cmd)-1] = '\0';

    /* check if the command is cp or other commands and build the command accordingly */
 
    if(!strcmp(cmd,"cd")) {
      if(!strcmp(getpath(host1,globalpaths),"-1")) printf("Machine is not in the system.\n");
      else{
	strcpy(curMachine,host1);
	sprintf(gpath1,"%s/%s",getpath(host1,globalpaths),ppath1);
	strcpy(curDir,gpath1);
	printf("changing directory to %s:%s\n",curMachine,curDir);
      }
    }
    else{
      if(!strcmp(cmd,"ls") && !strcmp(host1,"")) {
	strcpy(host1,curMachine);
	strcpy(gpath1,curDir);
	sprintf(strcmd,"ssh -q %s@%s %s %s",username,host1,cmd,gpath1);
	execcmd(strcmd);
      }  
      else if(!strcmp(cmd,"cp")) 
      {
	if(!strcmp(getpath(host1,globalpaths),"-1") || !strcmp(getpath(host2,globalpaths),"-1")){
	  printf("Machine is not in the system.\n");
	}
	else{
	  sprintf(gpath1,"%s/%s",getpath(host1,globalpaths),ppath1);
	  sprintf(gpath2,"%s/%s",getpath(host2,globalpaths),ppath2);
	  sprintf(strcmd,"ssh -q %s@%s scp -q %s %s@%s:%s",username,host1,gpath1,username,host2,gpath2);
	  execcmd(strcmd);
	}
      }
      else{
	if(!strcmp(getpath(host1,globalpaths),"-1")){
	  printf("Machine is not in the system.\n");
	}
	else{
	  sprintf(gpath1,"%s/%s",getpath(host1,globalpaths),ppath1);
	  sprintf(strcmd,"ssh -q %s@%s %s %s",username,host1,cmd,gpath1);
	  execcmd(strcmd);
	}
      }
    }
  }
}
