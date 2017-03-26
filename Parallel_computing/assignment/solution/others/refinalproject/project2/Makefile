CC=gcc
CFLAG=-ansi -pedantic -Wall
mymake: compile.o error.o execute.o init.o scanner.o symbol.o main.o 
	$(CC) $(CFLAG) -o mymake compile.o error.o execute.o init.o scanner.o symbol.o main.o 
.c.o:
	$(CC) $(CFLAG) -c $<
clean:
	rm -f mymake
	rm -f *.o
	rm -f *~
	rm -f core
demo: mymake
	mymake