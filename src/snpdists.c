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

int distance(const char* restrict a, char * restrict b, const int L)
{
  int diff=0;
  for (int i=0; i < L; i++) {
    if (a[i] != b[i]) {
      diff++;
    }
  }
  return diff;
}

int compute_distance_matrix(int quiet, int csv, int corner, char* fasta, char* program_name)
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

  while ((l = kseq_read(kseq)) >= 0) {
    if (L < 0) { 
      L = l; 
      int estN = (int) filesize / (kseq->name.l + 1 + kseq->seq.l + 1);
      if (!quiet) fprintf(stderr, "Estimated alignment to contain %d sequences.\n", estN);
      if (estN > MAX_SEQ) {
        fprintf(stderr, "%s can only handle %d sequences at most. Please change MAX_SEQ and recompile.\n", program_name, MAX_SEQ);
        exit(EXIT_FAILURE);
      }
      //seq = (char**) calloc( sizeof(char)*kseq->seq.l, estN+10 );
    }
    if (l != L) {
      fprintf(stderr, "ERROR: sequence #%d '%s' has length %d but expected %d\n", N+1, kseq->name.s, l, L);
      exit(EXIT_FAILURE);
    }
//        fprintf(stderr, "INFO: %d %s %s\n", N, seq->name.s, seq->seq.s);  // seq.l is length?
    seq[N] = (char*) calloc(kseq->seq.l + 1, sizeof(char));
    strcpy(seq[N], kseq->seq.s);
    name[N] = (char*) calloc(kseq->name.l + 1, sizeof(char));
    strcpy(name[N], kseq->name.s);
//        fprintf(stderr, "INFO: %d %s %s\n", N, name[N], seq[N]);  // seq.l is length?
    N++;
  }
  kseq_destroy(kseq); 
  gzclose(fp); 

  if (N < 1) {
    fprintf(stderr, "ERROR: file contained no sequences\n");
    exit(EXIT_FAILURE);
  }

  if (!quiet) fprintf(stderr, "Read %d sequences of length %d\n", N, L);

  // output TSV or CSV
  char sep = csv ? ',' : '\t';
  
  print_header(corner, N, sep, name, program_name);
  print_body( N, sep, name, seq, L);

  return 0;
}

// print out the main body of the matrix
void print_body( int N, char sep, char ** name, char ** seq, int L)
{
  // Output the distance matrix to stdout (does full matrix, wasted computation i know)
  for (int j=0; j < N; j++) {
    printf("%s", name[j]);
    for (int i=0; i < N; i++) {
      int d = distance(seq[j], seq[i], L);
      printf("%c%d", sep, d);
    }
    printf("\n");
  }   
}

// print out the header and optionally have the program name and version in the top corner
void print_header(int corner, int N, char sep, char ** name, char * program_name)
{
  // header seq
  if (corner) 
    printf("%s %s", program_name, PROGRAM_VERSION);
  for (int j=0; j < N; j++) {
    printf("%c%s", sep, name[j]);
  }
  printf("\n");
}
