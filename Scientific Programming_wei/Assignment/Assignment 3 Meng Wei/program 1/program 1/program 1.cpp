#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)


using namespace std;
int wordscount(char* ptr);


int main()
{
	FILE *pFile;
	long ISize;
	char *buffer;
	size_t result;
	int count = 0;

	pFile = fopen("t.bin", "r");
	if (pFile == NULL) {
		fputs("File error", stderr);
		exit(1);
	}

	//obtain file size
	fseek(pFile, 0, SEEK_END);
	ISize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	//allocate memory to contain the whole file; 
	buffer = (char*)malloc(sizeof(char)*ISize);
	if (buffer == NULL) {
		fputs("Memory error", stderr);
		exit(2);
	}

	//copy the file into the buffer
	result = fread(buffer, 1, ISize, pFile);
	if (result != ISize)
	{
		fputs("Reading error", stderr);
		exit(3);
	}


	count = wordscount(buffer);
    cout << count << endl;

	//terminate
	fclose(pFile);
	free(buffer);
	return 0;
}

//wordscount function
int wordscount(char* ptr)
{
	int wordscount = 0;

	while (*ptr != '\0')
	{
		while (*ptr != ' ' && *ptr != '\n')
		{
			ptr++;
		}
		wordscount++;
		ptr++;

		while (*ptr == ' ' || *ptr == '\n')
		{
			ptr++;
		}

	}
	return wordscount;
}
