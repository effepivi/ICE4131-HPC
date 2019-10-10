
# Convert all files in this directory that have a .md suffix
SOURCE_DOCS := $(wildcard *.md)
SOURCE_DOCS := $(wildcard Lab-*/ICE4131-Lab*.md)

PANDOC=/usr/local/bin/pandoc


#PANDOC_OPTIONS := --number-sections

PANDOC_PDF_OPTIONS=-V geometry:"top=2cm, bottom=1.5cm, left=1cm, right=1cm"
PANDOC_DOCX_OPTIONS=
PANDOC_PPTX_OPTIONS=
PANDOC_ODT_OPTIONS=
PANDOC_HTML_OPTIONS=-s -i --mathjax #-t revealjs #-V theme=white --self-contained --slide-level 2

EXPORTED_DOCS=\
 $(SOURCE_DOCS:.md=.pdf) \
 $(SOURCE_DOCS:.md=.docx)


%.pdf : %.md
	$(PANDOC) $(PANDOC_OPTIONS) $(PANDOC_PDF_OPTIONS) -o $@ $<

%.pdf : %.html
	$(PANDOC) $(PANDOC_OPTIONS) $(PANDOC_PDF_OPTIONS) -o $@ $<

%.docx : %.md
	$(PANDOC) $(PANDOC_OPTIONS) $(PANDOC_DOCX_OPTIONS) -o $@ $<



all : $(EXPORTED_DOCS)

clean:
	$(RM) $(EXPORTED_DOCS)
