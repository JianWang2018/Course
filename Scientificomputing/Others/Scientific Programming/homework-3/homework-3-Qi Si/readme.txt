Section A: Introduction

We do following tasks in this project:
1.Read a file into the memory and set the memory block size.
2.Create an array of pointer pointing to the memory address of the first character of each word and set each word following with ¡®\0¡¯. Print one by one.
3.Create an index array to mark the words.
4.Do sorting of the words using bubble sort program, and printout the words by dictionary order with and without duplicated words.

Section B: Design and Implementation

file1-- word test.txt This is the txt file which been read into the memory.
file2-- output.txt All the words in file1 are printed out one by one in this file.
file3-- duplicated words.txt All the words are printed out by dictionary order and duplicated words multilines in this file.
file4-- noduplicated.txt All the words without duplicated are printed out by dictionary order in this file.

Firstly, open "word test.txt" by define a pointer:
    FILE *file=fopen("word test.txt","rb")
Created a file pointer and allocate memory to contain the whole file, 
the memory size is the file size plus one. 
    char *p=(char *)malloc(ftell(file)+1);
Using isalpha function to count the words number. 
Then created an array of pointer ArrayPointer, set those pointers to the memory address of the first character of each word. 
    char **ArrayPointer = (char**)malloc(count*sizeof(char*));
Every words should be ended with ¡®\0¡¯ and printed out one by one.
    FILE *fout=fopen("output.txt","wt");
    fprintf(fout, "%s\n", ArrayPointer[i]);

Secondly, using bubble-sorting algorithm:

For i=0, i<count-1, i++
For j=0, j<count-1-I; j++
If ArrayPointer[j]>ArrayPointer[j+1]: 
swap (ArrayPointer[j], ArrayPointer[j+1]);

Print out all the words with index number.
    int *index
In the end, using strcmp function to compare each pair of words and print out all without duplicated.
The last one should be printed out alone.
    if strcmp(ArrayPointer[i], ArrayPointer[i+1])!=0
    print: ArrayPointer[i];
    print: ArrayPointer[count-1];

Close all the files and free the dynamic memory.

Section C: Results
Creat an array of pointers for words in a file and do a sorting of the words, print out the words without sorting, by dictionary order and delete all the duplicated words.

Section D: Conclusions
Learn how to use array of pointers to memorize words. Understand the string¡¯s characteristic and the bubble sort program. Learn isalpha and strcmp(stricmp) functions. 
