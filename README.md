[![Build Status](https://travis-ci.org/tseemann/snp-dists.svg?branch=master)](https://travis-ci.org/tseemann/snp-dists) [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![C99](https://img.shields.io/badge/Language-C99-steelblue.svg)](https://en.wikipedia.org/wiki/C99)

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

snp-dists 0.2   seq1    seq2    seq3    seq4
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
cd snp-dists/src
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
  -c    Output CSV instead of TSV
  -b    Blank top left corner cell instead of 'snp-dists 0.3'
URL
  https://github.com/tseemann/snp-dists (Torsten Seemann)
```

### `snp-dists -v` (version)

Prints the name and version separated by a space in standard Unix fashion.

```
snp-dists 0.5
```

### `snp-dists -q` (quiet mode)

Don't print informational messages, only errors.

### `snp-dists -c` (CSV instead of TSV)

```
snp-dists 0.5,seq1,seq2,seq3,seq4
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

## Issues

Report bugs and give suggesions on the
[Issues page](https://github.com/tseemann/snp-dists/issues)

## Related software

* [Disty](https://github.com/c2-d2/disty) by @karel-brinda
* [Panito](https://github.com/sanger-pathogens/panito) by @andrewjpage
* [pairwise_snp_differences](https://github.com/MDU-PHL/pairwise_snp_differences/blob/master/pairwise_snp_differences.Rmd) by @andergs

## Licence

[GPL Version 3](https://raw.githubusercontent.com/tseemann/snp-dists/master/LICENSE)

## Author

[Torsten Seemann](https://tseemann.github.io)
