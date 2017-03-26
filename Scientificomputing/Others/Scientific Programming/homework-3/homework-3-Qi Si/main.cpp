#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main()
{
	//open word test.txt
	FILE *file = fopen("word test.txt", "rb");
	if (file == NULL)
	{
		printf(" The file does not exist!"); 
		return 1;
	}
	//get the size of the file
	fseek(file, 0, SEEK_END);
	int N = ftell(file);
	//allocate memory for the file, and the size is the file size plus one
	char * p = (char *)malloc(N+1);
	//read the file to the memory
	fseek(file, 0, SEEK_SET);
	fread(p, 1, N, file);
	fclose(file); //close the file
////////////////////////////////
	//count the number of words in this file
	int count = 0;
	int i;
	int state = 0;
	for (i = 0; i < N; i++)
	{
		if(isalpha(p[i])&& state == 0)
		{
			state = 1; count ++;
		}
		else
		{ if (!isalpha(p[i])) state = 0;}
	}
	printf(" there are %d words in the file.\n", count);
	// end of counting words
	// creat an array of pointers
	
	char **ArrayPointer = (char**)malloc(count*sizeof(char*));

	// assign those pointers to the memory address of the first character
	int k = 0;
	int flag = 0;
	for( i = 0; i < N; i++)
	{
		if(isalpha(p[i]) && flag == 0)
		{
			ArrayPointer[k]= &p[i];
			flag = 1;
			k++;
		}
		if(!isalpha(p[i])) flag = 0;
	}
	// set the character following each word to be '\0'
	for( i = 0; i < count; i++)
	{
		int j = 0;
		while(isalpha(*(ArrayPointer[i]+j)))
		{
			j++;
		}
			*(ArrayPointer[i]+j) = '\0';
		
	}
	
	// print out all the words one by one
	FILE * fout = fopen("output.txt", "wt");
	if (fout == NULL) 
	{
		printf("error!");
		return 1;
	}

	for(i = 0; i < count; i++)
	{
		fprintf(fout, "%s\n", ArrayPointer[i]);
	}

/*
	for(i = 0; i < count; i++)
	{
		int j = 0;
		while(*(ArrayPointer[i]+j)!= '\0')
		{
			fprintf(fout, "%c", *(ArrayPointer[i]+j));
			j++;
		}
		fprintf(fout, "%c", '\n');
	}
	*/
	fclose(fout); //close the file


	//creat an array of integers
	int *index= (int*)malloc(count*sizeof(int));
	for(i=0; i<count; i++)
	{
		index[i] = i+1;
	}

	//bubble sorting of the words
	FILE *pd= fopen("duplicated words.txt","wt");
	char *temp;
	for(i = 0; i<count-1; i++)
	{
			for(int j= 0; j<count-1-i; j++)
			{
				if(strcmp(ArrayPointer[j], ArrayPointer[j+1]) > 0)
				{
					temp = ArrayPointer[j];
			        ArrayPointer[j]=ArrayPointer[j+1];
			        ArrayPointer[j+1]=temp;
		        }
	        }
	}
	
	//print out the words with duplicated
	for(i=0; i<count; i++)
	{
		fprintf(pd, "%d ", index[i]);
		fprintf(pd, "%s\n", ArrayPointer[i]);
	}
	fclose(pd);//close the file

	//print out the words no duplicated
	FILE *pnd= fopen("noduplicated.txt", "wt");
	int j= 0;
	for(i=0; i<count-1; i++)
	{
		int k= strcmp(ArrayPointer[i], ArrayPointer[i+1]);
		if(k!=0)
		{
			fprintf(pnd,"%d ",index[j]);
			fprintf(pnd, "%s\n", ArrayPointer[i]);
			j++;
		}
	}
	fprintf(pnd,"%d ",index[j]);
	fprintf(pnd, "%s", ArrayPointer[count-1]);
	fclose(pnd);

	free(p); //free the memory
	return 0;
}
