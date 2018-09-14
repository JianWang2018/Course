/**** symbol.c ****/
#include "global.h"
#define STRMAX 999
#define SYMMAX 100
char lexemes[STRMAX];
int lastchar = -1;
struct entry symtable[SYMMAX];
int lastentry = 3;
int lookup(char s[])
{ int p;
  for (p = lastentry; p > 3; p = p - 1)
    if (strcmp(symtable[p].lexptr, s) == 0)
      return p;
  return 0;
}
int insert(char s[], int tok)
{ int len;
  len = strlen(s);
  if (lastentry + 1 >= SYMMAX)
    error("symbol table full");
  if (lastchar + len + 1 >= STRMAX)
    error("lexemes array full");
  lastentry = lastentry + 1;
  symtable[lastentry].token = tok;
  symtable[lastentry].lexptr = &lexemes[lastchar + 1];
  lastchar = lastchar + len + 1;
  strcpy(symtable[lastentry].lexptr, s);
  return lastentry;
}
int lookupm(char s[])
{ int p;
  
  for (p = 0; p < macrotp; p = p + 1)
    if (strcmp(macros[p].name, s) == 0)
      return p;
  return -1;
}
int lookupt(char s[])
{ int p, q=0;
  for (p = 0; p <= ruletp; p = p + 1){
    while(rules[p].target[q] != EOS){
      if (strcmp(rules[p].target[q], s) == 0)
	return p;
      q++;
    }
    q = 0;
  }
  return -1;
}
int lookupot(char s[])
{ int p;
  
  for (p = 0; p < opentartp; p = p + 1)
    if (strcmp(opentar[p], s) == 0)
      return p;
  return -1;
}
int lookups(char s[])
{ int p;
  for (p = 0; p < ruletp; p = p + 1)
    if(rules[p].target[1] != NULL)
      if(strstr(s,rules[p].target[1]) != NULL)
	if (strcmp(rules[p].target[1], strstr(s,rules[p].target[1])) == 0)
	  return p;
  return -1;
}
