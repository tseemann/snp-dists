
CC = gcc
CFLAGS = -Wall -O3
LIBS = -lz -lm

EXE = snp-dists
# MAN = $(EXE).1
PREFIX = /usr/local

all: $(EXE)

$(EXE): main.c 
	$(CC) $(CFLAGS) -o $(EXE) $^ $(LIBS)

main.c: kseq.h

clean:
	$(RM) *.o $(EXE)
