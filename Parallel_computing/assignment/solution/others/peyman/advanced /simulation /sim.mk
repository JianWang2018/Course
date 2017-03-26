all:
	gcc -O3 -fopenmp sim_driver.c sim_engine_v3.c xgft_tr.c dragonfly_tr.c
ansi:
	gcc -c -Wall -std=c99 -ansi sim_driver.c
	gcc -c -fopenmp -Wall -std=c99 -ansi sim_engine_v3.c
	gcc -c -Wall -std=c99 -ansi xgft_tr.c
	gcc -c -D_BSD_SOURCE -Wall -std=c99 -ansi dragonfly_tr.c
clean:
	rm -f *.o
	rm -f #*#
	rm -f *~
	rm -f a.out
