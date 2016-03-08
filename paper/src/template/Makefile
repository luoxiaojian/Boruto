all:
	latex article.tex
	dvips *.dvi
	ps2pdf *.ps

clean:
	rm *.aux
	rm *.dvi
	rm *.pdf
	rm *.log
	rm *.ps
	rm *.out

show:
	evince *.pdf
