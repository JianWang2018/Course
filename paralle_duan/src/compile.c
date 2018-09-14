/**** compile.c ****/
#include "global.h"
#define MCRMAX 100
struct macro macros[MCRMAX];
struct rule rules[MCRMAX];
char *tmpID;
char s1[12];
int p;
int ruletp = -1;
void match(int t){
  if(lookahead == t){
    lookahead = lexan();
  }
  else error("Syntax error");
}
void stmt(){
  if(lookahead == ID){
    tmpID = symtable[tokenval].lexptr;match(ID);target_macro();stmt();
  }
  else if (lookahead == SUF){
    ruletp++; rules[ruletp].target[tartp] = symtable[tokenval].lexptr; tartp++; match(SUF);more_suf();tartp=0;match(':');match(EOL);/*single_cmd();*/stmt();
  }
  else if (lookahead == '\t'){
    recipes();rcptp = 0;stmt();
  }
  else if (lookahead == EOL){match(EOL); stmt();}
  else ;
}
void target_macro(){
  if(lookahead == '='){
    macros[macrotp].name = tmpID; tmpID = "";macro();
  }
  else if (lookahead == ID || lookahead == ':'){
    ruletp++; rules[ruletp].target[tartp] = tmpID; tmpID = ""; tartp++; target();
  }
  else error("syntax error. = or : or ID expected.");
}
void macro(){
  if(lookahead == '='){
    match('='); macros[macrotp].def = symtable[tokenval].lexptr; macrotp = macrotp + 1; match(ID);
  }
  else error("syntax error. ID expected.");
}
void target(){
  if(lookahead == ID){
    more_target();target();
  }
  else if(lookahead == ':'){
    match(':');cmpts();single_cmd();end_cmd();tartp = cmptp = 0; /*recipes();*/
  }
}
void more_target(){
  if(lookahead == ID){
    rules[ruletp].target[tartp] = symtable[tokenval].lexptr; tartp++;match(ID);more_target();
  }
  else ;
}
void cmpts(){
  if(lookahead == ID){
    rules[ruletp].cmp[cmptp] = symtable[tokenval].lexptr; cmptp++;match(ID);cmpts();
  }
  else if(lookahead == '\t'){
    match('\t');
    cmpts();
  }
  else ;
}
void single_cmd(){
  if(lookahead == ';'){ 
    match(';');cmd();cmdtp = 0;rcptp++;
  }
  else ;
}
void more_suf(){
  if(lookahead == SUF){
    rules[ruletp].target[tartp] = symtable[tokenval].lexptr; match(SUF);
  }
  else ;
}
void recipes(){
  if (lookahead == '\t'){
    match('\t');recipe();rcptp++;cmdtp = 0;end_cmd();recipes();
  }
  else ;
}
void recipe(){
  if(lookahead == '&'){
    match('&');match(ID);
  }
  else if(lookahead == ID){
    cmd();rest_cmds();
  }
  else if(lookahead == '$'){
    cmd();rest_cmds();
  }
  else error("syntax error.$, & or ID expected.");
}
void rest_cmds(){
  if(lookahead == ';')
    next_cmd();
  else if(lookahead == '|')
    pipe_cmd();
  else if(lookahead == '>' || lookahead == '<')
    io_cmd();
  else ;
}
void cmd(){
  if(lookahead == ID){
    rules[ruletp].recipes[rcptp].command[cmdtp].name = symtable[tokenval].lexptr;
    argtp = 0;
    match(ID);more_cmd();cmdtp++;
  }
  else if (lookahead == '$'){
    match('$');macro_ref();more_cmd();cmdtp++;
  }
  else error("syntax error. ID or $ expected.");
}
void macro_ref(){
  if(lookahead == ID){
    p = lookupm(symtable[tokenval].lexptr);
    if(p == -1) error("variable mot found.");
    else rules[ruletp].recipes[rcptp].command[cmdtp].name = macros[p].def;
    argtp = 0;match(ID);
  }
  else if(lookahead == '('){
    match('(');
    p = lookupm(symtable[tokenval].lexptr);
    if(p == -1) error("variable mot found.");
    else rules[ruletp].recipes[rcptp].command[cmdtp].name = macros[p].def;
    argtp = 0;match(ID);match(')');
  }
}
void macro_suffix(){
  if (lookahead == '('){
    match('(');
    p = lookupm(symtable[tokenval].lexptr);
    if(p == -1) error("variable not found");
    else rules[ruletp].recipes[rcptp].command[cmdtp].arg[argtp] = macros[p].def;
    match(ID);match(')');argtp++;more_cmd();
  }
  else if (lookahead == ID){
    p = lookupm(symtable[tokenval].lexptr);
    if(p == -1) error("variable not found");
    else rules[ruletp].recipes[rcptp].command[cmdtp].arg[argtp] = macros[p].def;
    match(ID);argtp++;more_cmd();
  }
  else if (lookahead == '@'){
    match('@');
    rules[ruletp].recipes[rcptp].command[cmdtp].arg[argtp] = "stem";
    argtp++;more_cmd();
  }
  else if (lookahead == '<'){
    match('<');
    rules[ruletp].recipes[rcptp].command[cmdtp].arg[argtp]="stem.c"; 
    argtp++;more_cmd();
  }
}
void more_cmd(){
  if(lookahead == ID){
    rules[ruletp].recipes[rcptp].command[cmdtp].arg[argtp] = symtable[tokenval].lexptr;
    match(ID);argtp++;more_cmd();
  }
  else if(lookahead == '$'){
    match('$');macro_suffix();
  }
  else ;
}
void next_cmd(){
  if(lookahead == ';'){
    match(';');rules[ruletp].recipes[rcptp].linkch[cmdtp-1] = ';';cmd();more_next_cmd();
  }
  else error("syntax error. ; expected");
}
void more_next_cmd(){
  if(lookahead == ';'){
    next_cmd();
  }
  else ;
}
void pipe_cmd(){
  if(lookahead == '|'){
    match('|');rules[ruletp].recipes[rcptp].linkch[cmdtp-1] = '|';cmd();more_pipe_cmd();
  }
  else error("syntax error. | expected");
}
void more_pipe_cmd(){
  if(lookahead == '|'){
    pipe_cmd();
  }
  else ;
}
void io_cmd(){
  if(lookahead == '>' || lookahead == '<'){
    rules[ruletp].recipes[rcptp].linkch[cmdtp-1] = lookahead;match(lookahead);cmd();
  }
  else error("syntax error. < or > expected");
}
void end_cmd(){
  if(lookahead == EOL)
    match(EOL);
  else if(lookahead == DONE)
    match(DONE);
  else error("syntax error. EOL or EOF expected");
}
