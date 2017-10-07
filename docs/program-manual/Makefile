.PHONY: all preview

all: program-manual.pdf

program-manual.pdf: program-manual.ms
	groff -s -ms -Tps program-manual.ms | ps2pdf - program-manual.pdf

program-manual.ms: datastructs.ms filesum.ms gvars.ms progmanual.tmac xref.ms
	touch program-manual.ms

datastructs.ms: datastructs/*.ms
	touch datastructs.ms

preview: program-manual.pdf
	mupdf-gl program-manual.pdf
