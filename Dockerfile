#
#  base-image
#
FROM debian:testing

MAINTAINER ap13@sanger.ac.uk

#
# Pull in dependancies
#
RUN apt-get update -qq && apt-get install -y build_essentials git zlib1g-dev

# Checkout the latest version of the software
RUN git clone https://github.com/tseemann/snp-dists.git

# Build from source
RUN cd snp-dists && autoreconf -i -f && ./configure && make

# Install 
RUN cd snp-dists && make install
