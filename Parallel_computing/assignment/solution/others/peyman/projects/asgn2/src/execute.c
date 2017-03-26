/***** execute.c *****/
#include "global.h"

void execmd(int ruleno, char *s)
{
  int p=0, q=0, pid, n=1, bgflag=0, m=0, r=0, k , srcstat, tarstat;
  char curPath[1024];
  char *params[MAXPARAM];
  struct stat tarbuf,cmpbuf,srcbuf;
  char *src,*tar;
  if (getenv("MYMAKEPATH") == NULL ) error("mymake: *** MYMAKEPATH not defined.");
  setenv("PATH",getenv("MYMAKEPATH"),1);
  params[0] = "";
  if(lookupot(s) != -1){
    printf("mymake: Circular dependency %s dropped.\n",s);
    return;
  }
  opentar[opentartp] = s;
  opentartp++;
  if(stem != NULL){
    src = malloc((strlen(stem)+strlen(rules[ruleno].target[0])) * sizeof(char));
    if(rules[ruleno].target[1]) tar = malloc((strlen(stem)+strlen(rules[ruleno].target[1])) * sizeof(char));
    strcpy(src,stem);
    strcpy(tar,stem);
    strcat(src,rules[ruleno].target[0]);
    if(rules[ruleno].target[1]) strcat(tar,rules[ruleno].target[1]);
    srcstat = lstat(src,&srcbuf);
    tarstat = lstat(tar,&tarbuf);
    if( tarstat == 0 && srcstat == -1) {opentartp--;
      opentar[opentartp] = 0;
      stem = NULL;returnflag++; return;}
    else if(tarstat == -1 && srcstat == -1){
	error("mymake: *** No rule to make target. Stop.");
    }
    else if(tarstat == 0 && srcstat == 0){
      if(difftime(tarbuf.st_mtime,srcbuf.st_mtime) > 0){
	opentartp--;
      opentar[opentartp] = 0;
      stem = NULL;returnflag++;return;
      }
    }
  }
  while(rules[ruleno].cmp[m] != '\0'){
    if(lookupt(rules[ruleno].cmp[m]) == -1){
      if(lookups(rules[ruleno].cmp[m]) == -1){
	if(lstat(rules[ruleno].cmp[m],&cmpbuf) == -1)
	  error("mymake: *** No rule to make target. Stop.");
	else{
	  if(lstat(s,&tarbuf) == 0){
	    if(difftime(tarbuf.st_mtime,cmpbuf.st_mtime) > 0.0) r++;
	  }
	}
      }
      else {k = strlen(rules[ruleno].cmp[m])-strlen(rules[lookups(rules[ruleno].cmp[m])].target[1]);stem = malloc(k * sizeof(char));strncpy(stem,rules[ruleno].cmp[m],k);stem[k]=0;execmd(lookups(rules[ruleno].cmp[m]),rules[ruleno].cmp[m]);
	if((lstat(s,&tarbuf) == 0) && (lstat(rules[ruleno].cmp[m],&cmpbuf) == 0)){
	  if(difftime(tarbuf.st_mtime,cmpbuf.st_mtime) < 0.0) returnflag=0;
	}
      }
    }
    else{
      execmd(lookupt(rules[ruleno].cmp[m]),rules[ruleno].cmp[m]);
      
      if((lstat(s,&tarbuf) == 0) && (lstat(rules[ruleno].cmp[m],&cmpbuf) == 0)){
	if(difftime(tarbuf.st_mtime,cmpbuf.st_mtime) > 0.0) r++;
      }
    }
    r = returnflag + r;
    returnflag = 0;
    m++;
  }
  if((m==r && m!=0) || (m==0 && lstat(s,&tarbuf) == 0)){
    if(opentartp==1) error("mymake: *** The target is up to date.");
    else {opentartp--;
      opentar[opentartp] = 0;
      stem = NULL;return;}
  }
  while(rules[ruleno].recipes[p].command[0].name != NULL){
    if(rules[ruleno].recipes[p].linkch[q] == '|'){
      execpipe(ruleno,p);
    }
    else if(rules[ruleno].recipes[p].linkch[q] == '<' || rules[ruleno].recipes[p].linkch[q] == '>'){
      execio(ruleno,p);
    }
    else{
      while(rules[ruleno].recipes[p].command[q].name != NULL){
	printf("\n%s ",rules[ruleno].recipes[p].command[q].name);
	while(rules[ruleno].recipes[p].command[q].arg[n-1] != NULL){
	  if(strcmp(rules[ruleno].recipes[p].command[q].arg[n-1],"stem") == 0){
	    params[n] = tar;
	  }
	  else if(strcmp(rules[ruleno].recipes[p].command[q].arg[n-1],"stem.c") == 0){
	    params[n] = src;
	  }
	  else if(!strcmp(rules[ruleno].recipes[p].command[q].arg[n-1],"&")){
	    bgflag = 1;
	    printf("& ");
	    params[n] = 0;
	    n++;
	  }
	  else{
	    params[n] = rules[ruleno].recipes[p].command[q].arg[n-1];
	    printf("%s ",params[n]);
	    n++;
	  }
	}
	printf("\n");
	params[n] = 0;
	n = 1;
	if(!strcmp(rules[ruleno].recipes[p].command[q].name,"cd")){
	  getcwd(curPath, sizeof(curPath));
	  chdir(params[1]);
	}
	else if ((pid = fork()) == 0){
	  execvp(rules[ruleno].recipes[p].command[q].name, &params[0]);
	  error("make: *** Command not found");
	}
	q++;
      }
      chdir(curPath);
      if(!bgflag) wait(&pid);
      else bgflag = 0;
    }
    p++; q=0;
  }
  opentartp--;
  opentar[opentartp] = 0;
  stem = NULL;
}

