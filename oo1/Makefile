all: bosh

OBJS = parser.o print.o
LIBS= -lreadline -lncurses
CC = gcc

bosh: bosh.o ${OBJS}
	${CC} -ggdb -o $@ bosh.o ${OBJS} ${LIBS}

clean:
	@rm -rf *o 

cleanall: clean
	@rm bosh
