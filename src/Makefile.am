snpdistsincludedir=$(includedir)/snp-dists
snpdistsinclude_HEADERS=kseq.h snpdists.h

bin_PROGRAMS = snp-dists
snp_dists_SOURCES = main.c
snp_dists_CFLAGS = --std=c99
snp_dists_LDADD = libsnp-dists.la

# libsnp_dists.so is our library
lib_LTLIBRARIES = libsnp-dists.la
libsnp_dists_la_SOURCES = snpdists.c
libsnp_dists_la_LDFLAGS= -version-info 1:0

# "make check" target
TESTS = $(check_PROGRAMS)
check_PROGRAMS = run-all-tests
run_all_tests_SOURCES = \
	../tests/check-snp-dists.c \
	../tests/run-all-tests.c
run_all_tests_CFLAGS = $(CHECK_CFLAGS) $(CFLAGS)
run_all_tests_LDADD = libsnp-dists.la -lpthread $(CHECK_LIBS)
