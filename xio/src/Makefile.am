include $(top_srcdir)/globus_automake_pre
include $(top_srcdir)/globus_automake_pre_top

SUBDIRS=\
	builtins                                    \
	.                                           \
	pkgdata                                     \
	doxygen

Sources= \
	globus_xio_handle.c                         \
	globus_xio_driver.c                         \
	globus_xio_attr.c                           \
	globus_xio_timeout.c                        \
	globus_xio_server.c                         \
	globus_xio_pass.c                           \
	globus_xio_util.c                           \
	globus_xio_wrapblock.c                      \
	globus_xio_system_select.c                  \
	globus_xio_system_win32.c                   \
	globus_i_xio_win32_complete.c               \
	globus_i_xio_win32_file.c \
	globus_i_xio_win32_mode.c \
	globus_i_xio_win32_socket.c \
	globus_i_xio_system_common.c                \
	globus_xio_load.c

include_HEADERS = \
    globus_xio_types.h                          \
    globus_xio_util.h                           \
    globus_xio_load.h                           \
    globus_xio_driver.h                         \
    globus_xio.h                                \
    globus_xio_wrapblock.h                      \
    globus_xio_system.h 

noinst_HEADERS = \
    globus_i_xio.h \
    globus_i_xio_system_common.h

lib_LTLIBRARIES = libglobus_xio.la

INCLUDES = $(GPT_INCLUDES)

libglobus_xio_la_SOURCES = $(Sources) 
libglobus_xio_la_LDFLAGS = $(GPT_LDFLAGS)
libglobus_xio_la_LIBADD = \
	$(GPT_LIB_LINKS) \
	$(XIO_BUILTIN_LIBS)

doc_DATA = GLOBUS_LICENSE

EXTRA_DIST = \
	globus_xio_system_win32.c \
	globus_i_xio_win32.h \
	globus_i_xio_win32_complete.c \
	globus_i_xio_win32_file.c \
	globus_i_xio_win32_mode.c \
	globus_i_xio_win32_socket.c \
	version.h.in \
	extensions.h.in \
	configure.in.in \
	$(doc_DATA)

include $(top_srcdir)/globus_automake_post
include $(top_srcdir)/globus_automake_post_top
