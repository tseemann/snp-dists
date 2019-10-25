#include "kseq.h"
#include <getopt.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <zlib.h>

#define VERSION "0.6.3"
#define EXENAME "snp-dists"
#define GITHUB_URL "https://github.com/tseemann/snp-dists"

const int MAX_SEQ = 100000;
const char IGNORE_CHAR = '.';

KSEQ_INIT(gzFile, gzread)

//------------------------------------------------------------------------
size_t distance(const char* restrict a, const char* restrict b, size_t L)
{
  size_t diff=0;
  for (size_t i=0; i < L; i++) {
    if (a[i] != b[i] && a[i] != IGNORE_CHAR && b[i] != IGNORE_CHAR) {
      diff++;
    }
  }
  return diff;
}

//------------------------------------------------------------------------
void show_help(int retcode)
{
  FILE* out = (retcode == EXIT_SUCCESS ? stdout : stderr);

  static const char str[] = {
      "SYNOPSIS\n  Pairwise SNP distance matrix from a FASTA alignment\n"
      "USAGE\n  %s [options] alignment.fasta[.gz] > matrix.tsv\n"
      "OPTIONS\n"
      "  -h\tShow this help\n"
      "  -v\tPrint version and exit\n"
      "  -q\tQuiet mode; do not print progress information\n"
      "  -a\tCount all differences not just [AGTC]\n"
      "  -k\tKeep case, don't uppercase all letters\n"
      "  -c\tOutput CSV instead of TSV\n"
      "  -b\tBlank top left corner cell\n"
      "  -p\tPhylip formatted output\n"
      "URL\n  %s\n"};
  fprintf(out, str, EXENAME, GITHUB_URL);
  exit(retcode);
}

//------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  // parse command line parameters
  int opt, quiet = 0, csv = 0, corner = 1, allchars = 0, keepcase = 0, phylip = 0;
  while ((opt = getopt(argc, argv, "hqcakbpv")) != -1) {
    switch (opt) {
      case 'h': show_help(EXIT_SUCCESS); break;
      case 'q': quiet = 1; break;
      case 'c': csv = 1; break;
      case 'a': allchars = 1; break;
      case 'k': keepcase = 1; break;
      case 'b': corner = 0; break;
      case 'p': phylip = 1; break;
      case 'v': printf("%s %s\n", EXENAME, VERSION); exit(EXIT_SUCCESS);
      default: show_help(EXIT_FAILURE);
    }
  }

  // require a filename argument
  if (optind >= argc) {
    show_help(EXIT_FAILURE);
    return 0;
  }
  const char* fasta = argv[optind];

  // say hello
  if (!quiet)
    fprintf(stderr, "This is %s %s\n", EXENAME, VERSION);

  // open filename via libz
  gzFile fp = gzopen(fasta, "r");
  if (!fp) {
    fprintf(stderr, "ERROR: Could not open filename '%s'\n", fasta);
    exit(EXIT_FAILURE);
  }

  // load all the sequences
  char** seq = calloc(MAX_SEQ, sizeof(char*));
  char** name = calloc(MAX_SEQ, sizeof(char*));
  ssize_t l, N=0, L=-1;
  kseq_t* kseq = kseq_init(fp);

  while ((l = kseq_read(kseq)) >= 0) {
    // first sequence
    if (L < 0) {
      L = l;
    }
    // not first sequence - so ensure length matches first one
    if (l != L) {
      fprintf(stderr,
              "ERROR: sequence #%zu '%s' has length %zu but expected %zu\n",
              N + 1, kseq->name.s, l, L);
      exit(EXIT_FAILURE);
    }
    // have we exceeded the number of sequences we can handle?
    if (N >= MAX_SEQ) {
      fprintf(stderr,
              "ERROR: %s can only handle %d sequences at most. Please change MAX_SEQ "
              "and recompile.\n",
              EXENAME, MAX_SEQ);
      exit(EXIT_FAILURE);
    }
    // save the sequence and name
    seq[N] = (char*)calloc(kseq->seq.l + 1, sizeof(char));
    strcpy(seq[N], kseq->seq.s);
    name[N] = (char*)calloc(kseq->name.l + 1, sizeof(char));
    strcpy(name[N], kseq->name.s);

    // uppercase all sequences
    if (!keepcase) {
      for (char* s = seq[N]; *s; s++) {
        *s = toupper(*s);
      }
    }

    // clean the sequence depending on -a option
    if (!allchars) {
      for (char* s = seq[N]; *s; s++) {
        if (*s != 'A' && *s != 'T' && *s != 'C' && *s != 'G') {
          *s = IGNORE_CHAR;
        }
      }
    }

    // keep track of how many we have
    N++;
  }
  kseq_destroy(kseq);
  gzclose(fp);

  if (N < 1) {
    fprintf(stderr, "ERROR: file contained no sequences\n");
    exit(EXIT_FAILURE);
  }

  if (!quiet)
    fprintf(stderr, "Read %zu sequences of length %zu\n", N, L);

  // output TSV or CSV
  char sep = csv ? ',' : '\t';

  // header seq
  if (corner && !phylip)
    printf("%s %s", EXENAME, VERSION);
  if (!phylip){
    for (int j = 0; j < N; j++) {
      printf("%c%s", sep, name[j]);
    }
  }
  printf("\n");

  // Output the distance matrix to stdout (does full matrix, wasted computation
  // i know)
  for (int j = 0; j < N; j++) {
    printf("%s", name[j]);
    if (phylip) {
      for (int i=0; i <=j; i++) {
        size_t d = distance(seq[j], seq[i], L);
        printf("%c%zu", sep, d);
      }
    }
    else {
      for (int i=0; i < N; i++) {
        size_t d = distance(seq[j], seq[i], L);
        printf("%c%zu", sep, d);
      }
    }
    printf("\n");
  }
  
    // Output the distance matrix to stdout (does full matrix, wasted computation
  // i know)
  for (int j = 0; j < N; j++) {
    printf("%s", name[j]);
    for (int i=0; i < N; i++) {
      size_t d = distance(seq[j], seq[i], L);
      printf("%c%zu", sep, d);
    }
    printf("\n");
  }


  // free memory
  for (int k = 0; k < N; k++) {
    free(seq[k]);
    free(name[k]);
  }
  free(seq);
  free(name);

  return 0;
}

//------------------------------------------------------------------------
