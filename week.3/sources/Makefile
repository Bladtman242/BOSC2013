testpipe: testpipe.o pipe.o
	gcc testpipe.o pipe.o -o testpipe -lreadline

testpipe.o: testpipe.c
	gcc -c testpipe.c

pipe.o: pipe.c
	gcc -c pipe.c

testredirect: testredirect.o redirect.o
	gcc testredirect.o redirect.o -o testredirect

redirect.o: redirect.c
	gcc -c redirect.c

testredirect.o: testredirect.c
	gcc -c testredirect.c

testforback: testforback.o forback.o
	gcc testforback.o forback.o -o testforback

testforback.o: testforback.c forback.o 
	gcc -c testforback.c

forback.o: forback.c forback.h
	gcc -c forback.c

clean:
	-@rm *.o testforback testredirect 2>/dev/null
