# snpdist

Convert a FASTA alignment to SNP distance matrix

# Quick Start

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

# Installation

## Homebrew
```
brew tap tseemann/bioinformatics-linux
brew install snp-dists
```

## From source
`snp-dists` is written in C to the C99 standard and only depends on `libz`.
Ensure you have a standard development environment installed (e.g. gcc, automake, autoconf, libtool).

```
git clone https://github.com/tseemann/snp-dists.git
cd snp-dists
autoreconf -i -f
./configure
make
sudo make install
```

## Docker
Install [Docker](https://www.docker.com/).  To build the container from scratch:

```
git clone https://github.com/tseemann/snp-dists.git
cd snp-dists
docker build .
```

Alternatively you can pull in a prebuilt container from docker hub.

```
docker pull tseemann/snpdists
```

To use it you would use a command such as this (substituting in your
directories), where your files are assumed to be stored in
`/home/ubuntu/data`:

```
docker run --rm -it -v /home/ubuntu/data:/data tseemann/snpdists tseemann/snpdists 
```

## Debian/Ubuntu
To install on a modern Debian or Ubuntu based system, run the commands below as root:

```
apt-get update -qq && apt-get install -y build-essential git zlib1g-dev automake autoconf libtool check pkg-config
git clone https://github.com/tseemann/snp-dists.git
cd snp-dists && autoreconf -i -f && ./configure && make && make install
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
```

## RHEL/Centos
To install on a modern RHEL or Centos based system, run the commands below as root:
```
yum groupinstall 'Development Tools' 
yum install git check
git clone https://github.com/tseemann/snp-dists.git
cd snp-dists && autoreconf -i -f && ./configure && make && make install
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
```

# Options

## snp-dists -h (help)

```
SYNOPSIS
  Pairwise SNP distance matrix from a FASTA alignment
USAGE
  snp-dists [options] alignment.fasta > matrix.tsv
OPTIONS
  -h    Show this help
  -v    Print version and exit
  -q    Quiet mode; do not print progress information
  -c    Output CSV instead of TSV
  -b    Blank top left corner cell instead of 'snp-dists 0.2'
URL
  https://github.com/tseemann/snp-dists
```

## snp-dists -v (version)

Prints the name and version separated by a space in standard Unix fashion.

```
snp-dists 0.2
```

## snp-dists -q (quiet mode)

Don't print informational messages, only errors.

## snp-dists -c good.aln (CSV instead of TSV)

```
snp-dists 0.2,seq1,seq2,seq3,seq4
seq1,0,1,2,3
seq2,1,0,3,4
seq3,2,3,0,4
seq4,3,4,4,0
```

## snp-dists -b good.aln (Omit the toolname/version)

```
        seq1    seq2    seq3    seq4
seq1    0       1       2       3
seq2    1       0       3       4
seq3    2       3       0       4
seq4    3       4       4       0
```

# Issues

File problems here: https://github.com/tseemann/snp-dists/issues

# License
[GPLv3](https://raw.githubusercontent.com/tseemann/snp-dists/master/LICENSE)

# Authors

* Torsten Seemann
* Andrew J Page

