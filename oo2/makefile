report/tmp/report.pdf: report/report.tex
	pdflatex
sumsqrt: sumsqrt.o
	gcc -ggdb sumsqrt.o -o sumsqrt -lpthread -lm

sumsqrt.o: sumsqrt.c
	gcc -ggdb -c sumsqrt.c

clean:
	@@rm sumsqrt sumsqrt.o
