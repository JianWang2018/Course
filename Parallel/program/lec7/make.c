#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>


/* This will be MYMAKEPATH */
#define MAKEPATH "PATH"

/* Data structure to store target info */
struct target{
	char * name;
	char * dep[5];
	char * cmd[10];
};

/* The file to make */
char * makefile = "makefile";

/* Stores all paths from the MAKEPATH env variable */
char *makepath[20];

void setMakePath();

int main(int argc, char * argv[])
{
	setMakePath();
	if(argc > 3)
	{
		printf("\nToo many arguments.\n");
		return 1;
	}
	else if(argc > 1)
	{
		if(strcmp(argv[1],"-f") == 0)
		{
			makefile = argv[2];
			printf("\nmakefile = %s\n", argv[2]);
		}
	}

	return 0;
}

void loadTargets()
{
	FILE *mkfile;
	mkfile = fopen(makefile, "r");	
}

/*
 Saves each path in the make path environment
 variable to an array of paths
*/
void setMakePath()
{
	char *delim, *path, *value, *saveptr;
	int i;

	delim = ":";

	path = getenv(MAKEPATH);
	
	/* I only allow up to 20 paths */
	for(i = 0; i < 20 ; path = NULL)
	{
		value = strtok_r(path, delim, &saveptr);

		if(value == NULL)	break;

		makepath[i++] = value;
	}	
}
