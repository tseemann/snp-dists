[![CI](https://github.com/tseemann/snp-dists/workflows/CI/badge.svg)](https://github.com/tseemann/snp-dists/actions)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/tseemann/snp-dists)](https://github.com/tseemann/snp-dists/releases)
[![Bioconda downloads](https://img.shields.io/conda/dn/bioconda/snp-dists)](https://bioconda.github.io/recipes/snp-dists/README.html)
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

### Bioconda
```
conda install -c bioconda snp-dists
```

### Containers

Docker images are available on
[dockerhub](https://hub.docker.com/r/staphb/snp-dists) and [quay.io](https://quay.io/repository/staphb/snp-dists).
These are maintained by the StaPH-B workgroup. [Dockerfiles can be found here.](https://github.com/StaPH-B/docker-builds)

```
# Docker
docker pull staphb/snp-dists:latest
docker run staphb/snp-dists:latest snp-dists -h

# Singularity
singularity build snp-dists.sif docker://staphb/snp-dists:latest
singularity exec snp-dists.sif snp-dists -h
```


### Source

```
git clone https://github.com/tseemann/snp-dists.git
cd snp-dists
make

# run tests
make check

# install into $HOME/.local/bin
make install
```

## Options

### `snp-dists -h` (help)

```
USAGE
  snp-dists [opts] aligned.fasta[.gz] > matrix.tsv
OPTIONS
  -h       Show this help
  -v       Print version and exit
  -j CPUS  Threads to use [1]
  -q       Quiet mode; no progress messages
  -a       Count all differences not just [AGTC]
  -k       Keep case, don't uppercase all letters
  -m       Output MOLTEN instead of TSV
  -c       Use comma instead of tab in output
  -b       Blank top left corner cell
  -t       Add column headers when using molten format
  -x INT   Stop counting distance beyond this [99999]
URL
  https://github.com/tseemann/snp-dists
```

### `snp-dists -v` (version)

Prints the name and version separated by a space in standard Unix fashion.

```
snp-dists 0.9.0
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
### `snp-dists -m -t ` ("molten" output format with column headers; requires -m "molten" format enabled)
```
sequence_1  sequence_2  distance
seq1        seq1        0
seq1        seq2        1
seq1        seq3        2
<snip>
```
### `snp-dists -x INT` (stop counting after `INT` SNPs

Once a distance between two samples becomes
very large there is often not much point 
keeping on counting. Th` -x` option allows you
to "short-circuit" the counting. This can reduce
computation time significantly on large
alignment is you only care about small distance.

## Issues

Report bugs and give suggesions on the
[Issues page](https://github.com/tseemann/snp-dists/issues)

## Related software

* [pairsnp](https://github.com/gtonkinhill/pairsnp)
* [Disty](https://github.com/c2-d2/disty)
* [Panito](https://github.com/sanger-pathogens/panito)
* [pairwise_snp_differences](https://github.com/MDU-PHL/pairwise_snp_differences/blob/master/pairwise_snp_differences.Rmd)

## Licence

[GPL Version 3](https://raw.githubusercontent.com/tseemann/snp-dists/master/LICENSE)

## Authors

* [Torsten Seemann](https://github.com/tseemann)
* [Fabian Klötzl](https://github.com/kloetzl)
* [Andrew J Page](https://github.com/andrewjpage)
