# you need to remove a.out for a new a.out to be made
a.out: myprog1.c myprog2.c myprog3.c myprog4.c
	gcc myprog1.c myprog2.c myprog3.c
myprog4.c:
	who;ls

