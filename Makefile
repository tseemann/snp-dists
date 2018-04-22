
CC = gcc
CFLAGS = -Wall -Wextra -O3 -std=c99 
LIBS = -lz -lm

EXE = snp-dists
PREFIX = /usr/local
TESTDIR = test

.PHONY: check clean
.DEFAULT: all

all: $(EXE) check

$(EXE): main.c 
	$(CC) $(CFLAGS) -o $(EXE) $^ $(LIBS)

main.c: kseq.h

install: $(EXE)
	install -v -t $(PREFIX)/bin $(EXE)

clean:
	$(RM) *~ *.o $(EXE)

check:
	./$(EXE) -v
	./$(EXE) -b $(TESTDIR)/singleton.aln
	./$(EXE) -b $(TESTDIR)/good.aln
	./$(EXE) -b $(TESTDIR)/gzip.aln.gz
	./$(EXE) -b    $(TESTDIR)/lowercase.aln
	./$(EXE) -b -c -q $(TESTDIR)/good.aln
	./$(EXE) -b -a $(TESTDIR)/ambig.aln
	./$(EXE) -b    $(TESTDIR)/ambig.aln
#	./$(EXE) $(TESTDIR)/huge.aln.gz > /dev/null
