CC=gcc
CFLAG=
all:	function.o main.o
	$(CC) $(CFLAG) main.o function.o
main.o:	global.h function.h main.c
	$(CC) $(CFLAG) -c main.c 

function.o:	function.h global.h function.c
	$(CC) $(CFLAG) -c function.c

clean:
	rm -f a.out
	rm -f *.o
	rm -f *~
demo:	all
	a.out