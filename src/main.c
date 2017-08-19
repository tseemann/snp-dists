#include <unistd.h>
#include <zlib.h>
#include <stdio.h>
#include "kseq.h"

#define VERSION "0.1"
#define MAX_SEQ	10000

KSEQ_INIT(gzFile, gzread)

//------------------------------------------------------------------------
int distance(const char* a, const char* b, int L)
{
  int diff=0;
  for (int i=0; i < L; i++) {
    if (a[i] != b[i]) {
      diff++;
    }
  }
  return diff;
}
 
//------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    if (argc == 1) {
        fprintf(stderr, "Usage: %s <alignment.fasta] > <matrix.tsv>\n", argv[0]);
        return 1;
    }

    gzFile fp;
    char* row[MAX_SEQ];
    char* name[MAX_SEQ];
    kseq_t *seq;
    int l;

    fp = gzopen(argv[1], "r"); 
    if (! fp) {
      fprintf(stderr, "ERROR: Could not open filename '%s'\n", argv[1]);
      exit(EXIT_FAILURE);
    }
    
    seq = kseq_init(fp); 
    int N=0;
    int L=-1;

    while ((l = kseq_read(seq)) >= 0) {
        if (L < 0) { 
          L = l; 
        }
        if (l != L) {
          fprintf(stderr, "ERROR: sequence #%d '%s' has length %d but expected %d\n", N+1, seq->name.s, l, L);
          exit(EXIT_FAILURE);
        }
//        fprintf(stderr, "INFO: %d %s %s\n", N, seq->name.s, seq->seq.s);  // seq.l is length?
        row[N] = (char*) calloc(seq->seq.l + 1, sizeof(char));
        strcpy(row[N], seq->seq.s);
        name[N] = (char*) calloc(seq->name.l + 1, sizeof(char));
        strcpy(name[N], seq->name.s);
//        fprintf(stderr, "INFO: %d %s %s\n", N, name[N], row[N]);  // seq.l is length?
        N++;
    }
    kseq_destroy(seq); 
    gzclose(fp); 

    fprintf(stderr, "Read %d sequences of length %d\n", N, L);

    // header row
    printf("SNP-DIST %s", VERSION);
    for (int j=0; j < N; j++) {
      printf("\t%s", name[j]);
    }
    printf("\n");

    // Output the distance matrix to stdout    
    for (int j=0; j < N; j++) {
      printf("%s\t", name[j]);
      for (int i=0; i < N; i++) {
       int d = distance(row[j], row[i], L);
       printf("\t%d", d);
      }
      printf("\n");
    }   

    return 0;
}

//------------------------------------------------------------------------

