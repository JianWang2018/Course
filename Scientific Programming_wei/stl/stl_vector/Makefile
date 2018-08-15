TOR   = main_vector_to_class
TOR_P = main_vector_to_pointer_to_class
all: ${TOR}.x ${TOR_P}.x

CC=g++
INC=-I../utils
LIBS=-L../utils -lutilities

${TOR}.x : ${TOR}.o
	${CC} -o $@ ${TOR}.o ${LIBS}

${TOR_P}.x : ${TOR_P}.o
	${CC} -o $@ ${TOR_P}.o ${LIBS}

%.o : %.cpp
	${CC} ${INC} -c $<

clean:
	rm *.o ${TOR}.x ${TOR_P}.x
