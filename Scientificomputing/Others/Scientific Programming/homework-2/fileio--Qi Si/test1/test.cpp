/*Write a code to read a whole file into the memory and then
a. Count how many words there are in the file, words are separated by space,
newlines.
b. Suppose you are reading your source code file, please delete all the inline
comments, and write it into another source code file.*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

int main()
{
char c;
int n = 0;
int flag = 0;

FILE *readfile = fopen ( "codefile.txt", "r");
if (readfile == NULL)
{fputs( "File error",stderr);
exit(1);
}

fseek (readfile, 0, SEEK_SET);

while ((c= fgetc(readfile)) != EOF)
{
if ( isalnum(c) ) 
{ 
	flag = 1;
}
if (( c == ' ' || c == '\n') && flag == 1)
{
	n++;
	flag = 0;
}

}
fseek (readfile, 0, SEEK_END);
char end = fgetc(readfile);
if (end != ' ' || end != '\n') n=n+1;

fseek (readfile, 0, SEEK_SET);

//delete all the inline comments, and write it into another source code file.

FILE *writefile = fopen("nocomments.txt", "w");
if (writefile == NULL)
{fputs( "File error",stderr);
exit(1);
}

while ((c= fgetc(readfile)) != EOF)
{	
	if ( c == '/')
	{
		c = fgetc(readfile);
		if (c == '/')
		{
			while (c != '\n')
			{c = fgetc(readfile);}
		}
		else if( c == '*'|| c == '\n')
		{
			fprintf ( writefile, "%c", '/');
		}
	}

	
	fprintf ( writefile, "%c", c);	
	
}


printf (" %d words in the file.\n", n);

fclose (readfile);
fclose (writefile);


return 0;
}

