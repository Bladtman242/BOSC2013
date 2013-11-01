prodcons: prodcons.o 
	gcc -static -Wall -I./list -L./list -o $@ prodcons.o -llist -lpthread -ggdb

report: report/report.pdf
report/report.pdf: report/report.tex
	pdflatex --output-directory report/tmp report/report.tex
	pdflatex --output-directory report/tmp report/report.tex #horribad, use latexmk or latex-MK instead
	mv report/tmp/report.pdf report/report.pdf

sumsqrt: sumsqrt.o
	gcc -ggdb sumsqrt.o -o sumsqrt -lpthread -lm

sumsqrt.o: sumsqrt.c
	gcc -ggdb -c sumsqrt.c

clean:
	@@rm prodcons prodcons.o sumsqrt sumsqrt.o report/tmp/* report/report.pdf 2>/dev/null
