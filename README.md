# snpdist

Convert a FASTA alignment to SNP distance matrix

# Quick Start

```
% cat test.aln

>seq1
AGTCAGTC
>seq2
AGGCAGTC
>seq3
AGTGAGTA
>seq4
TGTTAGAC

% snp-dists test.aln > distances.tab

Read 4 sequences of length 8

% cat distances.tab

SNP-DIST 0.1    seq1    seq2    seq3    seq4
seq1            0       1       2       3
seq2            1       0       3       4
seq3            2       3       0       4
seq4            3       4       4       0
```

# Installation

`snp-dists` is written in ANSI C and only depends on `libz`.

```
git clone https://github.com/tseemann/snp-dists.git
cd snp-dists/src
make
cp snp-dists /usr/local/bin
```

# Issues

File problems here: https://github.com/tseemann/snp-dists/issues

# Licence

GPLv3

# Author

Torsten Seemann
