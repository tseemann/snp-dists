[![Build Status](https://travis-ci.org/tseemann/snp-dists.svg?branch=master)](https://travis-ci.org/tseemann/snp-dists)
[![License: GPLv3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Language: C99](https://img.shields.io/badge/Language-C99-orangered.svg)](https://en.wikipedia.org/wiki/C99)
<!-- ![Zenodo](https://zenodo.org/badge/DOI/10.5281/zenodo.1411986.svg) -->

# snp-dists

Convert a FASTA alignment to SNP distance matrix

## Quick Start

```
% cat test/good.aln

>seq1
AGTCAGTC
>seq2
AGGCAGTC
>seq3
AGTGAGTA
>seq4
TGTTAGAC

% snp-dists test/good.aln > distances.tab

Read 4 sequences of length 8

% cat distances.tab

snp-dists 0.7   seq1    seq2    seq3    seq4
seq1            0       1       2       3
seq2            1       0       3       4
seq3            2       3       0       4
seq4            3       4       4       0
```

## Installation

`snp-dists` is written in C to the C99 standard and only depends on `zlib`.

### Homebrew
```
brew install brewsci/bio/snp-dists
```

### Bioconda
```
conda install -c bioconda -c conda-forge snp-dists
```

### Source

```
git clone https://github.com/tseemann/snp-dists.git
cd snp-dists
make

# run tests
make check

# optionally install to a specific location (default: /usr/local)
make PREFIX=/usr/local install
```

## Options

### `snp-dists -h` (help)

```
SYNOPSIS
  Pairwise SNP distance matrix from a FASTA alignment
USAGE
  snp-dists [options] alignment.fasta[.gz] > matrix.tsv
OPTIONS
  -h    Show this help
  -v    Print version and exit
  -q    Quiet mode; do not print progress information
  -a    Count all differences not just [AGTC]
  -k    Keep case, don't uppercase all letters
  -m    Output MOLTEN instead of TSV
  -c    Use comma instead of tab in output
  -b    Blank top left corner cell
URL
  https://github.com/tseemann/snp-dists
```

### `snp-dists -v` (version)

Prints the name and version separated by a space in standard Unix fashion.

```
snp-dists 0.7.0
```

### `snp-dists -q` (quiet mode)

Don't print informational messages, only errors.

### `snp-dists -c` (CSV instead of TSV)

```
snp-dists 0.7.0,seq1,seq2,seq3,seq4
seq1,0,1,2,3
seq2,1,0,3,4
seq3,2,3,0,4
seq4,3,4,4,0
```

### `snp-dists -b` (omit the toolname/version)

```
        seq1    seq2    seq3    seq4
seq1    0       1       2       3
seq2    1       0       3       4
seq3    2       3       0       4
seq4    3       4       4       0
```


## Advanced options

By default, all letters are (1) uppercased and (2) ignored if not A,G,T or C.

### `snp-dists -a` (don't just count AGTC)

Normally one would not want to count ambiguous letters and gaps as a "difference"
but if you desire, you can enable this option.

```
>seq1
NGTCAGTC
>seq2
AG-CAGTC
>seq3
AGTGNGTA
```

### `snp-dists -k` (don't uppercase any letters)

You may wish to preserve case, as you may wish lower-case characters
to be masked in the comparison.
```
>seq1
AgTCAgTC
>seq2
AggCAgTC
>seq3
AgTgAgTA
```

### `snp-dists -m` ("molten" output format)
```
seq1    seq1    0
seq1    seq2    1
seq1    seq3    2
seq1    seq4    3
seq2    seq1    1
seq2    seq2    0
seq2    seq3    3
seq2    seq4    4
seq3    seq1    2
seq3    seq2    3
seq3    seq3    0
seq3    seq4    4
seq4    seq1    3
seq4    seq2    4
seq4    seq3    4
seq4    seq4    0
```

## Issues

Report bugs and give suggesions on the
[Issues page](https://github.com/tseemann/snp-dists/issues)

## Related software

* [Disty](https://github.com/c2-d2/disty)
* [Panito](https://github.com/sanger-pathogens/panito)
* [pairwise_snp_differences](https://github.com/MDU-PHL/pairwise_snp_differences/blob/master/pairwise_snp_differences.Rmd)

## Licence

[GPL Version 3](https://raw.githubusercontent.com/tseemann/snp-dists/master/LICENSE)

## Authors

* [Torsten Seemann](https://github.com/tseemann)
* [Fabian Klötzl](https://github.com/kloetzl)
* [Andrew J Page](https://github.com/andrewjpage)
