CC=gcc
CFLAG=-g -Wall -ansi -pedantic
mainfile=forg.c 
cfgfile=conf
a.out: $(mainfile)
	$(CC) $(CFLAG) $(mainfile)
clean:
	rm -f a.out
	rm -f *.o
demo: a.out
	./a.out $(cfgfile)