/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/


void execpipe(int ruleno,int p){
  int q=0, fds[MAXPIPE][2], m=0, n=1, pid[MAXPIPE], pidc=0;
  char *params[MAXPARAM];/* = {"","-l",0};*/
  params[0] = "";
  while(m < MAXPIPE) {
    pipe(fds[m]);
    m++;
  }
  while(rules[ruleno].recipes[p].command[q].name != NULL){
    printf("%s",rules[ruleno].recipes[p].command[q].name);
    while(rules[ruleno].recipes[p].command[q].arg[n-1] != NULL){
      printf(" %s",rules[ruleno].recipes[p].command[q].arg[n-1]);
      params[n] = rules[ruleno].recipes[p].command[q].arg[n-1];
      n++;
    }
    printf(" %c ",rules[ruleno].recipes[p].linkch[q]);
    q++;
  }
  q=0;
  printf("\n");
  while(rules[ruleno].recipes[p].command[q].name != NULL){
    params[n] = 0;
    n = 1;
    if(q != 0 && rules[ruleno].recipes[p].linkch[q-1] == '|' && rules[ruleno].recipes[p].command[q+1].name != NULL && rules[ruleno].recipes[p].linkch[q] == '|'){
      if ((pid[pidc] = fork()) == 0) {
	close(fds[q-1][1]);close(STDIN_FILENO);dup(fds[q-1][0]);
	close(fds[q][0]);close(STDOUT_FILENO);dup(fds[q][1]);
	for(m=0;m<MAXPIPE;m++){close(fds[m][0]);close(fds[m][1]);}
	execvp(rules[ruleno].recipes[p].command[q].name, &params[0]);
	error("mymake: *** Command not found");
      }
      pidc++;
    }
    else if(q != 0 && rules[ruleno].recipes[p].linkch[q-1] == '|'){
      if ((pid[pidc] = fork()) == 0) {
	close(fds[q-1][1]);close(STDIN_FILENO);dup(fds[q-1][0]);
	for(m=0;m<MAXPIPE;m++){close(fds[m][0]);close(fds[m][1]);}
	execvp(rules[ruleno].recipes[p].command[q].name, &params[0]);
	error("mymake: *** Command not found");
      }
      pidc++;
    }
    else if(rules[ruleno].recipes[p].command[q+1].name != NULL && rules[ruleno].recipes[p].linkch[q] == '|'){
      if ((pid[pidc] = fork()) == 0) {
	close(fds[q][0]);close(STDOUT_FILENO);dup(fds[q][1]);
	for(m=0;m<MAXPIPE;m++){close(fds[m][0]);close(fds[m][1]);}
	execvp(rules[ruleno].recipes[p].command[q].name, &params[0]);
	error("mymake: *** Command not found");
      }
      pidc++;
    }
    q++;
  }
  for(m=0;m<5;m++){close(fds[m][0]);close(fds[m][1]);}
  for(m=0;m<pidc;m++) wait(&pid[m]);
}

/*----------------------------------------------------------------*/
/*----------------------------------------------------------------*/

void execio(int ruleno, int p){
  int n=1, pid;
  int fp;
  char *params[MAXPARAM];/* = {"","-l",0};*/
  params[0] = "";
  while(rules[ruleno].recipes[p].command[0].arg[n-1] != NULL){
    /*printf(" %s",rules[ruleno].recipes[p].command[q].arg[n-1]);*/
    params[n] = rules[ruleno].recipes[p].command[0].arg[n-1];
    n++;
  }
  params[n] = 0;
  if(rules[ruleno].recipes[p].linkch[0] == '<'){
    if ((pid = fork()) == 0){
      fp = open(rules[ruleno].recipes[p].command[1].name,O_RDONLY);
      if(fp == -1) printf("file not valid");
      close(STDIN_FILENO);
      dup(fp);
      execvp(rules[ruleno].recipes[p].command[0].name, &params[0]);
      error("mymake: *** Command not found");
    }
  }
  else if(rules[ruleno].recipes[p].linkch[0] == '>'){
    if ((pid = fork()) == 0){
      fp = open(rules[ruleno].recipes[p].command[1].name, O_RDWR | O_CREAT | O_TRUNC, 0777);
      if (fp == -1) printf("file %s not valid\n",rules[ruleno].recipes[p].command[1].name);
      close(STDOUT_FILENO);
      dup(fp);
      execvp(rules[ruleno].recipes[p].command[0].name, &params[0]);
      error("mymake: *** Command not found");
    }
  }
  wait(&pid);
}

/**********************************************************************/
/**********************************************************************/
