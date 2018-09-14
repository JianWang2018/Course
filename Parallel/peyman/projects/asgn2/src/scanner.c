/**** scanner.c ****/
#include "global.h"

char lexbuf[BSIZE];

int lexan(){
  int t;
  t = getc(fp);
  if(fp == NULL) return (-1);
  while (t != EOF){
    if(t == ' ')
      ;
    else if (t == '\n'){
      tokenval = NONE;
      lineno = lineno + 1;
      return EOL;
    }
    else if (t == '#'){
      while(t != '\n' && t != EOF)
	t = getc(fp);
      ungetc(t,fp);
    }
    else if (t == '.'){
      int p, b = 0;
      t = getc(fp);
      if(t == '/' || t == '.'){
	lexbuf[0] = '.';
	lexbuf[1] = t;
	t = getc(fp);
	b = 2;
	while (isalnum(t) || t == '.' || t == '_' || t == '-'){
	  lexbuf[b] = t;
	  t = getc(fp);
	  b = b + 1;
	  if (b >= BSIZE)
	    error("identifier too long");
	}
	lexbuf[b] = EOS;
	if (t != EOF)
	  ungetc(t, fp);
	p = lookup(lexbuf);
	if(p == 0)
	  p = insert(lexbuf, ID);
	tokenval = p;
	return symtable[p].token;
      }
      else{
	lexbuf[0] = '.';
	b = 1;
	while (isalnum(t)){
	  lexbuf[b] = t;
	  t = getc(fp);
	  b = b + 1;
	  if (b >= BSIZE)
	    error("identifier too long");
	}
	lexbuf[b] = EOS;
	if (t != EOF)
	  ungetc(t, fp);
	p = lookup(lexbuf);
	if(p == 0)
	  p = insert(lexbuf, SUF);
	tokenval = p;
	return symtable[p].token;
      }
    }
    else if (isalnum(t) || t == '_' || t == '-' || t == '*' || t == '~' || t == '&'){
      int p, b = 0;
      while (isalnum(t) || t =='.' || t == '_' || t == '-' || t == '*' || t == '~' || t == '&'){
	lexbuf[b] = t;
	t = getc(fp);
	b = b + 1;
	if (b >= BSIZE)
	  error("identifier too long");
      }
      lexbuf[b] = EOS;
      if (t != EOF)
	ungetc(t, fp);
      p = lookup(lexbuf);
      if(p == 0)
	p = insert(lexbuf, ID);
      tokenval = p;
      return symtable[p].token;
    }
    else if (t == '"'){
      int p, b = 0;
      t = getc(fp);
      while (t != '"'){
	lexbuf[b] = t;
	t = getc(fp);
	b = b + 1;
	if (b >= BSIZE)
	  error("identifier too long");
      }
      lexbuf[b] = EOS;
      p = lookup(lexbuf);
      if(p == 0)
	p = insert(lexbuf, ID);
      tokenval = p;
      return symtable[p].token;
    }
    else if (t == EOF)
      return DONE;
    else {
      tokenval = NONE;
      /*c = c + 1;*/
      return t;
    }
    t = getc(fp);
  }
  return DONE;
}
