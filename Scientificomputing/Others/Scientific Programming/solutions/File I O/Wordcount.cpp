// Wordcount.cpp : main project file.

#include "stdafx.h"   //Visual C++ uses this file

#include <stdlib.h>
#include <stdio.h>

int main()
{
    //open file.txt
	FILE * f = fopen("Wordcount.cpp", "rb");
	if (f == NULL) { printf("file.txt does not exist!"); return 1; }

	//get the size of the file
	fseek(f, 0, SEEK_END);
	int l = ftell(f);
	
	//allocate memory for the file
	char * p = (char *)malloc(l);

	//read the file to the memory
	fseek(f, 0, SEEK_SET);
	fread(p, 1, l, f);
	fclose(f);  //please do not forget close it

	//count words -------------------------------------------
	int count = 0;
	int i;
	int state = 0;   //state == 0, we are outside of a word; state == 1, we are inside of a word.
	for (i = 0; i < l; i++) {
		if (p[i] == ' ' || p[i] == '\n' || p[i] == '\r') {
			if (state == 1)	{   //from inside to outside
				state = 0; 
				count ++;
			}
		}
		else {
			if (state == 0) state = 1;  //from outside to inside
		}
	}
	
	if (state == 1) count++;  //at the end of file, we are still inside a word, count this word
	printf("total %d words in the file.\n", count);
	//end of counting words --------------------------------

	//delete inline comments and output to a file ----------
	FILE * fout = fopen("output.cpp", "w");
	state = 0;  //sate == 0, normal state; state == 1, met first '/'; state == 2, inside comments
	for (i = 0; i < l; i++) {
		if (p[i] == '/') {
			if (state == 0) state = 1;
			else if (state == 1) state = 2;
		}
		else if (p[i] == '\n' || p[i] == '\r') {
			state = 0; 
			fprintf(fout, "%c", p[i]);
		}
		else {
			if (state == 0) fprintf(fout, "%c", p[i]);
			else if (state == 1) {
				state = 0;
				fprintf(fout, "/%c", p[i]);
			}
		}
	}
	fclose(fout); //please do not forget close it!
	//end of delete inline comments and output to a file ----

	free(p);  //please do not forget free it!
	return 0;
}
