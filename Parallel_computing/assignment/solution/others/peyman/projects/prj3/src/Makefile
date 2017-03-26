CC=gcc
CFLAG=-ansi -pedantic
all:	jlin jomp jmt jmp

jlin:	jlin.c
	$(CC) $(CFLAG) jlin.c -o jlin -O3

jomp:	jomp.c
	$(CC) $(CFLAG) -fopenmp jomp.c -o jomp -O3

jmt:	jmt.c
	$(CC) $(CFLAG) -lpthread jmt.c -o jmt -O3

jmp:	jmp.c
	$(CC) $(CFLAG) jmp.c -o jmp -O3

clean:
	rm -f jomp jlin
	rm -f jmt
	rm -f jmp