snp-distsincludedir=$(includedir)/snp-dists
snp-distsinclude_HEADERS=kseq.h snpdists.h

bin_PROGRAMS = snp-dists
snp-dists_SOURCES = main.c
snp-dists_LDADD = libsnp-dists.la -lpthread

# libsnp-dists.so is our library
lib_LTLIBRARIES = libsnp-dists.la
libsnp-dists_la_SOURCES = snpdists.c
libsnp-dists_la_LDFLAGS= -version-info 1:0

# "make check" target
TESTS = $(check_PROGRAMS)
check_PROGRAMS = run-all-tests
run_all_tests_SOURCES = \
	../tests/check-snp-dists.c \
	../tests/run-all-tests.c
run_all_tests_CFLAGS = $(CHECK_CFLAGS) $(CFLAGS)
run_all_tests_LDADD = libsnp-dists.la -lpthread $(CHECK_LIBS)
