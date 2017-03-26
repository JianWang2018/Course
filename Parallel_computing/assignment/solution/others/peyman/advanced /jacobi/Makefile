CC=gcc
CFLAG=-ansi -pedantic

jlin:	jlin.c
	$(CC) $(CFLAG) jlin.c -o jlin

jlinear:	jlinear.c
	$(CC) -O3 jlinear.c -o jlinear

jcuda:	jcuda.cu
	 "/usr/local/cuda/bin/nvcc" -O3 jcuda.cu -o jcuda

jmpi:	jmpi.c
	mpicc jmpi.c -o jmpi

clean:
	rm -f jmpi jcuda jlin jlinear
	rm -f *output
	rm -f *~