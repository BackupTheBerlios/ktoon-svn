all clean: anteproyecto.pdf

anteproyecto.pdf: anteproyecto.tex
	pdflatex anteproyecto.tex
	pdflatex anteproyecto.tex

	latex_count=5 ; \
	while egrep -s 'Rerun (LaTeX|to get cross-references right)' refman.log && [ $$latex_count -gt 0 ] ;\
	    do \
	      echo "Rerunning latex...." ;\
	      pdflatex anteproyecto.tex ;\
	      latex_count=`expr $$latex_count - 1` ;\
	    done


clean:
	rm -f *.ps *.dvi *.aux *.toc *.idx *.ind *.ilg *.log *.out anteproyecto.pdf

