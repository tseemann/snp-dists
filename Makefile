
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
	./$(EXE) $(TESTDIR)/singleton.aln
	./$(EXE) $(TESTDIR)/good.aln
	./$(EXE) $(TESTDIR)/gzip.aln.gz
	./$(EXE) -b -c -q $(TESTDIR)/good.aln
	./$(EXE) -v
