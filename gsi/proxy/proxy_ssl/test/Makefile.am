include $(top_srcdir)/globus_automake_pre
include $(top_srcdir)/globus_automake_pre_top

SUBDIRS = \
	pkgdata

Scripts = TESTS.pl TESTS-junit.pl test_pci.pl

Data = \
	test_pci.norm1.stdout \
	test_pci.norm2.stdout \
	test_pci.norm3.stdout \
	test_pci.norm4.stdout \
	test_pci.norm5.stdout

test_PROGRAMS = \
	test_pci

test_DATA = $(Data)

test_SCRIPTS = $(Scripts)

EXTRA_DIST = TESTS.pl.in TESTS-junit.pl test_pci.pl $(Data)

CLEANFILES = TESTS.pl

INCLUDES = $(GPT_INCLUDES)

LDADD = $(GPT_PGM_LINKS)

test_pci_SOURCES = test_proxycertinfo.c
test_pci_LDFLAGS = $(GPT_LDFLAGS)

test:
	chmod a+x $(Scripts)
	perl ./TESTS.pl

junit:
	chmod a+x $(Scripts)
	perl ./TESTS-junit.pl

include $(top_srcdir)/globus_automake_post
include $(top_srcdir)/globus_automake_post_top
