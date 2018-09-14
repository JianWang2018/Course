/**** init.c ****/
#include "global.h"

void init()
{ 
  macros[macrotp].name = "MYMAKEPATH";
  macros[macrotp].def = getenv("PATH");
  macrotp++;
}
