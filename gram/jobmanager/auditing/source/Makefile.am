include $(top_srcdir)/globus_automake_pre
include $(top_srcdir)/globus_automake_pre_top

SUBDIRS= pkgdata

# Where targets are installed
libexec_SCRIPTS = globus-gram-audit

pkgdata_DATA = 

mandatadir=$(mandir)/man8
mandata_DATA = globus-gram-audit.8
MANUAL_SOURCE = manual.xml globus-gram-audit-manual.xml 

# Files which we want to put in the source package
EXTRA_DIST = $(libexec_SCRIPTS) $(mandata_DATA) $(MANUAL_SOURCE)

if GENERATE_MANPAGES
export XML_CATALOG_FILES :=  $(XML_CATALOG_PATH)

$(mandata_DATA): $(MANUAL_SOURCE)
	$(XMLLINT) --xinclude --dtdvalid http://www.oasis-open.org/docbook/xml/4.4/docbookx.dtd manual.xml > /dev/null
	$(XSLTPROC) --xinclude http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl manual.xml
endif

include $(top_srcdir)/globus_automake_post
include $(top_srcdir)/globus_automake_post_top