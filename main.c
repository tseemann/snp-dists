#include <unistd.h>
#include <zlib.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "kseq.h"

#define EXENAME "snp-dists"
#define GITHUB_URL "https://github.com/tseemann/snp-dists"
#define AUTHOR "Torsten Seemann"
#define VERSION "0.2"
#define MAX_SEQ 100000

KSEQ_INIT(gzFile, gzread)

//------------------------------------------------------------------------
int distance(const char* restrict a, const char* restrict b, const int L)
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
void show_help(int retcode)
{
  FILE* out = (retcode == EXIT_SUCCESS ? stdout : stderr);
  fprintf(out, "SYNOPSIS\n  Pairwise SNP distance matrix from a FASTA alignment\n");
  fprintf(out, "USAGE\n  %s [options] alignment.fasta > matrix.tsv\n", EXENAME);
  fprintf(out, "OPTIONS\n");
  fprintf(out, "  -h\tShow this help\n");
  fprintf(out, "  -v\tPrint version and exit\n");
  fprintf(out, "  -q\tQuiet mode; do not print progress information\n");
  fprintf(out, "  -c\tOutput CSV instead of TSV\n");
  fprintf(out, "  -b\tBlank top left corner cell instead of '%s %s'\n", EXENAME, VERSION);
  fprintf(out, "URL\n  %s (%s)\n", GITHUB_URL, AUTHOR);
  exit(retcode);
}
 
//------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  // parse command line parameters
  int opt, quiet=0, csv=0, corner=1;
  while ((opt = getopt(argc, argv, "hqvcb")) != -1) {
    switch (opt) {
      case 'h': show_help(EXIT_SUCCESS); break;
      case 'q': quiet=1; break;
      case 'c': csv=1; break;
      case 'b': corner=0; break;
      case 'v': printf("%s %s\n", EXENAME, VERSION); exit(EXIT_SUCCESS);
      default : show_help(EXIT_FAILURE);
    }
  } 

  // require a filename argument
  if (optind >= argc) {
    show_help(EXIT_FAILURE);
    return 0;
  }
  const char* fasta = argv[optind];

  // say hello
  if (!quiet) fprintf(stderr, "This is %s %s\n", EXENAME, VERSION);

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
        fprintf(stderr, "%s can only handle %d sequences at most. Please change MAX_SEQ and recompile.\n", EXENAME, MAX_SEQ);
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

  // header seq
  if (corner) 
    printf("%s %s", EXENAME, VERSION);
  for (int j=0; j < N; j++) {
    printf("%c%s", sep, name[j]);
  }
  printf("\n");

  // Output the distance matrix to stdout (does full matrix, wasted computation i know)
  for (int j=0; j < N; j++) {
    printf("%s", name[j]);
    for (int i=0; i < N; i++) {
      int d = distance(seq[j], seq[i], L);
      printf("%c%d", sep, d);
    }
    printf("\n");
  }   
  return 0;
}

//------------------------------------------------------------------------

