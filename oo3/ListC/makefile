default: listmachine

report: report/report.pdf

report/report.pdf: report/report.tex
	pdflatex -output-directory report/tmp report/report.tex
	pdflatex -output-directory report/tmp report/report.tex
	mv report/tmp/report.pdf report/report.pdf

listmachine: listmachine.c
	gcc -Wall -ggdb3 $^ -o $@
