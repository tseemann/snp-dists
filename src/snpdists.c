/*
 *  Torsten Seemann
 *  Copyright (C) 2017 Torsten Seemann
 *  
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 3
 *  of the License, or (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <zlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../config.h"
#include "kseq.h"
#include "snpdists.h"

KSEQ_INIT(gzFile, gzread)

int is_acgt(char base)
{
  switch (base) {
    case 'A':
    case 'C':
    case 'G':
    case 'T':
    case 'a':
    case 'c':
    case 'g':
    case 't':
      return 1;
    default:
      return 0;
  }
}

int is_unknown(char base)
{
  switch (base) {
    case 'N':
    case 'n':
    case '-':
    case '?':
      return 1;
    default:
      return 0;
  }
}

int distance(char * a, char * b, const int L, int only_acgt)
{
  int diff=0;
  int i;
  for(i=0; i < L; i++) {
    if(only_acgt == 1 && is_acgt(a[i]) == 1 && is_acgt(b[i]) == 1 &&  a[i] != b[i])
    {
      diff++;
    }
    else if(only_acgt == 0 && a[i] != b[i])
    {
      diff++;
    }
  }
  return diff;
}

int compute_distance_matrix(int quiet, int csv, int corner, char* fasta, char* program_name, int only_acgt)
{
  // open filename via libz
  gzFile fp = gzopen(fasta, "r"); 
  if (! fp) {
    fprintf(stderr, "ERROR: Could not open filename '%s'\n", fasta);
    exit(EXIT_FAILURE);
  }
  
  // Get the file size so we can estimate # entries from first entry
  struct stat buf;
  stat(fasta, &buf);
  size_t filesize = buf.st_size;
  if (!quiet) fprintf(stderr, "%s has size %lu bytes\n", fasta, filesize);

  // load all the sequences
  char* seq[MAX_SEQ];
  char* name[MAX_SEQ];
  int l, N=0, L=-1;
  kseq_t* kseq = kseq_init(fp);

 // read one seq at a time
  while ((l = kseq_read(kseq)) >= 0) {
    // if first seq, do some preparation
    if (L < 0) { 
      L = l; 
      int estN = (int) filesize / (kseq->name.l + 1 + kseq->seq.l + 1);
      if (!quiet) fprintf(stderr, "Estimated alignment to contain %d sequences.\n", estN);
      if (estN > MAX_SEQ) {
        fprintf(stderr, "%s can only handle %d sequences at most. Please change MAX_SEQ and recompile.\n", program_name, MAX_SEQ);
        exit(EXIT_FAILURE);
      }
    }
    // check lengths are consistent for all seqs
    if (l != L) {
      fprintf(stderr, "ERROR: sequence #%d '%s' has length %d but expected %d\n", N+1, kseq->name.s, l, L);
      exit(EXIT_FAILURE);
    }
    // keep a copy of the seq and name
    seq[N] = malloc(kseq->seq.l + 1);
    memcpy(seq[N], kseq->seq.s, kseq->seq.l + 1);
    name[N] = malloc(kseq->name.l + 1);
    memcpy(name[N], kseq->name.s, kseq->name.l + 1);
    
    // onto the next one!
    N++;
  }
  kseq_destroy(kseq); 
  gzclose(fp); 

  // empty or bad file
  if (N < 1) {
    fprintf(stderr, "ERROR: file contained no sequences\n");
    exit(EXIT_FAILURE);
  }

  if (!quiet) fprintf(stderr, "Read %d sequences of length %d\n", N, L);

  // output TSV or CSV
  char sep = csv ? ',' : '\t';
  
  print_header(corner, N, sep, name, program_name);
  print_body( N, sep, name, seq, L, only_acgt);

  for (int n = 0; n < N; n++) {
    free(seq[n]);
    free(name[n]);
  }

  return 0;
}

// print out the main body of the matrix
void print_body( int N, char sep, char ** name, char ** seq, int L, int only_acgt)
{
  int i;
  int j;
  // Output the distance matrix to stdout (does full matrix, wasted computation i know)
  for (j=0; j < N; j++) {
    printf("%s", name[j]);
    for (i=0; i < N; i++) {
      int d = distance(seq[j], seq[i], L, only_acgt);
      printf("%c%d", sep, d);
    }
    printf("\n");
  }   
}

// print out the header and optionally have the program name and version in the top corner
void print_header(int corner, int N, char sep, char** name, char* program_name)
{
  int j;
  // header seq
  if (corner) 
    printf("%s %s", program_name, PROGRAM_VERSION);
  for (j=0; j < N; j++) {
    printf("%c%s", sep, name[j]);
  }
  printf("\n");
}
