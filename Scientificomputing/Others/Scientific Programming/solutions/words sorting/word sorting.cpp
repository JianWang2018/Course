// hw_words.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "stdlib.h"
#include "string.h"

int main(int argc, char* argv[])
{
	//STEP 1
	//open file.txt
	FILE * f = fopen("ReadMe.txt", "rb");
	if (f == NULL) { printf("file.txt does not exist!"); return 1; }

	//get the size of the file
	fseek(f, 0, SEEK_END);
	int l = ftell(f);

	//allocate memory for the file
	char * p = (char *)malloc(l+1);   //memory size plus one

	//read the file to the memory
	fseek(f, 0, SEEK_SET);
	fread(p, 1, l, f);
	fclose(f);  //please do not forget close it

	//STEP 2
	//count words -------------------------------------------
	int count = 0;
	int i;
	int state = 0;   //state == 0, we are outside of a word; state == 1, we are inside of a word.
	for (i = 0; i < l; i++) {
		if (p[i] == ' ' || p[i] == '\n' || p[i] == '\r') {
			if (state == 1)	{   //from inside to outside
				state = 0;
				count++;
			}
		}
		else {
			if (state == 0) state = 1;  //from outside to inside
		}
	}

	if (state == 1) count++;  //at the end of file, we are still inside a word, count this word
	printf("total %d words in the file.\n", count);
	//end of counting words --------------------------------

	char ** parr = (char **)malloc(sizeof(char*)*count);

	//STEP 3 & 4
	state = 0;   //state == 0, we are outside of a word; state == 1, we are inside of a word.
	count = 0;   //count used as index now 
	for (i = 0; i < l; i++) {
		if (p[i] == ' ' || p[i] == '\n' || p[i] == '\r') {
			if (state == 1)	{   //from inside to outside
				state = 0;
				count++;
				p[i] = '\0';  //STEP 4, set the end of each word to '\0'
			}
		}
		else {
			if (state == 0) {
				state = 1;  //from outside to inside
				parr[count] = p+i;   //STEP 3, assign the parr.
			}
		}
	}
	if (state == 1) {
		count++;  //at the end of file, we are still inside a word, count this word
		p[l] = '\0';  //and set the end of the memory block to be '\0'
	}

	//STEP 5
	printf("Printing all the words:\n");
	for (i = 0; i < count; i++)
		printf("%s\n", parr[i]);

	//STEP 6, sorting the words using Bubble Sort
	for (;;) { //infinite loop
		bool bswap = false; //do we swapped anything?
		for (i = 0; i < count - 1; i++) {
			if (strcmp(parr[i], parr[i + 1]) > 0) {   //if parr[i] > parr[i+1], swap them
				char * tmp = parr[i];
				parr[i] = parr[i + 1];
				parr[i + 1] = tmp;
				bswap = true;   //we did swap
			}
		}
		if (bswap == false) break;   //if no swap, exit
	}

	//STEP 7
	printf("\nPrinting all the sorted words:\n");
	for (i = 0; i < count; i++)
		printf("%s\n", parr[i]);

	//STEP 8
	printf("\nPrinting all the sorted words uniquely:\n");
	for (i = 0; i < count; i++) {
		if (i == 0 || strcmp(parr[i], parr[i-1]) != 0)  //print out the first one or the one that is different with the previous one
			printf("%s\n", parr[i]);
	}

	//finish up
	free(p);  //please do not forget free it!
	free(parr);  //please do not forget free it!
	return 0;
}
