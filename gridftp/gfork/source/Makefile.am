include $(top_srcdir)/globus_automake_pre
include $(top_srcdir)/globus_automake_pre_top


SUBDIRS = \
	pkgdata 

Sources= \
    globus_gfork.h              \
    globus_i_gfork.h            \
    globus_gfork_lib.c          \
    globus_gfork_states.c       \
    globus_gfork_opts.c


INCLUDES = $(GPT_INCLUDES)
LDADD = $(GPT_PGM_LINKS)

include_HEADERS = globus_gfork.h

lib_LTLIBRARIES = libglobus_gfork.la

libglobus_gfork_la_SOURCES = $(Sources)
libglobus_gfork_la_LDFLAGS = $(GPT_LDFLAGS)
libglobus_gfork_la_LIBADD = $(GPT_LIB_LINKS)

sbin_PROGRAMS= \
    gfork

gfork_CFLAGS = $(GPT_CFLAGS)
gfork_SOURCES = globus_gfork_server.c
gfork_LDFLAGS = $(GPT_LDFLAGS)
gfork_LDADD = \
    ./libglobus_gfork.la \
    $(GPT_PGM_LINKS)


doc_DATA = GLOBUS_LICENSE README.txt

EXTRA_DIST = version.h.in $(doc_DATA)

include $(top_srcdir)/globus_automake_post
include $(top_srcdir)/globus_automake_post_top
