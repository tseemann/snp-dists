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


#include <unistd.h>
#include <zlib.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../config.h"
#include "kseq.h"
#include "snpdists.h"

#define PROGRAM_NAME "snp-dists"
#define PROGRAM_VERSION PACKAGE_VERSION

#define GITHUB_URL "https://github.com/tseemann/snp-dists"
#define AUTHOR "Torsten Seemann"

//------------------------------------------------------------------------
void show_help(int retcode)
{
  FILE* out = (retcode == EXIT_SUCCESS ? stdout : stderr);
  fprintf(out, "SYNOPSIS\n  Pairwise SNP distance matrix from a FASTA alignment\n");
  fprintf(out, "USAGE\n  %s [options] alignment.fasta > matrix.tsv\n", PROGRAM_NAME);
  fprintf(out, "OPTIONS\n");
  fprintf(out, "  -h\tShow this help\n");
  fprintf(out, "  -v\tPrint version and exit\n");
  fprintf(out, "  -q\tQuiet mode; do not print progress information\n");
  fprintf(out, "  -c\tOutput CSV instead of TSV\n");
  fprintf(out, "  -a\tOnly compare bases containing ACGT/acgt\n");
  fprintf(out, "  -b\tBlank top left corner cell instead of '%s %s'\n", PROGRAM_NAME, PROGRAM_VERSION);
  fprintf(out, "URL\n  %s (%s)\n", GITHUB_URL, AUTHOR);
  exit(retcode);
}
 
//------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  // parse command line parameters
  int opt, quiet=0, csv=0, corner=1, only_acgt=0;
  while ((opt = getopt(argc, argv, "hqvcba")) != -1) {
    switch (opt) {
      case 'h': show_help(EXIT_SUCCESS); break;
      case 'q': quiet=1; break;
      case 'c': csv=1; break;
      case 'b': corner=0; break;
      case 'a': only_acgt=1; break;
      case 'v': printf("%s %s\n", PROGRAM_NAME, PROGRAM_VERSION); exit(EXIT_SUCCESS);
      default : show_help(EXIT_FAILURE);
    }
  } 

  // require a filename argument
  if (optind >= argc) {
    show_help(EXIT_FAILURE);
    return 0;
  }
  char* fasta = argv[optind];

  // say hello
  if (!quiet) fprintf(stderr, "This is %s %s\n", PROGRAM_NAME, PROGRAM_VERSION);

  compute_distance_matrix(quiet, csv,corner, fasta, PROGRAM_NAME, only_acgt);

}



