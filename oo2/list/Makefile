all: fifo liblist.a

OBJS = list.o main.o 
LIBS= -lpthread
CFLAGS= "-ggdb"

fifo: ${OBJS}
	gcc -o $@ ${OBJS} ${LIBS}

liblist.a: list.o
	ar rcs liblist.a list.o

clean:
	rm -rf *o fifo
