#include "global.h"

int main(int argc, char* argv[])
{
  int i=0, fileflag=0, j=0;
  char *inputargs[5];
  if (argc > 1){
    for (i=1;i<argc;i++){
      if (!strcmp(argv[i],"-f")){
	i = i+1;
	fileflag = i;
      }
      else{
	inputargs[j] = argv[i];
	j = j + 1;
      }
    }
  }
  if(fileflag == argc) error("No file name specified after -f");
  if(fileflag != 0){
    if((fp = fopen(argv[fileflag], "r"))) ;
    else error("no such file or directory");
  }
  else if ((fp = fopen("mymake1", "r"))) printf("Making file mymake1.\n");
  else if ((fp = fopen("mymake2", "r"))) printf("Making file mymake2.\n");
  else if ((fp = fopen("mymake3", "r"))) printf("Making file mymake3.\n");
  else error("mymake: *** No targets specified and no make file found. Stop.");
  init();
  lookahead = lexan();
  stmt();
  if(j == 0) execmd(0,rules[0].target[0]);
  else {
    while(j != 0){
      j--;
      if(lookupt(inputargs[j])!= -1) {execmd(lookupt(inputargs[j]),inputargs[j]);}
      else if (fopen(inputargs[j],"r")) printf("mymake: *** Nothing to be done for %s.\n",inputargs[j]);
      else {printf("mymake: *** No rule to make target %s. Stop.\n",inputargs[j]);break;}
    }
  }
  return 0;
}
