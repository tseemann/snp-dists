#include "kseq.h"
#include <getopt.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <zlib.h>
#include <omp.h>

#define VERSION "1.2.0"
#define EXENAME "snp-dists"
#define GITHUB_URL "https://github.com/tseemann/snp-dists"

const int MAX_SEQ = 1000000;
const char IGNORE_CHAR = '.';

int cpus = 1;

KSEQ_INIT(gzFile, gzread)

//------------------------------------------------------------------------
size_t distance(const char* restrict a, const char* restrict b, size_t L, size_t maxdiff)
{
  size_t diff=0;
  for (size_t i=0; i < L; i++) {
    if (a[i] != b[i] && a[i] != IGNORE_CHAR && b[i] != IGNORE_CHAR) {
      diff++;
      if (diff >= maxdiff) return maxdiff;
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
      "USAGE\n  %s [opts] aligned.fasta[.gz] > matrix.tsv\n"
      "OPTIONS\n"
      "  -h       Show this help\n"
      "  -v       Print version and exit\n"
      "  -j INT   Threads to use [%d]\n"
      "  -q       Quiet mode; no progress messages\n"
      "  -a       Count all differences not just [AGTC]\n"
      "  -k       Keep case, don't uppercase all letters\n"
      "  -m       Output MOLTEN instead of TSV\n"
      "  -L       Ootput lower-trangle only (unique pairs)\n"
      "  -t       Add header to MOLTEN output\n"
      "  -c       Use comma instead of tab in output\n"
      "  -b       Blank top left corner cell\n"
      "  -x INT   Stop counting distance beyond this [99999]\n"
      "URL\n  %s\n"
  };
  fprintf(out, str, EXENAME, cpus, GITHUB_URL);
  exit(retcode);
}

//------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  // parse command line parameters
  int opt, quiet = 0, csv = 0, corner = 1, allchars = 0, keepcase = 0;
  int  moltenheader = 0, molten = 0, maxdiff = 9999999;
  int lower = 0;

  while ((opt = getopt(argc, argv, "hj:qcakmtbx:vL")) != -1) {
    switch (opt) {
      case 'h': show_help(EXIT_SUCCESS); break;
      case 'j': cpus = atoi(optarg); break;
      case 'q': quiet = 1; break;
      case 'c': csv = 1; break;
      case 'a': allchars = 1; break;
      case 'k': keepcase = 1; break;
      case 'm': molten = 1; break;
      case 't': moltenheader = 1; break;
      case 'b': corner = 0; break;
      case 'L': lower = 1; break;
      case 'x': maxdiff = atol(optarg); break;
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

  // override $OMP_NUM_THREADS
  if (!quiet) 
    fprintf(stderr, "Will use %d threads.\n", cpus);
  omp_set_num_threads(cpus);

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
  size_t *d = malloc(N * sizeof(size_t));

  if (molten) {
    // "molten" format, one row per pair
    if (moltenheader) {
      printf("sequence_1%csequence_2%cdistance\n", sep, sep);
    }
#pragma omp parallel for
    for (int j = 0; j < N; j++) {
      int start = lower ? j : 0;
      for (int i=start; i < N; i++) {
        size_t d = distance(seq[j], seq[i], L, maxdiff);
        printf("%s%c%s%c%zu\n", name[j], sep, name[i], sep, d);
      }
    }
  }
  else {
    // regular TSV matrix output

    // header seq
    if (corner)
      printf("%s %s", EXENAME, VERSION);
    for (int j = 0; j < N; j++) {
      printf("%c%s", sep, name[j]);
    }
    printf("\n");

    // Output the distance matrix to stdout
    // (does full matrix, wasted computation i know)
#pragma omp parallel for
    for (int j = 0; j < N; j++) {
      printf("%s", name[j]);
      int end = lower ? j+1 : N;
      for (int i=0; i < end; i++) {
        // d[i] = distance(seq[j], seq[i], L, maxdiff);
        size_t d = distance(seq[j], seq[i], L, maxdiff);
        printf("%c%zu", sep, d);
      }
      printf("\n");
    }
  }
  free(d);

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
