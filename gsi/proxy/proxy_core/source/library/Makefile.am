
include $(top_srcdir)/globus_automake_pre

Sources= \
	globus_gsi_proxy.h \
	globus_gsi_proxy.c \
	globus_i_gsi_proxy.h \
	globus_gsi_proxy_handle.c \
	globus_gsi_proxy_handle_attrs.c \
	globus_gsi_proxy_constants.h \
	globus_gsi_proxy_error.c 

include_HEADERS= \
	globus_gsi_proxy.h \
	globus_gsi_proxy_constants.h 

lib_LTLIBRARIES = libglobus_gsi_proxy_core.la

INCLUDES = -I.. @GPT_INCLUDES@

libglobus_gsi_proxy_core_la_SOURCES = $(Sources)

libglobus_gsi_proxy_core_la_LIBADD = @GPT_LIB_LINKS@

libglobus_gsi_proxy_core_la_LDFLAGS = @GPT_LDFLAGS@

include $(top_srcdir)/globus_automake_post
