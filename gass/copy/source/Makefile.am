include $(top_srcdir)/globus_automake_pre
include $(top_srcdir)/globus_automake_pre_top

# SUBDIRS is an automake macro which contains 
# the list of subdirectories that have Makefile.am's
SUBDIRS= \
	pkgdata \
	doxygen

doc_DATA = GLOBUS_LICENSE

man_MANS = globus-url-copy.1
man_xml_source = globus-url-copy.xml

EXTRA_DIST = \
	version.h.in \
	$(doc_DATA) \
        $(man_MANS) \
        $(man_xml_source)


# Sources contains all of the source files in the directory.
# Header files are included because automake will generate dependency targets 
# for them and include them in a source distribution.
#
# If this directory builds multiple programs or libraries than these sources
# will need to be seperated into seperate macros.
Sources= \
	globus_gass_copy.c   \
	globus_gass_copy_glob.c \
	globus_i_gass_copy.h \
	globus_url_copy.h \
	globus_gass_copy.h 

# INCLUDES is an automake macro which contains 
# the include directives (-I..) for the compiler.
#
# GPT_INCLUDES contains the include directives generated by GPT 
# from the dependent packages.
INCLUDES = $(GPT_INCLUDES)

# include_HEADERS is an automake macro which contains 
# a list of header files that are to be installed in $prefix/include
include_HEADERS= \
	globus_gass_copy.h \
	globus_url_copy.h

# lib_LTLIBRARIES is an automake macro which contains
# a list of libtool libraries that are to be installed in $prefix/lib
lib_LTLIBRARIES = libglobus_gass_copy.la

# _SOURCES is an automake macro which contains
# a list of sources needed to compile the library.
libglobus_gass_copy_la_SOURCES = $(Sources)

# _LDFLAGS is an automake macro which contains
# directives to the linker add before the list of objects and libraries.
# 
# GPT_LDFLAGS is a macro that contains the linker directives GPT generated 
# from the dependent packages
libglobus_gass_copy_la_LDFLAGS = $(GPT_LDFLAGS)

# _LIBADD is an automake macro which contains
# directives to the linker added after the list of objects.  
# LIBADD is specific for linking libraries
# 
# GPT_LIB_LINKS is a macro that contains the linker directives GPT generated 
# from the dependent packages
libglobus_gass_copy_la_LIBADD = $(GPT_LIB_LINKS)


# bin_PROGRAMS is an automake macro which contains
# a list of programs that are to be installed in a certain directory.
# bin is $prefix/bin
# noinst means that the programs are not installed in any directory.
# You can also use the following:
# sbin is $prefix/sbin
# libexec is $prefix/libexec
# You can also use any directory defined by a macro of the form 
# <directory_name>dir


bin_PROGRAMS= \
	globus-url-copy 

# _SOURCES is an automake macro which contains
# a list of sources needed to compile the library.
globus_url_copy_SOURCES = globus_url_copy.c

# _LDFLAGS is an automake macro which contains
# directives to the linker add before the list of objects and libraries.
# 
# GPT_LDFLAGS is a macro that contains the linker directives GPT generated 
# from the dependent packages
globus_url_copy_LDFLAGS = $(GPT_LDFLAGS)  -export-dynamic

# _LDADD is an automake macro which contains
# directives to the linker added after the list of objects.  
# LDADD is specific for linking programs
# 
# GPT_PGM_LINKS is a macro that contains the linker directives GPT generated 
# from the dependent packages
globus_url_copy_LDADD = ./libglobus_gass_copy.la \
			$(GPT_PGM_LINKS)

if BUILD_MANPAGES
export XML_CATALOG_FILES :=  $(XML_CATALOG_PATH)

MAN_XSL=http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl
DOCBOOK_DTD=http://www.oasis-open.org/docbook/xml/4.4/docbookx.dtd


%.1: %.xml
	$(XMLLINT) --nonet --dtdvalid $(DOCBOOK_DTD) $< > /dev/null
	$(XSLTPROC) --nonet $(MAN_XSL) $<
CLEANFILES = $(man_MANS)
endif

include $(top_srcdir)/globus_automake_post
include $(top_srcdir)/globus_automake_post_top

