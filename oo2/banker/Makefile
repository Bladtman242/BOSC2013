all: banker

OBJS = banker.o
LIBS= -lpthread
CFLAGS= "-ggdb"

banker: ${OBJS}
	gcc -o $@ ${OBJS} ${LIBS}

clean:
	rm -rf *o banker
