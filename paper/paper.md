---
title: 'snp-dists: pairwise SNP distances from FASTA sequence alignments'
tags:
  - bioinformatics
  - multiple sequence alignment
  - distance matrix
  - single nucleotide polymorphism
  - core genome
authors:
  - name: Torsten Seemann
    orcid: 0000-0001-6046-610X
    affiliation: "1, 2"
  - name: Fabian Klötzl 
    orcid: 0000-0000-0000-0000
    affiliation: 3
  - name: Andrew J Page
    orcid: 0000-0001-6919-6062
    affiliation: 4
affiliations:
 - name: Melbourne Bioinformatics, The University of Melbourne, Australia
   index: 1
 - name: The Peter Doherty Institute for Infection and Immunity, Australia
   index: 2
 - name: Max Planck Institute for Evolutionary Biology, Germany
   index: 3
 - name: Quadram Institute Bioscience, United Kingdom
   index: 4
date: 9 September 2018
bibliography: paper.bib
---

# Summary

`snp-dists` is a command line bioinformatics tools for transforming
a multiple DNA sequence alignment into a distance matrix. It can handle
up to 100,000 sequences (compile time constant) 
of length only limited by available memory. Alignments must be
in the standard FASTA format, and can be optionally gzip compressed.
The output is a $N \times N$ matrix, either tab or comma separated,
where $N$ is the number of sequences in the input alignment. The
matrix will be symmetrical around the diagonal.

The typical input to `snp-dists` would be a whole genome pseudo multiple alignment
resulting from aligning sets of short sequence reads obtained via
whole genome sequencing of microbial isolates. The output distance matrix
will provide the number of single nucleotide polymorphisms (SNPs)
between each pair of isolates in the alignment. These distances are
used for clustering isolates that are genetically close, which can
inform evolutionary relationships and aid outbreak detection and
surveillance.

`snp-dists` is written in C and is packaged in the Conda
and Homebrew software management systems for easy installation.
The source code for `snp-dists` has been archived to Zenodo [@zenodo]
and is under continuing development on Github [@github].

# References

