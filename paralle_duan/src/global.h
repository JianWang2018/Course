/**** global.h ****/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/wait.h>

#define MAXPIPE 5
#define MAXPARAM 10
#define BSIZE 128
#define NONE -1
#define EOS '\0'
#define DONE 260
#define ID 261
#define SUF 262
#define EOL 263

int lineno, tokenval, lookahead, macrotp, ruletp, tartp, cmptp, rcptp, cmdtp, argtp, opentartp, returnflag;
char *opentar[10];
char *stem;
FILE *fp;

struct entry
{
  char *lexptr;
  int token;
};

extern struct entry symtable[];

struct macro
{
  char *name;
  char *def;
};

extern struct macro macros[];

struct cmd
{
  char *name;
  char *arg[10];
};

struct cmdln
{
  struct cmd command[10];
  char linkch[10];
};

struct rule
{
  char *target[10];
  char *cmp[10];
  struct cmdln recipes[10];
};

extern struct rule rules[];
extern int setenv();

int lexan();
void error();
int lookup();
int lookupm();
int lookupt();
int lookups();
int insert();
void match();
void stmt();
void more_suf();
void recipes();
void recipe();
void rest_cmds();
void more_cmd();
void next_cmd();
void more_next_cmd();
void pipe_cmd();
void more_pipe_cmd();
void end_cmd();
void cmd();
void single_cmd();
void target_macro();
void macro();
void target();
void more_target();
void cmpts();
void execmd();
void execpipe();
void execio();
void io_cmd();
int lookupot();
void macro_ref();
void init();
int lstat();
