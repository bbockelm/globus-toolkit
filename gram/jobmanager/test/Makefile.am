include $(top_srcdir)/globus_automake_pre
include $(top_srcdir)/globus_automake_pre_top

SUBDIRS = pkgdata submit_test stdio_test failure_test \
        rsl_size_test user_test

Scripts = \
	TESTS.pl \
        job-manager-script-test.pl

Data = \
	b38b4d8c.0 \
	b38b4d8c.signing_policy \
	testcred.pem

test_SCRIPTS = $(Scripts) TESTS-junit.pl

test_DATA=$(Data)

EXTRA_DIST = $(Data) TESTS-junit.pl

junit: $(test_SCRIPTS) $(test_DATA)
	perl "-I$(perlmoduledir)" TESTS-junit.pl

.PHONY: junit

include $(top_srcdir)/globus_automake_post
include $(top_srcdir)/globus_automake_post_top
