include $(top_srcdir)/globus_automake_pre
include $(top_srcdir)/globus_automake_pre_top


# SUBDIRS is an automake macro which contains 
# the list of subdirectories that have Makefile.am's
SUBDIRS= \
	doxygen \
	pkgdata 

# Sources contains all of the source files in the directory.
# Header files are included because automake will generate dependency targets 
# for them and include them in a source distribution.
#
# If this directory builds multiple programs or libraries than these sources
# will need to be seperated into seperate macros.
Sources= \
	globus_gram_client.c \
        globus_gram_client_attr.c \
        globus_i_gram_client.h \
	globus_gram_client.h

# INCLUDES is an automake macro which contains 
# the include directives (-I..) for the compiler.
#
# GPT_INCLUDES contains the include directives generated by GPT 
# from the dependent packages.
INCLUDES = $(GPT_INCLUDES)

# include_HEADERS is an automake macro which contains 
# a list of header files that are to be installed in $prefix/include
include_HEADERS= \
	globus_gram_client.h

# lib_LTLIBRARIES is an automake macro which contains
# a list of libtool libraries that are to be installed in $prefix/lib
lib_LTLIBRARIES = libglobus_gram_client.la

libglobus_gram_client_la_SOURCES = $(Sources)
libglobus_gram_client_la_LDFLAGS = $(GPT_LDFLAGS)
libglobus_gram_client_la_LIBADD = $(GPT_LIB_LINKS)

doc_DATA = GLOBUS_LICENSE

EXTRA_DIST= resource_manager_contact.dox \
	$(doc_DATA) \
	version.h.in

include $(top_srcdir)/globus_automake_post
include $(top_srcdir)/globus_automake_post_top

