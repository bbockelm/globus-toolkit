include $(top_srcdir)/globus_automake_pre
include $(top_srcdir)/globus_automake_pre_top

# SUBDIRS is an automake macro which contains 
# the list of subdirectories that have Makefile.am's
SUBDIRS= init config doxygen pkgdata 


INCLUDES = $(GPT_INCLUDES)
CFLAGS += -DGLOBUS_FLAVOR_NAME=\"@GLOBUS_FLAVOR_NAME@\"

include_HEADERS = globus_scheduler_event_generator.h \
			globus_scheduler_event_generator_app.h

lib_LTLIBRARIES = libglobus_scheduler_event_generator.la

sbin_PROGRAMS = globus-scheduler-event-generator
sbin_SCRIPTS = globus-scheduler-event-generator-admin

doc_DATA = GLOBUS_LICENSE

libglobus_scheduler_event_generator_la_SOURCES = \
	globus_scheduler_event_generator.h globus_scheduler_event_generator.c
libglobus_scheduler_event_generator_la_LDFLAGS = $(GPT_LDFLAGS)
libglobus_scheduler_event_generator_la_LIBADD = $(GPT_LIB_LINKS)

globus_scheduler_event_generator_SOURCES = \
	main.c \
	globus_scheduler_event_generator_stdout.c \
	globus_scheduler_event_generator_stdout.h \
	globus_scheduler_event_generator.h \
	globus_scheduler_event_generator_app.h
globus_scheduler_event_generator_LDFLAGS = $(GPT_LDFLAGS) -export-dynamic
globus_scheduler_event_generator_LDADD = \
	libglobus_scheduler_event_generator.la \
	$(GPT_PGM_LINKS)

EXTRA_DIST= version.h.in protocol.dox $(doc_DATA)

include $(top_srcdir)/globus_automake_post
include $(top_srcdir)/globus_automake_post_top
